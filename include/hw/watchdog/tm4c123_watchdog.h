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

#ifndef HW_ARM_TM4C123_WATCHDOG_H
#define HW_ARM_TM4C123_WATCHDOG_H

#include "hw/sysbus.h"
#include "qom/object.h"
#include "hw/misc/tm4c123_sysctl.h"
#include "hw/ptimer.h"

#define WDT_0 0x40000000
#define WDT_1 0x40001000

#define WDT_LOAD 0x000
#define WDT_VALUE 0x004
#define WDT_CTL 0x008
#define WDT_ICR 0x00C
#define WDT_RIS 0x010
#define WDT_MIS 0x014
#define WDT_TEST 0x418
#define WDT_LOCK 0xC00
#define WDT_PER_ID4 0xFD0
#define WDT_PER_ID5 0xFD4
#define WDT_PER_ID6 0xFD8
#define WDT_PER_ID7 0xFDC
#define WDT_PER_ID0 0xFE0
#define WDT_PER_ID1 0xFE4
#define WDT_PER_ID2 0xFE8
#define WDT_PER_ID3 0xFEC
#define WDT_PCELL_ID0 0xFF0
#define WDT_PCELL_ID1 0xFF4
#define WDT_PCELL_ID2 0xFF8
#define WDT_PCELL_ID3 0xFFC

#define UNLOCK_VALUE 0x1ACCE551

#define WDT_CTL_INTEN (1 << 0)
#define WDT_CTL_INTTYPE (1 << 2)

#define TYPE_TM4C123_WATCHDOG "tm4c123-watchdog"

OBJECT_DECLARE_SIMPLE_TYPE(TM4C123WatchdogState, TM4C123_WATCHDOG)

struct TM4C123WatchdogState {
    SysBusDevice parent_obj;
    MemoryRegion mmio;
    qemu_irq irq;
    struct ptimer_state *timer;
    TM4C123SysCtlState* sysctl;

    uint32_t wdt_load;
    uint32_t wdt_value;
    uint32_t wdt_ctl;
    uint32_t wdt_icr;
    uint32_t wdt_ris;
    uint32_t wdt_mis;
    uint32_t wdt_test;
    uint32_t wdt_lock;
    uint32_t wdt_per_id4;
    uint32_t wdt_per_id5;
    uint32_t wdt_per_id6;
    uint32_t wdt_per_id7;
    uint32_t wdt_per_id0;
    uint32_t wdt_per_id1;
    uint32_t wdt_per_id2;
    uint32_t wdt_per_id3;
    uint32_t wdt_pcell_id0;
    uint32_t wdt_pcell_id1;
    uint32_t wdt_pcell_id2;
    uint32_t wdt_pcell_id3;

    Clock* wdt_clock;
};

#endif
