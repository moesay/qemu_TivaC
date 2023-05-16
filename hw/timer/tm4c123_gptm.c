/*
 * TM4C123 General purpose timers
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

#include "hw/timer/tm4c123_gptm.h"
#include "hw/irq.h"
#include "trace.h"
#include "qemu/timer.h"
#include <time.h>

#define LOG(fmt, args...) qemu_log("%s: " fmt, __func__, ## args)
#define READONLY LOG("0x%"HWADDR_PRIx" is a readonly field\n.", addr)

static uint64_t ns_to_ticks(void *opaque, uint64_t ns, uint32_t prescaler)
{
    TM4C123GPTMState *s = opaque;

    uint32_t freq = clock_get_hz(s->clk) / prescaler;
    float sec = (float)ns / (float)NANOSECONDS_PER_SECOND;
    return sec * freq;
}

static unsigned long ticks_to_time_ns(void *opaque, uint64_t ticks, uint32_t prescaler)
{
    TM4C123GPTMState *s = opaque;
    uint32_t freq = clock_get_hz(s->clk) / prescaler;
    return (((float)ticks / (float)freq) * NANOSECONDS_PER_SECOND);
}

static void log_message(const char *message)
{
    uint64_t ns = qemu_clock_get_ns(QEMU_CLOCK_REALTIME);
    time_t seconds = ns / 1000000000;
    struct tm *timeinfo = localtime(&seconds);
    char buffer[80];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeinfo);
    LOG("[%s] %s\n", buffer, message);
}

static uint16_t get_timer_width(void *opaque)
{
    TM4C123GPTMState *s = opaque;
    switch(s->mmio.addr) {
        case TIMER0_32...TIMER5_32:
            return TIMER_WIDTH_32;
        case TIMER0_64...TIMER5_64:
            return TIMER_WIDTH_64;
    }
    return 0;
}

static uint64_t build_interval_value(void *opaque)
{
    TM4C123GPTMState *s = opaque;
    uint16_t timer_width = get_timer_width(s);
    uint64_t interval_value = 0;
    if(timer_width == TIMER_WIDTH_32) {
        /* timer is in 32 bit mode or 32bit rtc*/
        uint16_t upper16 = extract32(s->gptm_talir, 16, 16);
        uint16_t lower16 = extract32(s->gptm_tblir, 0, 16);
        interval_value = ((uint32_t)lower16 << 16) + upper16;
    }
    else if(timer_width == TIMER_WIDTH_64) {
        interval_value = ((uint64_t)s->gptm_talir << 32) + s->gptm_tblir;
    }

    trace_tm4c123_gptm_build_interval_value(s->gptm_talir, s->gptm_tblir, interval_value);
    return interval_value;
}

static void set_timers(void *opaque)
{
    TM4C123GPTMState *s = opaque;
    uint64_t interval_value;
    uint16_t timer_width;
    if(s->gptm_ctl & GPTM_TACTL_EN) {
        timer_width = get_timer_width(s);
        if(timer_width == TIMER_WIDTH_32) {
            /* What is the mode of the timer? 16/32 */
            if(s->gptm_cfg == 0x4) {
                /* 16 bit mode */
                interval_value = extract32(s->gptm_talir, 0, 16);
                /* Start the timer? */
                timer_mod(s->a, qemu_clock_get_ns(QEMU_CLOCK_VIRTUAL) + ticks_to_time_ns(s, interval_value, s->gptm_tapr));
                LOG("Timer A/16 is running\n");
            }
            else if(s->gptm_cfg == 0x1) {
                /* 32 bit mode rtc */
                interval_value = build_interval_value(s);
                timer_mod(s->a, qemu_clock_get_ns(QEMU_CLOCK_REALTIME) + ticks_to_time_ns(s, interval_value, s->gptm_tapr));
                LOG("Timer A/32 RTC is running\n");
            }
            else if(s->gptm_cfg == 0x0) {
                /* 32 bit mode rtc */
                interval_value = build_interval_value(s);
                timer_mod(s->a, qemu_clock_get_ns(QEMU_CLOCK_VIRTUAL) + ticks_to_time_ns(s, interval_value, s->gptm_tapr));
                LOG("Timer A/32 is running\n");
            }
        }
        else if(timer_width == TIMER_WIDTH_64) {
            /* What is the mode of the timer? 32/64 */
            if(s->gptm_cfg == 0) {
                /* 64 bit mode */
                interval_value = build_interval_value(s);
                timer_mod(s->a, qemu_clock_get_ns(QEMU_CLOCK_VIRTUAL) + ticks_to_time_ns(s, interval_value, s->gptm_tapr));
                LOG("Timer A/64 is running\n");
            }
            else if(s->gptm_cfg == 0x1) {
                /* 64 bit mode */
                interval_value = build_interval_value(s);
                timer_mod(s->a, qemu_clock_get_ns(QEMU_CLOCK_REALTIME) + ticks_to_time_ns(s, interval_value, s->gptm_tapr));
                LOG("Timer A/64 RTC is running\n");
            }
            else if(s->gptm_cfg == 0x4) {
                interval_value = s->gptm_talir;
                timer_mod(s->a, qemu_clock_get_ns(QEMU_CLOCK_REALTIME) + ticks_to_time_ns(s, interval_value, s->gptm_tapr));
                LOG("Timer A/32 RTC is running\n");
            }
        }
    }
    else if(s->gptm_ctl & GPTM_TBCTL_EN) {
        timer_width = get_timer_width(s);
        if(timer_width == TIMER_WIDTH_32) {
            /* What is the mode of the timer? 16/32 */
            if(s->gptm_cfg == 0x4) {
                /* 16 bit mode */
                interval_value = extract32(s->gptm_tblir, 0, 16);
                /* Start the timer? */
                timer_mod(s->b, qemu_clock_get_ns(QEMU_CLOCK_VIRTUAL) + ticks_to_time_ns(s, interval_value, s->gptm_tbpr));
                LOG("Timer B/16 is running\n");
            }
            else if(s->gptm_cfg == 0x01) {
                /* 32 bit mode rtc */
                interval_value = build_interval_value(s);
                timer_mod(s->b, qemu_clock_get_ns(QEMU_CLOCK_REALTIME) + ticks_to_time_ns(s, interval_value, s->gptm_tbpr));
                LOG("Timer B/32 RTC is running\n");
            }
            else if(s->gptm_cfg == 0x00) {
                /* 32 bit mode rtc */
                interval_value = build_interval_value(s);
                timer_mod(s->b, qemu_clock_get_ns(QEMU_CLOCK_VIRTUAL) + ticks_to_time_ns(s, interval_value, s->gptm_tbpr));
                LOG("Timer B/32 RTC is running\n");
            }
        }
        else if(timer_width == TIMER_WIDTH_64) {
            /* What is the mode of the timer? 32/64 */
            if(s->gptm_cfg == 0) {
                /* 64 bit mode */
                interval_value = build_interval_value(s);
                timer_mod(s->b, qemu_clock_get_ns(QEMU_CLOCK_VIRTUAL) + ticks_to_time_ns(s, interval_value, s->gptm_tbpr));
                LOG("Timer A/64 is running\n");
            }
            else if(s->gptm_cfg == 0x1) {
                /* 64 bit mode */
                interval_value = build_interval_value(s);
                timer_mod(s->b, qemu_clock_get_ns(QEMU_CLOCK_REALTIME) + ticks_to_time_ns(s, interval_value, s->gptm_tbpr));
                LOG("Timer A/64 RTC is running\n");
            }
            else if(s->gptm_cfg == 0x4) {
                interval_value = s->gptm_tblir;
                timer_mod(s->b, qemu_clock_get_ns(QEMU_CLOCK_REALTIME) + ticks_to_time_ns(s, interval_value, s->gptm_tbpr));
                LOG("Timer A/32 RTC is running\n");
            }
        }
    }
}

static bool gptm_clock_enabled(TM4C123SysCtlState *s, hwaddr addr)
{
    switch(addr) {
        case TIMER0_32:
            return test_bit(0, (const unsigned long*)&s->sysctl_rcgctimer);
            break;
        case TIMER1_32:
            return test_bit(1, (const unsigned long*)&s->sysctl_rcgctimer);
            break;
        case TIMER2_32:
            return test_bit(2, (const unsigned long*)&s->sysctl_rcgctimer);
            break;
        case TIMER3_32:
            return test_bit(3, (const unsigned long*)&s->sysctl_rcgctimer);
            break;
        case TIMER4_32:
            return test_bit(4, (const unsigned long*)&s->sysctl_rcgctimer);
            break;
        case TIMER5_32:
            return test_bit(5, (const unsigned long*)&s->sysctl_rcgctimer);
            break;
        case TIMER0_64:
            return test_bit(0, (const unsigned long*)&s->sysctl_rcgcwtimer);
            break;
        case TIMER1_64:
            return test_bit(1, (const unsigned long*)&s->sysctl_rcgcwtimer);
            break;
        case TIMER2_64:
            return test_bit(2, (const unsigned long*)&s->sysctl_rcgcwtimer);
            break;
        case TIMER3_64:
            return test_bit(3, (const unsigned long*)&s->sysctl_rcgcwtimer);
            break;
        case TIMER4_64:
            return test_bit(4, (const unsigned long*)&s->sysctl_rcgcwtimer);
            break;
        case TIMER5_64:
            return test_bit(5, (const unsigned long*)&s->sysctl_rcgcwtimer);
            break;
    }
    return false;
}

static void tm4c123_gptm_reset(DeviceState* dev)
{
    TM4C123GPTMState *s = TM4C123_GPTM(dev);

    s->gptm_cfg = 0x00000000;
    s->gptm_amr = 0x00000000;
    s->gptm_bmr = 0x00000000;
    s->gptm_ctl = 0x00000000;
    s->gptm_sync = 0x00000000;
    s->gptm_imr = 0x00000000;
    s->gptm_ris = 0x00000000;
    s->gptm_mis = 0x00000000;
    s->gptm_icr = 0x00000000;
    s->gptm_talir = 0xFFFFFFFF;
    s->gptm_tblir = 0x00000000;
    s->gptm_tamatchr = 0xFFFFFFFF;
    s->gptm_tbmatchr = 0x00000000;
    s->gptm_tapr = 0x00000000;
    s->gptm_tbpr = 0x00000000;
    s->gptm_tapmr = 0x00000000;
    s->gptm_tbpmr = 0x00000000;
    s->gptm_tar = 0xFFFFFFFF;
    s->gptm_tbr = 0x00000000;
    s->gptm_tav = 0xFFFFFFFF;
    s->gptm_tbv = 0x00000000;
    s->gptm_rtcpd = 0x00007FFF;
    s->gptm_taps = 0x00000000;
    s->gptm_tbps = 0x00000000;
    s->gptm_tapv = 0x00000000;
    s->gptm_tbpv = 0x00000000;
    s->gptm_pp = 0x00000000;
}

static uint64_t tm4c123_gptm_read(void *opaque, hwaddr addr, unsigned int size)
{
    TM4C123GPTMState *s = opaque;

    if(!gptm_clock_enabled(s->sysctl, s->mmio.addr)) {
        hw_error("GPTM module clock is not enabled");
    }

    trace_tm4c123_gptm_read(addr);

    switch(addr) {
        case GPTM_CFG:
            return s->gptm_cfg;
        case GPTM_AMR:
            return s->gptm_amr;
        case GPTM_BMR:
            return s->gptm_bmr;
        case GPTM_CTL:
            return s->gptm_ctl;
        case GPTM_SYNC:
            return s->gptm_sync;
        case GPTM_IMR:
            return s->gptm_imr;
        case GPTM_RIS:
            return s->gptm_ris;
        case GPTM_MIS:
            return s->gptm_mis;
        case GPTM_ICR:
            return s->gptm_icr;
        case GPTM_TALIR:
            return s->gptm_talir;
        case GPTM_TBLIR:
            return s->gptm_tblir;
        case GPTM_TAMATCHR:
            return s->gptm_tamatchr;
        case GPTM_TBMATCHR:
            return s->gptm_tbmatchr;
        case GPTM_TAPR:
            return s->gptm_tapr;
        case GPTM_TBPR:
            return s->gptm_tbpr;
        case GPTM_TAPMR:
            return s->gptm_tapmr;
        case GPTM_TBPMR:
            return s->gptm_tbpmr;
        case GPTM_TAR:
            return s->gptm_tar;
        case GPTM_TBR:
            return s->gptm_tbr;
        case GPTM_TAV:
            if(get_timer_width(s) == TIMER_WIDTH_64 && s->gptm_cfg == 0) {
                return extract64(
                        ns_to_ticks(s, s->a->expire_time - qemu_clock_get_ns(QEMU_CLOCK_VIRTUAL), s->gptm_tapr),
                        0, 31);
            }
            else {
                return ns_to_ticks(s, s->a->expire_time - qemu_clock_get_ns(QEMU_CLOCK_VIRTUAL), s->gptm_tapr);
            }
        case GPTM_TBV:
            if(get_timer_width(s) == TIMER_WIDTH_64 && s->gptm_cfg == 0) {
                return extract64(
                        ns_to_ticks(s, s->a->expire_time - qemu_clock_get_ns(QEMU_CLOCK_VIRTUAL), s->gptm_tapr),
                        32, 64);
            }
            else {
                return ns_to_ticks(s, s->b->expire_time - qemu_clock_get_ns(QEMU_CLOCK_VIRTUAL), s->gptm_tabr);
            }
        case GPTM_RTCPD:
            return s->gptm_rtcpd;
        case GPTM_TAPS:
            return s->gptm_taps;
        case GPTM_TBPS:
            return s->gptm_tbps;
        case GPTM_TAPV:
            return s->gptm_tapv;
        case GPTM_TBPV:
            return s->gptm_tbpv;
        case GPTM_PP:
            return s->gptm_pp;
        default:
            LOG("Bad address 0x%"HWADDR_PRIx"\n", addr);
    }

    return 0;
}

static void tm4c123_gptm_write(void *opaque, hwaddr addr, uint64_t val64, unsigned int size)
{
    TM4C123GPTMState *s = opaque;
    uint32_t val32 = val64;

    if(!gptm_clock_enabled(s->sysctl, s->mmio.addr)) {
        hw_error("GPTM module clock is not enabled");
    }

    trace_tm4c123_gptm_write(addr, val32);

    switch(addr) {
        case GPTM_CFG:
            s->gptm_cfg = val32;
            break;
        case GPTM_AMR:
            s->gptm_amr = val32;
            break;
        case GPTM_BMR:
            s->gptm_bmr = val32;
            break;
        case GPTM_CTL:
            s->gptm_ctl = val32;
            set_timers(s);
            break;
        case GPTM_SYNC:
            s->gptm_sync = val32;
            break;
        case GPTM_IMR:
            s->gptm_imr = val32;
            break;
        case GPTM_RIS:
            READONLY;
            break;
        case GPTM_MIS:
            s->gptm_mis = val32;
            break;
        case GPTM_ICR:
            s->gptm_ris &= ~val32;
            s->gptm_mis &= ~val32;
            break;
        case GPTM_TALIR:
            s->gptm_talir = val32;
            break;
        case GPTM_TBLIR:
            s->gptm_tblir = val32;
            break;
        case GPTM_TAMATCHR:
            s->gptm_tamatchr = val32;
            break;
        case GPTM_TBMATCHR:
            s->gptm_tbmatchr = val32;
            break;
        case GPTM_TAPR:
            s->gptm_tapr = val32;
            break;
        case GPTM_TBPR:
            s->gptm_tbpr = val32;
            break;
        case GPTM_TAPMR:
            s->gptm_tapmr = val32;
            break;
        case GPTM_TBPMR:
            s->gptm_tbpmr = val32;
            break;
        case GPTM_TAR:
            READONLY;
            break;
        case GPTM_TBR:
            READONLY;
            break;
        case GPTM_TAV:
            s->gptm_tav = val32;
            break;
        case GPTM_TBV:
            s->gptm_tbv = val32;
            break;
        case GPTM_RTCPD:
            READONLY;
            break;
        case GPTM_TAPS:
            READONLY;
            break;
        case GPTM_TBPS:
            READONLY;
            break;
        case GPTM_TAPV:
            READONLY;
            break;
        case GPTM_TBPV:
            READONLY;
            break;
        case GPTM_PP:
            READONLY;
            break;
        default:
            LOG("Bad address 0x%"HWADDR_PRIx"\n", addr);
    }
}

static const MemoryRegionOps tm4c123_gptm_ops = {
    .read = tm4c123_gptm_read,
    .write = tm4c123_gptm_write,
    .endianness = DEVICE_NATIVE_ENDIAN,
};

static void timer_a_callback(void *opaque)
{
    TM4C123GPTMState *s = opaque;

    log_message("timer A\n");

    if(test_bit(0, (unsigned long*)&s->gptm_imr)) {
        qemu_irq_pulse(s->irq_a);
        set_bit(0, (unsigned long*)&s->gptm_mis);
    }
    set_bit(0, (unsigned long*)&s->gptm_ris);
    if ((s->gptm_amr & 0x0000000F) == 0x2) {
        set_timers(s);
    }
}

static void timer_b_callback(void *opaque)
{
    TM4C123GPTMState *s = opaque;
    log_message("timer B\n");

    if(test_bit(8, (unsigned long*)&s->gptm_imr)) {
        qemu_irq_pulse(s->irq_b);
        set_bit(8, (unsigned long*)&s->gptm_mis);
    }
    set_bit(8, (unsigned long*)&s->gptm_ris);
    if ((s->gptm_bmr & 0x0000000F) == 0x2) {
        set_timers(s);
    }
}

static void tm4c123_gptm_init(Object *obj)
{
    TM4C123GPTMState*s = TM4C123_GPTM(obj);
    s->clk = qdev_init_clock_in(DEVICE(s), "gptm_clock", NULL, NULL, 0);
    s->a = timer_new_ns(QEMU_CLOCK_VIRTUAL, timer_a_callback, s);
    s->b = timer_new_ns(QEMU_CLOCK_VIRTUAL, timer_b_callback, s);
    timer_init_ns(s->a, QEMU_CLOCK_VIRTUAL, timer_a_callback, s);
    timer_init_ns(s->b, QEMU_CLOCK_VIRTUAL, timer_b_callback, s);

    sysbus_init_irq(SYS_BUS_DEVICE(obj), &s->irq_a);
    sysbus_init_irq(SYS_BUS_DEVICE(obj), &s->irq_b);
    memory_region_init_io(&s->mmio, obj, &tm4c123_gptm_ops, s, TYPE_TM4C123_GPTM, 0xFFF);
    sysbus_init_mmio(SYS_BUS_DEVICE(obj), &s->mmio);
}

static void tm4c123_gptm_realize(DeviceState *dev, Error **errp)
{
    TM4C123GPTMState *s = TM4C123_GPTM(dev);
    qdev_connect_clock_in(dev, "gptm_clock", qdev_get_clock_out(DEVICE(s->sysctl), "outclk"));

}

static void tm4c123_gptm_class_init(ObjectClass *kclass, void *data)
{
    DeviceClass *dc = DEVICE_CLASS(kclass);
    dc->reset = tm4c123_gptm_reset;
    dc->realize = tm4c123_gptm_realize;
}

static const TypeInfo tm4c123_gptm_info = {
    .name = TYPE_TM4C123_GPTM,
    .parent = TYPE_SYS_BUS_DEVICE,
    .instance_size = sizeof(TM4C123GPTMState),
    .instance_init = tm4c123_gptm_init,
    .class_init = tm4c123_gptm_class_init,
};

static void tm4c123_gptm_register_types(void)
{
    type_register_static(&tm4c123_gptm_info);
}

type_init(tm4c123_gptm_register_types)
