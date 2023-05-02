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

#ifndef HW_ARM_TM4C123_GPIO_H
#define HW_ARM_TM4C123_GPIO_H

#include "hw/sysbus.h"
#include "qom/object.h"

/* #define GPIO_DATA 0x00 */
#define GPIO_DATA 0x3FC
#define GPIO_DIR 0x400
#define GPIO_IS 0x404
#define GPIO_IBE 0x408
#define GPIO_IEV 0x40C
#define GPIO_IM 0x410
#define GPIO_RIS 0x414
#define GPIO_MIS 0x418
#define GPIO_ICR 0x41C
#define GPIO_AFSEL 0x420
#define GPIO_DR2R 0x500
#define GPIO_DR4R 0x504
#define GPIO_DR8R 0x508
#define GPIO_ODR 0x50C
#define GPIO_PUR 0x510
#define GPIO_PDR 0x514
#define GPIO_SLR 0x518
#define GPIO_DEN 0x51C
#define GPIO_LOCK 0x520
#define GPIO_OCR 0x524
#define GPIO_AMSEL 0x528
#define GPIO_PCTL 0x52C
#define GPIO_ADCCTL 0x530
#define GPIO_DMACTL 0x534
#define GPIO_PER_ID4 0xFD0
#define GPIO_PER_ID5 0xFD4
#define GPIO_PER_ID6 0xFD8
#define GPIO_PER_ID7 0xFDC
#define GPIO_PER_ID0 0XFE0
#define GPIO_PER_ID1 0xFE4
#define GPIO_PER_ID2 0xFE8
#define GPIO_PER_ID3 0xFEC
#define GPIO_PCELL_ID0 0xFF0
#define GPIO_PCELL_ID1 0xFF4
#define GPIO_PCELL_ID2 0xFF8
#define GPIO_PCELL_ID3 0xFFC

#define TYPE_TM4C123_GPIO "tm4c123-gpio"

OBJECT_DECLARE_SIMPLE_TYPE(TM4C123GPIOState, TM4C123_GPIO)

struct TM4C123GPIOState {
    SysBusDevice parent_obj;
    MemoryRegion mmio;

    uint32_t gpio_data;
    uint32_t gpio_dir;
    uint32_t gpio_is;
    uint32_t gpio_ibe;
    uint32_t gpio_iev;
    uint32_t gpio_im;
    uint32_t gpio_ris;
    uint32_t gpio_mis;
    uint32_t gpio_icr;
    uint32_t gpio_afsel;
    uint32_t gpio_dr2r;
    uint32_t gpio_dr4r;
    uint32_t gpio_dr8r;
    uint32_t gpio_odr;
    uint32_t gpio_pur;
    uint32_t gpio_pdr;
    uint32_t gpio_slr;
    uint32_t gpio_den;
    uint32_t gpio_lock;
    uint32_t gpio_ocr;
    uint32_t gpio_amsel;
    uint32_t gpio_pctl;
    uint32_t gpio_adcctl;
    uint32_t gpio_dmactl;
    uint32_t gpio_per_id4;
    uint32_t gpio_per_id5;
    uint32_t gpio_per_id6;
    uint32_t gpio_per_id7;
    uint32_t gpio_per_id0;
    uint32_t gpio_per_id1;
    uint32_t gpio_per_id2;
    uint32_t gpio_per_id3;
    uint32_t gpio_pcell_id0;
    uint32_t gpio_pcell_id1;
    uint32_t gpio_pcell_id2;
    uint32_t gpio_pcell_id3;

    Clock* clock;
    qemu_irq irq;
};

#endif
