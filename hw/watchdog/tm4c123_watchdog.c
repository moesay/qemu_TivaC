/*
 * TM4C123 Watchdog Timers
 *
 * Copyright (c) 2023 Mohamed ElSayed <m.elsayed4420@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "qemu/osdep.h"
#include "hw/watchdog/tm4c123_watchdog.h"
#include "hw/irq.h"
#include "hw/qdev-properties.h"
#include "hw/qdev-clock.h"
#include "sysemu/runstate.h"
#include "qemu/log.h"
#include "qemu/module.h"
#include "hw/nmi.h"
#include "trace.h"

#define READONLY LOG("0x%"HWADDR_PRIx" is a readonly field\n.", addr)
#define LOG(fmt, args...) qemu_log("%s: " fmt, __func__, ## args)

static bool locked = false;

static void tm4c123_wdt_expired(void *opaque)
{
    TM4C123WatchdogState *s = opaque;
    /*if this is the first timeout/the ris is not cleared */
    if(!test_bit(0, (const unsigned long*)&s->wdt_mis)) {
        set_bit(0, (unsigned long*)&s->wdt_mis);
        nmi_monitor_handle(0, NULL);
        qemu_irq_pulse(s->irq);
    }
    else {
        if(test_bit(1, (const unsigned long*)&s->wdt_ctl))
            qemu_system_reset_request(SHUTDOWN_CAUSE_GUEST_RESET);
        else {
            nmi_monitor_handle(0, NULL);
            qemu_irq_pulse(s->irq);
        }
    }
}

static bool wdt_clock_enabled(TM4C123SysCtlState *s, hwaddr addr)
{
    switch(addr) {
        case WDT_0:
            return (s->sysctl_rcgcwd & (1 << 0));
            break;
        case WDT_1:
            return (s->sysctl_rcgcwd & (1 << 1));
            break;
    }
    return false;
}

static void tm4c123_wdt_reset(DeviceState *dev)
{
    TM4C123WatchdogState *s = TM4C123_WATCHDOG(dev);

    s->wdt_load = 0xFFFFFFFF;
    s->wdt_value = 0xFFFFFFFF;
    s->wdt_ctl = (s->mmio.addr == WDT_0 ? 0x00000000 : 0x80000000);
    s->wdt_icr = 0x00000000;
    s->wdt_ris = 0x00000000;
    s->wdt_mis = 0x00000000;
    s->wdt_test = 0x00000000;
    s->wdt_lock = 0x00000000;
    s->wdt_per_id4 = 0x00000000;
    s->wdt_per_id5 = 0x00000000;
    s->wdt_per_id6 = 0x00000000;
    s->wdt_per_id7 = 0x00000000;
    s->wdt_per_id0 = 0x00000005;
    s->wdt_per_id1 = 0x00000018;
    s->wdt_per_id2 = 0x00000018;
    s->wdt_per_id3 = 0x00000001;
    s->wdt_pcell_id0 = 0x0000000D;
    s->wdt_pcell_id1 = 0x000000F0;
    s->wdt_pcell_id2 = 0x00000006;
    s->wdt_pcell_id3 = 0x000000B1;
}

static uint64_t tm4c123_wdt_read(void *opaque, hwaddr addr, unsigned int size)
{
    TM4C123WatchdogState *s = opaque;

    if(!wdt_clock_enabled(s->sysctl, s->mmio.addr)) {
        hw_error("Watchdog timer module clock is not enabled");
    }

    switch(addr) {
        case WDT_LOAD:
            return s->wdt_load;
        case WDT_VALUE:
            return ptimer_get_count(s->timer);
        case WDT_CTL:
            return s->wdt_ctl;
        case WDT_ICR:
            return s->wdt_icr;
        case WDT_RIS:
            return s->wdt_ris;
        case WDT_MIS:
            return s->wdt_mis;
        case WDT_TEST:
            return s->wdt_test;
        case WDT_LOCK:
            return s->wdt_lock;
        case WDT_PER_ID4:
            return s->wdt_per_id4;
        case WDT_PER_ID5:
            return s->wdt_per_id5;
        case WDT_PER_ID6:
            return s->wdt_per_id6;
        case WDT_PER_ID7:
            return s->wdt_per_id7;
        case WDT_PER_ID0:
            return s->wdt_per_id0;
        case WDT_PER_ID1:
            return s->wdt_per_id1;
        case WDT_PER_ID2:
            return s->wdt_per_id2;
        case WDT_PER_ID3:
            return s->wdt_per_id3;
        case WDT_PCELL_ID0:
            return s->wdt_pcell_id0;
        case WDT_PCELL_ID1:
            return s->wdt_pcell_id1;
        case WDT_PCELL_ID2:
            return s->wdt_pcell_id2;
        case WDT_PCELL_ID3:
            return s->wdt_pcell_id3;
        default:
            LOG("Bad address 0x%"HWADDR_PRIx"\n", addr);
    }
    return 0;
}

static void tm4c123_wdt_write(void *opaque, hwaddr addr, uint64_t val64, unsigned int size)
{
    TM4C123WatchdogState *s = opaque;
    uint32_t val32 = val64;

    trace_tm4c123_wdt_write(addr, val64);
    if(!wdt_clock_enabled(s->sysctl, s->mmio.addr)) {
        hw_error("Watchdog module clock is not enabled");
    }

    switch(addr) {
        case WDT_LOAD:
            s->wdt_load = val32;
            locked = true;
            s->wdt_ctl |= WDT_CTL_INTEN;
            ptimer_transaction_begin(s->timer);
            ptimer_set_count(s->timer, s->wdt_load);
            ptimer_set_limit(s->timer, s->wdt_load, 1);
            ptimer_run(s->timer, 0);
            ptimer_transaction_commit(s->timer);
            break;
        case WDT_VALUE:
            READONLY;
            break;
        case WDT_CTL:
            s->wdt_ctl = val32;
            break;
        case WDT_ICR:
            ptimer_transaction_begin(s->timer);
            ptimer_set_limit(s->timer, s->wdt_load, 1);
            ptimer_transaction_commit(s->timer);
            clear_bit(0, (unsigned long*)&s->wdt_ris);
            clear_bit(0, (unsigned long*)&s->wdt_mis);
            s->wdt_icr = val32;
            break;
        case WDT_RIS:
            READONLY;
            break;
        case WDT_MIS:
            READONLY;
            break;
        case WDT_TEST:
            s->wdt_test = val32;
            break;
        case WDT_LOCK:
            /* The actual hardware never locks the module */
            if(val32 == UNLOCK_VALUE) {
                locked = false;
                s->wdt_lock = 0;
            }
            break;
        case WDT_PER_ID4:
            READONLY;
            break;
        case WDT_PER_ID5:
            READONLY;
            break;
        case WDT_PER_ID6:
            READONLY;
            break;
        case WDT_PER_ID7:
            READONLY;
            break;
        case WDT_PER_ID0:
            READONLY;
            break;
        case WDT_PER_ID1:
            READONLY;
            break;
        case WDT_PER_ID2:
            READONLY;
            break;
        case WDT_PER_ID3:
            READONLY;
            break;
        case WDT_PCELL_ID0:
            READONLY;
            break;
        case WDT_PCELL_ID1:
            READONLY;
            break;
        case WDT_PCELL_ID2:
            READONLY;
            break;
        case WDT_PCELL_ID3:
            READONLY;
            break;
        default:
            LOG("Bad address 0x%"HWADDR_PRIx"\n", addr);
    }
}

const struct MemoryRegionOps tm4c123_wdt_ops = {
    .read = tm4c123_wdt_read,
    .write = tm4c123_wdt_write,
    .endianness = DEVICE_NATIVE_ENDIAN,
};

static void tm4c123_wdt_init(Object *obj)
{
    TM4C123WatchdogState *s = TM4C123_WATCHDOG(obj);

    s->wdt_clock = qdev_init_clock_in(DEVICE(s), "wdt_clock", NULL, NULL, 0);

    sysbus_init_irq(SYS_BUS_DEVICE(obj), &s->irq);
    memory_region_init_io(&s->mmio, obj, &tm4c123_wdt_ops, s, TYPE_TM4C123_WATCHDOG, 0xFFF);
    sysbus_init_mmio(SYS_BUS_DEVICE(obj), &s->mmio);
}

static void tm4c123_wdt_realize(DeviceState *dev, Error **errp)
{
    TM4C123WatchdogState *s = TM4C123_WATCHDOG(dev);
    qdev_connect_clock_in(dev, "wdt_clock", qdev_get_clock_out(DEVICE(s->sysctl), "outclk"));

    s->timer = ptimer_init(tm4c123_wdt_expired, s,
                           PTIMER_POLICY_NO_IMMEDIATE_RELOAD |
                           PTIMER_POLICY_NO_COUNTER_ROUND_DOWN);

    ptimer_transaction_begin(s->timer);
    ptimer_set_period_from_clock(s->timer, s->wdt_clock, 1);
    ptimer_set_limit(s->timer, 0xFFFFFFFF, 0);
    ptimer_transaction_commit(s->timer);
}

static void tm4c123_wdt_class_init(ObjectClass *kclass, void *data)
{
    DeviceClass *dc = DEVICE_CLASS(kclass);
    dc->realize = tm4c123_wdt_realize;
    dc->reset = tm4c123_wdt_reset;
}

static const TypeInfo tm4c123_wdt_info = {
    .name = TYPE_TM4C123_WATCHDOG,
    .parent = TYPE_SYS_BUS_DEVICE,
    .instance_size = sizeof(TM4C123WatchdogState),
    .instance_init = tm4c123_wdt_init,
    .class_init = tm4c123_wdt_class_init,
};

static void tm4c123_wdt_register_types(void)
{
    type_register_static(&tm4c123_wdt_info);
}

type_init(tm4c123_wdt_register_types)
