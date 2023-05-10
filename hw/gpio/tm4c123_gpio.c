/*
 * TM4C123 GPIO
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
#include "hw/gpio/tm4c123_gpio.h"
#include "qemu/log.h"
#include "qemu/module.h"
#include "hw/misc/tm4c123_sysctl.h"
#include "qemu/bitops.h"
#include "trace.h"

#define LOG(fmt, args...) qemu_log("%s: " fmt, __func__, ## args)
#define READONLY LOG("0x%"HWADDR_PRIx" is a readonly field\n.", addr)

static bool gpio_clock_enabled(TM4C123SysCtlState *s, hwaddr addr)
{
    switch(addr) {
        case GPIO_A:
            return test_bit(0, (const unsigned long*)&s->sysctl_rcgcgpio);
            break;
        case GPIO_B:
            return test_bit(1, (const unsigned long*)&s->sysctl_rcgcgpio);
            break;
        case GPIO_C:
            return test_bit(2, (const unsigned long*)&s->sysctl_rcgcgpio);
            break;
        case GPIO_D:
            return test_bit(3, (const unsigned long*)&s->sysctl_rcgcgpio);
            break;
        case GPIO_E:
            return test_bit(4, (const unsigned long*)&s->sysctl_rcgcgpio);
            break;
        case GPIO_F:
            return test_bit(5, (const unsigned long*)&s->sysctl_rcgcgpio);
            break;
    }
    return false;
}

static void tm4c123_gpio_reset(DeviceState *dev)
{
    TM4C123GPIOState *s = TM4C123_GPIO(dev);

    s->gpio_data = 0x00000000;
    s->gpio_dir = 0x00000000;
    s->gpio_is = 0x00000000;
    s->gpio_ibe = 0x00000000;
    s->gpio_iev = 0x00000000;
    s->gpio_im = 0x00000000;
    s->gpio_ris = 0x00000000;
    s->gpio_mis = 0x00000000;
    s->gpio_icr = 0x00000000;
    s->gpio_afsel = 0x00000000;
    s->gpio_dr2r = 0x000000FF;
    s->gpio_dr4r = 0x00000000;
    s->gpio_dr8r = 0x00000000;
    s->gpio_odr = 0x00000000;
    s->gpio_pur = 0x00000000;
    s->gpio_pdr = 0x00000000;
    s->gpio_slr = 0x00000000;
    s->gpio_den = 0x00000000;
    s->gpio_lock = 0x00000001;
    s->gpio_ocr = 0x00000000;
    s->gpio_amsel = 0x00000000;
    s->gpio_pctl = 0x00000000;
    s->gpio_adcctl = 0x00000000;
    s->gpio_dmactl = 0x00000000;
    s->gpio_per_id4 = 0x00000000;
    s->gpio_per_id5 = 0x00000000;
    s->gpio_per_id6 = 0x00000000;
    s->gpio_per_id7 = 0x00000000;
    s->gpio_per_id0 = 0x00000061;
    s->gpio_per_id1 = 0x00000000;
    s->gpio_per_id2 = 0x00000018;
    s->gpio_per_id3 = 0x00000001;
    s->gpio_pcell_id0 = 0x0000000D;
    s->gpio_pcell_id1 = 0x000000F0;
    s->gpio_pcell_id2 = 0x00000005;
    s->gpio_pcell_id3 = 0x000000B1;
}

static void tm4c123_gpio_write(void *opaque, hwaddr addr, uint64_t val64, unsigned int size)
{
    TM4C123GPIOState *s = opaque;
    uint32_t val32 = val64;

    if(!gpio_clock_enabled(s->sysctl, s->mmio.addr)) {
        hw_error("GPIO module clock is not enabled");
    }
    trace_tm4c123_gpio_write(addr, val32);

    switch(addr) {
        case GPIO_DATA:
            {
                uint32_t rising_edge = (val32 ^ s->gpio_data) & val32;
                //level detection
                s->gpio_mis = s->gpio_is & s->gpio_iev & val32;
                s->gpio_mis |= s->gpio_is & ~(s->gpio_iev | val32);
                s->gpio_mis &= s->gpio_im;

                //edge detection
                //both edges
                s->gpio_mis |= (s->gpio_is | s->gpio_ibe) & (~s->gpio_is);
                //rising edge
                s->gpio_mis |= (~(s->gpio_is | s->gpio_ibe | s->gpio_iev)) & rising_edge;
                //falling edge
                s->gpio_mis |= ~(s->gpio_is | s->gpio_ibe | s->gpio_iev | rising_edge);
                s->gpio_mis &= s->gpio_im;
                s->gpio_ris |= s->gpio_mis & val32;

                s->gpio_data = val32;
                if(s->gpio_im != 0)
                    qemu_irq_pulse(s->irq);
            }
            break;
        case GPIO_DIR:
            s->gpio_dir = val32;
            break;
        case GPIO_IS:
            s->gpio_is = val32;
            break;
        case GPIO_IBE:
            s->gpio_ibe = val32;
            break;
        case GPIO_IEV:
            s->gpio_iev = val32;
            break;
        case GPIO_IM:
            s->gpio_im = val32;
            break;
        case GPIO_RIS:
            s->gpio_ris = val32;
            break;
        case GPIO_MIS:
            READONLY;
            break;
        case GPIO_ICR:
            s->gpio_mis ^= val32;
            s->gpio_ris ^= val32;
            s->gpio_icr = val32;
            break;
        case GPIO_AFSEL:
            s->gpio_afsel = val32;
            break;
        case GPIO_DR2R:
            s->gpio_dr2r = val32;
            break;
        case GPIO_DR4R:
            s->gpio_dr4r = val32;
            break;
        case GPIO_DR8R:
            s->gpio_dr8r = val32;
            break;
        case GPIO_ODR:
            s->gpio_odr = val32;
            break;
        case GPIO_PUR:
            s->gpio_pur = val32;
            break;
        case GPIO_PDR:
            s->gpio_pdr = val32;
            break;
        case GPIO_SLR:
            s->gpio_slr = val32;
            break;
        case GPIO_DEN:
            s->gpio_den = val32;
            break;
        case GPIO_LOCK:
            s->gpio_lock = val32;
            break;
        case GPIO_OCR:
            s->gpio_ocr = val32;
            break;
        case GPIO_AMSEL:
            s->gpio_amsel = val32;
            break;
        case GPIO_PCTL:
            s->gpio_pctl = val32;
            break;
        case GPIO_ADCCTL:
            s->gpio_adcctl = val32;
            break;
        case GPIO_DMACTL:
            s->gpio_dmactl = val32;
            break;
        case GPIO_PER_ID4:
            READONLY;
            break;
        case GPIO_PER_ID5:
            READONLY;
            break;
        case GPIO_PER_ID6:
            READONLY;
            break;
        case GPIO_PER_ID7:
            READONLY;
            break;
        case GPIO_PER_ID0:
            READONLY;
            break;
        case GPIO_PER_ID1:
            READONLY;
            break;
        case GPIO_PER_ID2:
            READONLY;
            break;
        case GPIO_PER_ID3:
            READONLY;
            break;
        case GPIO_PCELL_ID0:
            READONLY;
            break;
        case GPIO_PCELL_ID1:
            READONLY;
            break;
        case GPIO_PCELL_ID2:
            READONLY;
            break;
        case GPIO_PCELL_ID3:
            READONLY;
            break;
        default:
            LOG("Bad address 0x%"HWADDR_PRIx"\n", addr);
    }
}

static uint64_t tm4c123_gpio_read(void *opaque, hwaddr addr, unsigned int size)
{
    TM4C123GPIOState *s = opaque;

    trace_tm4c123_gpio_read(addr);

    if(!gpio_clock_enabled(s->sysctl, s->mmio.addr)) {
        hw_error("GPIO module clock is not enabled");
    }

    switch(addr) {
        case GPIO_DATA:
            return s->gpio_data;
        case GPIO_DIR:
            return s->gpio_dir;
        case GPIO_IS:
            return s->gpio_is;
        case GPIO_IBE:
            return s->gpio_ibe;
        case GPIO_IEV:
            return s->gpio_iev;
        case GPIO_IM:
            return s->gpio_im;
        case GPIO_RIS:
            return s->gpio_ris;
        case GPIO_MIS:
            return s->gpio_mis;
        case GPIO_ICR:
            return s->gpio_icr;
        case GPIO_AFSEL:
            return s->gpio_afsel;
        case GPIO_DR2R:
            return s->gpio_dr2r;
        case GPIO_DR4R:
            return s->gpio_dr4r;
        case GPIO_DR8R:
            return s->gpio_dr8r;
        case GPIO_ODR:
            return s->gpio_odr;
        case GPIO_PUR:
            return s->gpio_pur;
        case GPIO_PDR:
            return s->gpio_pdr;
        case GPIO_SLR:
            return s->gpio_slr;
        case GPIO_DEN:
            return s->gpio_den;
        case GPIO_LOCK:
            return s->gpio_lock;
        case GPIO_OCR:
            return s->gpio_ocr;
        case GPIO_AMSEL:
            return s->gpio_amsel;
        case GPIO_PCTL:
            return s->gpio_pctl;
        case GPIO_ADCCTL:
            return s->gpio_adcctl;
        case GPIO_DMACTL:
            return s->gpio_dmactl;
        case GPIO_PER_ID4:
            return s->gpio_per_id4;
        case GPIO_PER_ID5:
            return s->gpio_per_id5;
        case GPIO_PER_ID6:
            return s->gpio_per_id6;
        case GPIO_PER_ID7:
            return s->gpio_per_id7;
        case GPIO_PER_ID0:
            return s->gpio_per_id0;
        case GPIO_PER_ID1:
            return s->gpio_per_id1;
        case GPIO_PER_ID2:
            return s->gpio_per_id2;
        case GPIO_PER_ID3:
            return s->gpio_per_id3;
        case GPIO_PCELL_ID0:
            return s->gpio_pcell_id0;
        case GPIO_PCELL_ID1:
            return s->gpio_pcell_id1;
        case GPIO_PCELL_ID2:
            return s->gpio_pcell_id2;
        case GPIO_PCELL_ID3:
            return s->gpio_pcell_id3;
        default:
            LOG("Bad address 0x%"HWADDR_PRIx"\n", addr);
    }
    return 0;
}

static const MemoryRegionOps tm4c123_gpio_ops = {
    .read = tm4c123_gpio_read,
    .write = tm4c123_gpio_write,
    .endianness = DEVICE_NATIVE_ENDIAN,
};

static void tm4c123_gpio_init(Object *obj)
{
    TM4C123GPIOState *s = TM4C123_GPIO(obj);

    sysbus_init_irq(SYS_BUS_DEVICE(obj), &s->irq);
    memory_region_init_io(&s->mmio, obj, &tm4c123_gpio_ops, s, TYPE_TM4C123_GPIO, 0xFFF);
    sysbus_init_mmio(SYS_BUS_DEVICE(obj), &s->mmio);
}

static void tm4c123_gpio_class_init(ObjectClass *kclass, void *data)
{
    DeviceClass *dc = DEVICE_CLASS(kclass);
    dc->reset = tm4c123_gpio_reset;
}

static const TypeInfo tm4c123_gpio_info = {
    .name = TYPE_TM4C123_GPIO,
    .parent = TYPE_SYS_BUS_DEVICE,
    .instance_size = sizeof(TM4C123GPIOState),
    .instance_init = tm4c123_gpio_init,
    .class_init = tm4c123_gpio_class_init,
};

static void tm4c123_gpio_register_types(void)
{
    type_register_static(&tm4c123_gpio_info);
}

type_init(tm4c123_gpio_register_types)
