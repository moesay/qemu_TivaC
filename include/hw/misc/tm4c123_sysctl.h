/*
 * TM4C123 SYSCTL
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

#ifndef HW_ARM_TM4C123_SYSCTL_H
#define HW_ARM_TM4C123_SYSCTL_H

#include "qom/object.h"
#include "hw/sysbus.h"
#include "hw/hw.h"

#define SYSCTL_DID0 0x000
#define SYSCTL_DID1 0x004
#define SYSCTL_PBORCTL 0x030
#define SYSCTL_RIS 0x050
#define SYSCTL_IMC 0x054
#define SYSCTL_MISC 0x058
#define SYSCTL_RESC 0x05C
#define SYSCTL_RCC 0x060
#define SYSCTL_GPIOHBCTL 0x06C
#define SYSCTL_RCC2 0x070
#define SYSCTL_MOSCCTL 0x07C
#define SYSCTL_DSLPCLKCFG 0x144
#define SYSCTL_SYSPROP 0x14C
#define SYSCTL_PIOSCCAL 0x150
#define SYSCTL_PIOSCSTAT 0x154
#define SYSCTL_PLLFREQ0 0x160
#define SYSCTL_PLLFREQ1 0x164
#define SYSCTL_PLLSTAT 0x168
#define SYSCTL_SLPPWRCFG 0x188
#define SYSCTL_DSLPPWRCFG 0x18C
#define SYSCTL_LDOSPCTL 0x1B4
#define SYSCTL_LDOSPCAL 0x1B8
#define SYSCTL_LDODPCTL 0x1Bc
#define SYSCTL_LDODPCAL 0x1C0
#define SYSCTL_SDPMST 0x1CC
#define SYSCTL_PPWD 0x300
#define SYSCTL_PPTIMER 0x304
#define SYSCTL_PPGPIO 0x308
#define SYSCTL_PPDMA 0x30C
#define SYSCTL_PPHIB 0x314
#define SYSCTL_PPUART 0x318
#define SYSCTL_PPSI 0x31C
#define SYSCTL_PPI2C 0x320
#define SYSCTL_PPUSB 0x328
#define SYSCTL_PPCAN 0x334
#define SYSCTL_PPADC 0x338
#define SYSCTL_PPACMP 0x33C
#define SYSCTL_PPPWM 0x340
#define SYSCTL_PPQEI 0x344
#define SYSCTL_PPEEPROM 0x358
#define SYSCTL_PPWTIMER 0x35C
#define SYSCTL_SRWD 0x500
#define SYSCTL_SRTIMER 0x504
#define SYSCTL_SRGPIO 0x508
#define SYSCTL_SRDMA 0x50C
#define SYSCTL_SRHIB 0x514
#define SYSCTL_SRUART 0x518
#define SYSCTL_SRSSI 0x51C
#define SYSCTL_SRI2C 0x520
#define SYSCTL_SRUSB 0x528
#define SYSCTL_SRCAN 0x534
#define SYSCTL_SRADC 0x538
#define SYSCTL_SRACMP 0x53C
#define SYSCTL_SRPWM 0x540
#define SYSCTL_SRQEI 0x544
#define SYSCTL_SREEPROM 0x558
#define SYSCTL_SRWTIMER 0x55C
#define SYSCTL_RCGCWD 0x600
#define SYSCTL_RCGCTIMER 0x604
#define SYSCTL_RCGCGPIO 0x608
#define SYSCTL_RCGCDMA 0x60C
#define SYSCTL_RCGCHIB 0x614
#define SYSCTL_RCGCUART 0x618
#define SYSCTL_RCGCSSI 0x61C
#define SYSCTL_RCGCI2C 0x620
#define SYSCTL_RCGCUSB 0x628
#define SYSCTL_RCGCCAN 0x634
#define SYSCTL_RCGCADC 0x638
#define SYSCTL_RCGCACMP 0x63C
#define SYSCTL_RCGCPWM 0x640
#define SYSCTL_RCGCQEI 0x644
#define SYSCTL_RCGCEEPROM 0x658
#define SYSCTL_RCGCWTIMER 0x65C
#define SYSCTL_SCGCWD 0x700
#define SYSCTL_SCGCTIMER 0x704
#define SYSCTL_SCGCGPIO 0x708
#define SYSCTL_SCGCDMA 0x70C
#define SYSCTL_SCGCHIB 0x714
#define SYSCTL_SCGCUART 0x718
#define SYSCTL_SCGCSSI 0x71C
#define SYSCTL_SCGCI2C 0x720
#define SYSCTL_SCGCUSB 0x728
#define SYSCTL_SCGCCAN 0x734
#define SYSCTL_SCGCADC 0x738
#define SYSCTL_SCGCACMP 0x73C
#define SYSCTL_SCGCPWM 0x740
#define SYSCTL_SCGCQEI 0x744
#define SYSCTL_SCGCEEPROM 0x758
#define SYSCTL_SCGCWTIMER 0x75C
#define SYSCTL_DCGCWD 0x800
#define SYSCTL_DCGCTIMER 0x804
#define SYSCTL_DCGCGPIO 0x808
#define SYSCTL_DCGCDMA 0x80C
#define SYSCTL_DCGCHIB 0x814
#define SYSCTL_DCGCUART 0x818
#define SYSCTL_DCGCSSI 0x81C
#define SYSCTL_DCGCI2C 0x820
#define SYSCTL_DCGCUSB 0x828
#define SYSCTL_DCGCCAN 0x834
#define SYSCTL_DCGCADC 0x838
#define SYSCTL_DCGCACMP 0x83C
#define SYSCTL_DCGCPWM 0x840
#define SYSCTL_DCGCQEI 0x844
#define SYSCTL_DCGCEEPROM 0x858
#define SYSCTL_DCGCWTIME 0x85C
#define SYSCTL_PRWD 0xA00
#define SYSCTL_PRTIMER 0xA04
#define SYSCTL_PRGPIO 0xA08
#define SYSCTL_PRDMA 0xA0C
#define SYSCTL_PRHIB 0xA14
#define SYSCTL_PRUART 0xA18
#define SYSCTL_PRSSI 0xA1C
#define SYSCTL_PRI2C 0xA20
#define SYSCTL_PRUSB 0xA28
#define SYSCTL_PRCAN 0xA34
#define SYSCTL_PRADC 0xA38
#define SYSCTL_PRACMP 0xA3C
#define SYSCTL_PRPWM 0xA40
#define SYSCTL_PRQEI 0xA44
#define SYSCTL_PREEPROM 0xA58
#define SYSCTL_PRWTIMER 0xA5C


#define SYSCTL_RCC_PWRDN (1 << 13)
#define SYSCTL_RCC2_PWRDN2 (1 << 13)
#define SYSCTL_RCC2_USERCC2 (1 << 31)
#define SYSCTL_RIS_PLLRIS (1 << 6)

#define TYPE_TM4C123_SYSCTL "tm4c123-sysctl"
OBJECT_DECLARE_SIMPLE_TYPE(TM4C123SysCtlState, TM4C123_SYSCTL)

struct TM4C123SysCtlState {

    SysBusDevice parent_obj;
    MemoryRegion mmio;

    uint32_t sysctl_did0;
    uint32_t sysctl_did1;
    uint32_t sysctl_pborctl;
    uint32_t sysctl_ris;
    uint32_t sysctl_imc;
    uint32_t sysctl_misc;
    uint32_t sysctl_resc;
    uint32_t sysctl_rcc;
    uint32_t sysctl_gpiohbctl;
    uint32_t sysctl_rcc2;
    uint32_t sysctl_moscctl;
    uint32_t sysctl_dslpclkcfg;
    uint32_t sysctl_sysprop;
    uint32_t sysctl_piosccal;
    uint32_t sysctl_pioscstat;
    uint32_t sysctl_pllfreq0;
    uint32_t sysctl_pllfreq1;
    uint32_t sysctl_pllstat;
    uint32_t sysctl_slppwrcfg;
    uint32_t sysctl_dslppwrcfg;
    uint32_t sysctl_ldospctl;
    uint32_t sysctl_ldospcal;
    uint32_t sysctl_ldodpctl;
    uint32_t sysctl_ldodpcal;
    uint32_t sysctl_sdpmst;
    uint32_t sysctl_ppwd;
    uint32_t sysctl_pptimer;
    uint32_t sysctl_ppgpio;
    uint32_t sysctl_ppdma;
    uint32_t sysctl_pphib;
    uint32_t sysctl_ppuart;
    uint32_t sysctl_ppsi;
    uint32_t sysctl_ppi2c;
    uint32_t sysctl_ppusb;
    uint32_t sysctl_ppcan;
    uint32_t sysctl_ppadc;
    uint32_t sysctl_ppacmp;
    uint32_t sysctl_pppwm;
    uint32_t sysctl_ppqei;
    uint32_t sysctl_ppeeprom;
    uint32_t sysctl_ppwtimer;
    uint32_t sysctl_srwd;
    uint32_t sysctl_srtimer;
    uint32_t sysctl_srgpio;
    uint32_t sysctl_srdma;
    uint32_t sysctl_srhib;
    uint32_t sysctl_sruart;
    uint32_t sysctl_srssi;
    uint32_t sysctl_sri2c;
    uint32_t sysctl_srusb;
    uint32_t sysctl_srcan;
    uint32_t sysctl_sradc;
    uint32_t sysctl_sracmp;
    uint32_t sysctl_srpwm;
    uint32_t sysctl_srqei;
    uint32_t sysctl_sreeprom;
    uint32_t sysctl_srwtimer;
    uint32_t sysctl_rcgcwd;
    uint32_t sysctl_rcgctimer;
    uint32_t sysctl_rcgcgpio;
    uint32_t sysctl_rcgcdma;
    uint32_t sysctl_rcgchib;
    uint32_t sysctl_rcgcuart;
    uint32_t sysctl_rcgcssi;
    uint32_t sysctl_rcgci2c;
    uint32_t sysctl_rcgcusb;
    uint32_t sysctl_rcgccan;
    uint32_t sysctl_rcgcadc;
    uint32_t sysctl_rcgcacmp;
    uint32_t sysctl_rcgcpwm;
    uint32_t sysctl_rcgcqei;
    uint32_t sysctl_rcgceeprom;
    uint32_t sysctl_rcgcwtimer;
    uint32_t sysctl_scgcwd;
    uint32_t sysctl_scgctimer;
    uint32_t sysctl_scgcgpio;
    uint32_t sysctl_scgcdma;
    uint32_t sysctl_scgchib;
    uint32_t sysctl_scgcuart;
    uint32_t sysctl_scgcssi;
    uint32_t sysctl_scgci2c;
    uint32_t sysctl_scgcusb;
    uint32_t sysctl_scgccan;
    uint32_t sysctl_scgcadc;
    uint32_t sysctl_scgcacmp;
    uint32_t sysctl_scgcpwm;
    uint32_t sysctl_scgcqei;
    uint32_t sysctl_scgceeprom;
    uint32_t sysctl_scgcwtimer;
    uint32_t sysctl_dcgcwd;
    uint32_t sysctl_dcgctimer;
    uint32_t sysctl_dcgcgpio;
    uint32_t sysctl_dcgcdma;
    uint32_t sysctl_dcgchib;
    uint32_t sysctl_dcgcuart;
    uint32_t sysctl_dcgcssi;
    uint32_t sysctl_dcgci2c;
    uint32_t sysctl_dcgcusb;
    uint32_t sysctl_dcgccan;
    uint32_t sysctl_dcgcadc;
    uint32_t sysctl_dcgcacmp;
    uint32_t sysctl_dcgcpwm;
    uint32_t sysctl_dcgcqei;
    uint32_t sysctl_dcgceeprom;
    uint32_t sysctl_dcgcwtime;
    uint32_t sysctl_prwd;
    uint32_t sysctl_prtimer;
    uint32_t sysctl_prgpio;
    uint32_t sysctl_prdma;
    uint32_t sysctl_prhib;
    uint32_t sysctl_pruart;
    uint32_t sysctl_prssi;
    uint32_t sysctl_pri2c;
    uint32_t sysctl_prusb;
    uint32_t sysctl_prcan;
    uint32_t sysctl_pradc;
    uint32_t sysctl_pracmp;
    uint32_t sysctl_prpwm;
    uint32_t sysctl_prqei;
    uint32_t sysctl_preeprom;
    uint32_t sysctl_prwtimer;

};

#endif
