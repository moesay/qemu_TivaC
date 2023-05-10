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

#ifndef HW_ARM_TM4C123_USART_H
#define HW_ARM_TM4C123_USART_H

#include "hw/sysbus.h"
#include "qom/object.h"
#include "chardev/char-fe.h"
#include "hw/misc/tm4c123_sysctl.h"

#define USART_DR            0x000
#define USART_RSR           0x004
#define USART_FR            0x018
#define USART_ILPR          0x020
#define USART_IBRD          0x024
#define USART_FBRD          0x028
#define USART_LCRH          0x02C
#define USART_CTL           0x030
#define USART_IFLS          0x034
#define USART_IM            0x038
#define USART_RIS           0x03C
#define USART_MIS           0x040
#define USART_ICR           0x044
#define USART_DMA_CTL       0x048
#define USART_9BIT_ADDR     0x0A4
#define USART_9BIT_MASK     0x0A8
#define USART_PP            0xFC0
#define USART_CC            0xFC8
#define USART_PER_ID4       0x0FD0
#define USART_PER_ID5       0xFD4
#define USART_PER_ID6       0xFD8
#define USART_PER_ID7       0xFDC
#define USART_PER_ID0       0xFE0
#define USART_PER_ID1       0xFE4
#define USART_PER_ID2       0xFE8
#define USART_PER_ID3       0xFEC
#define USART_PCELL_ID0     0xFF0
#define USART_PCELL_ID1     0xFF4
#define USART_PCELL_ID2     0xFF8
#define USART_PCELL_ID3     0xFFC

#define USART_FR_RXFF (1 << 6)
#define USART_FR_TXFF (1 << 5)
#define USART_FR_RXFE (1 << 4)
#define USART_FR_BUSY (1 << 3)
#define USART_CR_RXE  (1 << 9)
#define USART_CR_EN   (1 << 0)
#define USART_IM_RXIM (1 << 4)

#define USART_0 0x4000C000
#define USART_1 0x4000D000
#define USART_2 0x4000E000
#define USART_3 0x4000F000
#define USART_4 0x40010000
#define USART_5 0x40011000
#define USART_6 0x40012000
#define USART_7 0x40013000
#define TYPE_TM4C123_USART "tm4c123-usart"

OBJECT_DECLARE_SIMPLE_TYPE(TM4C123USARTState, TM4C123_USART)

struct TM4C123USARTState {
    SysBusDevice parent_obj;
    MemoryRegion mmio;

    uint32_t usart_dr;
    uint32_t usart_rsr;
    uint32_t usart_fr;
    uint32_t usart_ilpr;
    uint32_t usart_ibrd;
    uint32_t usart_fbrd;
    uint32_t usart_lcrh;
    uint32_t usart_ctl;
    uint32_t usart_ifls;
    uint32_t usart_im;
    uint32_t usart_ris;
    uint32_t usart_mis;
    uint32_t usart_icr;
    uint32_t usart_dma_ctl;
    uint32_t usart_9bit_addr;
    uint32_t usart_9bit_mask;
    uint32_t usart_pp;
    uint32_t usart_cc;
    uint32_t usart_per_id4;
    uint32_t usart_per_id5;
    uint32_t usart_per_id6;
    uint32_t usart_per_id7;
    uint32_t usart_per_id0;
    uint32_t usart_per_id1;
    uint32_t usart_per_id2;
    uint32_t usart_per_id3;
    uint32_t usart_pcell_id0;
    uint32_t usart_pcell_id1;
    uint32_t usart_pcell_id2;
    uint32_t usart_pcell_id3;

    CharBackend chr;
    qemu_irq irq;
    TM4C123SysCtlState *sysctl;
};

#endif
