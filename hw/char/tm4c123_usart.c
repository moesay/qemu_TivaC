/*
 * TM4C123 USART
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
#include "hw/char/tm4c123_usart.h"
#include "hw/irq.h"
#include "hw/qdev-properties.h"
#include "hw/qdev-properties-system.h"
#include "qemu/log.h"
#include "qemu/module.h"

#ifndef TM4C123_USART_ERR_DEBUG
#define TM4C123_USART_ERR_DEBUG 3
#endif

#define DB_PRINT_L(lvl, fmt, args...) do { \
    if (TM4C123_USART_ERR_DEBUG >= lvl) { \
        qemu_log("%s: " fmt, __func__, ## args); \
    } \
} while (0)

#define DB_PRINT(fmt, args...) DB_PRINT_L(1, fmt, ## args)

static int tm4c123_usart_can_receive(void* opaque)
{
    TM4C123USARTState *s = opaque;

    if(!(s->usart_fr & USART_FR_RXFF)) {
        //the module can receive data.
        return 1;
    }
    return 0;
}

static void tm4c123_usart_receive(void *opaque, const uint8_t *buf, int size)
{
    TM4C123USARTState *s = opaque;

    if(!(s->usart_ctl & USART_CR_EN && s->usart_ctl & USART_CR_RXE)) {
        //the module is not enabled
        DB_PRINT("The module is not enbled\n");
        return;
    }

    s->usart_dr = *buf;
    s->usart_ctl &= ~USART_FR_RXFE;

    if(s->usart_im & USART_IM_RXIM) {
        qemu_set_irq(s->irq, 1);
    }

    DB_PRINT("Receiving: %c\n", s->usart_dr);
}

static void tm4c123_usart_reset(DeviceState *dev)
{
    TM4C123USARTState *s = TM4C123_USART(dev);

    s->usart_dr = 0x00000000;
    s->usart_rsr = 0x00000000;
    s->usart_fr = 0x00000090;
    s->usart_ilpr = 0x00000000;
    s->usart_ibrd = 0x00000000;
    s->usart_fbrd = 0x00000000;
    s->usart_lcrh = 0x00000000;
    s->usart_ctl = 0x00000300;
    s->usart_ifls = 0x00000012;
    s->usart_im = 0x00000000;
    s->usart_ris = 0x00000000;
    s->usart_mis = 0x00000000;
    s->usart_icr = 0x00000000;
    s->usart_dma_ctl = 0x00000000;
    s->usart_9bit_addr = 0x00000000;
    s->usart_9bit_mask = 0x000000FF;
    s->usart_pp = 0x00000003;
    s->usart_cc = 0x00000000;
    s->usart_per_id4 = 0x00000000;
    s->usart_per_id5 = 0x00000000;
    s->usart_per_id6 = 0x00000000;
    s->usart_per_id7 = 0x00000000;
    s->usart_per_id0 = 0x00000060;
    s->usart_per_id1 = 0x00000000;
    s->usart_per_id2 = 0x00000018;
    s->usart_per_id3 = 0x00000001;
    s->usart_pcell_id0 = 0x0000000D;
    s->usart_pcell_id1 = 0x000000F0;
    s->usart_pcell_id2 = 0x00000005;
    s->usart_pcell_id3 = 0x000000B1;

    qemu_set_irq(s->irq, 0);
}

static uint64_t tm4c123_usart_read(void *opaque, hwaddr addr, unsigned int size)
{
    TM4C123USARTState *s = opaque;
    DB_PRINT("Reading from: 0x%"HWADDR_PRIx"\n", addr);
    switch(addr) {
        case USART_DR:
            return s->usart_dr;
        case USART_RSR:
            return s->usart_rsr;
        case USART_FR:
            return s->usart_fr;
        case USART_ILPR:
            return s->usart_ilpr;
        case USART_IBRD:
            return s->usart_ibrd;
        case USART_FBRD:
            return s->usart_fbrd;
        case USART_LCRH:
            return s->usart_lcrh;
        case USART_CTL:
            return s->usart_ctl;
        case USART_IFLS:
            return s->usart_ifls;
        case USART_IM:
            return s->usart_im;
        case USART_RIS:
            return s->usart_ris;
        case USART_MIS:
            return s->usart_mis;
        case USART_ICR:
            return s->usart_icr;
        case USART_DMA_CTL:
            return s->usart_dma_ctl;
        case USART_9BIT_ADDR:
            return s->usart_9bit_addr;
        case USART_9BIT_MASK:
            return s->usart_9bit_mask;
        case USART_PP:
            return s->usart_pp;
        case USART_CC:
            return s->usart_cc;
        case USART_PER_ID4:
            return s->usart_per_id4;
        case USART_PER_ID5:
            return s->usart_per_id5;
        case USART_PER_ID6:
            return s->usart_per_id6;
        case USART_PER_ID7:
            return s->usart_per_id7;
        case USART_PER_ID0:
            return s->usart_per_id0;
        case USART_PER_ID1:
            return s->usart_per_id1;
        case USART_PER_ID2:
            return s->usart_per_id2;
        case USART_PER_ID3:
            return s->usart_per_id3;
        case USART_PCELL_ID0:
            return s->usart_pcell_id0;
        case USART_PCELL_ID1:
            return s->usart_pcell_id1;
        case USART_PCELL_ID2:
            return s->usart_pcell_id2;
        case USART_PCELL_ID3:
            return s->usart_pcell_id3;
        default:
        qemu_log_mask(LOG_GUEST_ERROR,
                      "%s: Bad offset 0x%"HWADDR_PRIx"\n", __func__, addr);
        return 0;
    }

    return 0;
}

static void tm4c123_usart_write(void *opaque, hwaddr addr, uint64_t val64, unsigned int size)
{
    TM4C123USARTState *s = opaque;
    uint32_t val32 = val64;
    switch(addr) {
        case USART_DR:
            s->usart_dr = val32;
            /*
             * reset the flag register.
             */
            break;
        case USART_RSR:
            s->usart_rsr = val32;
            break;
        case USART_FR:
            s->usart_fr = val32;
            break;
        case USART_ILPR:
            s->usart_ilpr = val32;
            break;
        case USART_IBRD:
            s->usart_ibrd = val32;
            break;
        case USART_FBRD:
            s->usart_fbrd = val32;
            break;
        case USART_LCRH:
            s->usart_lcrh = val32;
            break;
        case USART_CTL:
            s->usart_ctl = val32;
            break;
        case USART_IFLS:
            s->usart_ifls = val32;
            break;
        case USART_IM:
            s->usart_im = val32;
            break;
        case USART_RIS:
            s->usart_ris = val32;
            break;
        case USART_MIS:
            s->usart_mis = val32;
            break;
        case USART_ICR:
            s->usart_icr = val32;
            break;
        case USART_DMA_CTL:
            s->usart_dma_ctl = val32;
            break;
        case USART_9BIT_ADDR:
            s->usart_9bit_addr = val32;
            break;
        case USART_9BIT_MASK:
            s->usart_9bit_mask = val32;
            break;
        case USART_PP:
            s->usart_pp = val32;
            break;
        case USART_CC:
            s->usart_cc = val32;
            break;
        case USART_PER_ID4:
            s->usart_per_id4 = val32;
            break;
        case USART_PER_ID5:
            s->usart_per_id5 = val32;
            break;
        case USART_PER_ID6:
            s->usart_per_id6 = val32;
            break;
        case USART_PER_ID7:
            s->usart_per_id7 = val32;
            break;
        case USART_PER_ID0:
            s->usart_per_id0 = val32;
            break;
        case USART_PER_ID1:
            s->usart_per_id1 = val32;
            break;
        case USART_PER_ID2:
            s->usart_per_id2 = val32;
            break;
        case USART_PER_ID3:
            s->usart_per_id3 = val32;
            break;
        case USART_PCELL_ID0:
            s->usart_pcell_id0 = val32;
            break;
        case USART_PCELL_ID1:
            s->usart_pcell_id1 = val32;
            break;
        case USART_PCELL_ID2:
            s->usart_pcell_id2 = val32;
            break;
        case USART_PCELL_ID3:
            s->usart_pcell_id3 = val32;
            break;
        default:
        qemu_log_mask(LOG_GUEST_ERROR,
                      "%s: Bad offset 0x%"HWADDR_PRIx"\n", __func__, addr);
        return;
    }

    return;
}

static const MemoryRegionOps tm4c123_usart_ops = {
    .read = tm4c123_usart_read,
    .write = tm4c123_usart_write,
    .endianness = DEVICE_NATIVE_ENDIAN,
};

static Property tm4c123_usart_properties[] = {
    DEFINE_PROP_CHR("chardev", TM4C123USARTState, chr),
    DEFINE_PROP_END_OF_LIST(),
};

static void tm4c123_usart_init(Object *obj)
{
    TM4C123USARTState *s = TM4C123_USART(obj);

    sysbus_init_irq(SYS_BUS_DEVICE(obj), &s->irq);

    memory_region_init_io(&s->mmio, obj, &tm4c123_usart_ops, s,
                          TYPE_TM4C123_USART, 0xFFF);
    sysbus_init_mmio(SYS_BUS_DEVICE(obj), &s->mmio);
}

static void tm4c123_usart_realize(DeviceState *dev, Error **errp)
{
    TM4C123USARTState *s = TM4C123_USART(dev);

    qemu_chr_fe_set_handlers(&s->chr, tm4c123_usart_can_receive,
                             tm4c123_usart_receive, NULL, NULL,
                             s, NULL, true);
}

static void tm4c123_usart_class_init(ObjectClass *klass, void *data)
{
    DeviceClass *dc = DEVICE_CLASS(klass);

    dc->reset = tm4c123_usart_reset;
    device_class_set_props(dc, tm4c123_usart_properties);
    dc->realize = tm4c123_usart_realize;
}

static const TypeInfo tm4c123_usart_info = {
    .name          = TYPE_TM4C123_USART,
    .parent        = TYPE_SYS_BUS_DEVICE,
    .instance_size = sizeof(TM4C123USARTState),
    .instance_init = tm4c123_usart_init,
    .class_init    = tm4c123_usart_class_init,
};

static void tm4c123_usart_register_types(void)
{
    type_register_static(&tm4c123_usart_info);
}

type_init(tm4c123_usart_register_types)
