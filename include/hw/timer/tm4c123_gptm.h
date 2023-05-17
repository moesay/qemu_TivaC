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

#ifndef HW_ARM_TM4C123_GPTM_H
#define HW_ARM_TM4C123_GPTM_H

#include "qemu/osdep.h"
#include "qemu/log.h"
#include "qemu/module.h"
#include "hw/misc/tm4c123_sysctl.h"
#include "qemu/bitops.h"
#include "hw/sysbus.h"
#include "hw/irq.h"
#include "qom/object.h"

#define TIMER_WIDTH_32 0x32B
#define TIMER_WIDTH_64 0x64B

#define TIMER0_32 0x40030000
#define TIMER1_32 0x40031000
#define TIMER2_32 0x40032000
#define TIMER3_32 0x40033000
#define TIMER4_32 0x40034000
#define TIMER5_32 0x40035000

#define TIMER0_64 0x40036000
#define TIMER1_64 0x40037000
#define TIMER2_64 0x4003C000
#define TIMER3_64 0x4003D000
#define TIMER4_64 0x4003E000
#define TIMER5_64 0x4003F000

#define GPTM_CFG 0x000
#define GPTM_AMR 0x004
#define GPTM_BMR 0x008
#define GPTM_CTL 0x00C
#define GPTM_SYNC 0x010
#define GPTM_IMR 0x018
#define GPTM_RIS 0x01C
#define GPTM_MIS 0x020
#define GPTM_ICR 0x024
#define GPTM_TALIR 0x028
#define GPTM_TBLIR 0x02C
#define GPTM_TAMATCHR 0x030
#define GPTM_TBMATCHR 0x034
#define GPTM_TAPR 0x038
#define GPTM_TBPR 0x03C
#define GPTM_TAPMR 0x040
#define GPTM_TBPMR 0x044
#define GPTM_TAR 0x048
#define GPTM_TBR 0x04C
#define GPTM_TAV 0x050
#define GPTM_TBV 0x054
#define GPTM_RTCPD 0x058
#define GPTM_TAPS 0x05C
#define GPTM_TBPS 0x060
#define GPTM_TAPV 0x064
#define GPTM_TBPV 0x068
#define GPTM_PP 0xFC0

#define GPTM_TACTL_EN (1 << 0)
#define GPTM_TBCTL_EN (1 << 8)
#define GPTM_TAM_CD   (1 << 4)
#define GPTM_TAM_MODE_ONESHOT (1 << 1)
#define GPTM_TAM_PERIODIC (1 << 2)

#define TYPE_TM4C123_GPTM "tm4c123-gptm"

OBJECT_DECLARE_SIMPLE_TYPE(TM4C123GPTMState, TM4C123_GPTM)

struct TM4C123GPTMState {
    SysBusDevice parent_obj;
    MemoryRegion mmio;
    qemu_irq irq_a;
    qemu_irq irq_b;
    TM4C123SysCtlState *sysctl;

    uint32_t gptm_cfg;
    uint32_t gptm_amr;
    uint32_t gptm_bmr;
    uint32_t gptm_ctl;
    uint32_t gptm_sync;
    uint32_t gptm_imr;
    uint32_t gptm_ris;
    uint32_t gptm_mis;
    uint32_t gptm_icr;
    uint32_t gptm_talir;
    uint32_t gptm_tblir;
    uint32_t gptm_tamatchr;
    uint32_t gptm_tbmatchr;
    uint32_t gptm_tapr;
    uint32_t gptm_tbpr;
    uint32_t gptm_tapmr;
    uint32_t gptm_tbpmr;
    uint32_t gptm_tar;
    uint32_t gptm_tbr;
    uint32_t gptm_tav;
    uint32_t gptm_tbv;
    uint32_t gptm_rtcpd;
    uint32_t gptm_taps;
    uint32_t gptm_tbps;
    uint32_t gptm_tapv;
    uint32_t gptm_tbpv;
    uint32_t gptm_pp;
    QEMUTimer *a;
    QEMUTimer *b;
    Clock* clk;
};

#endif
