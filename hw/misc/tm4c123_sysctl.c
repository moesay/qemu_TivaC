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

#include "qemu/osdep.h"
#include "hw/misc/tm4c123_sysctl.h"
#include "qemu/log.h"
#include "qemu/module.h"
#include "trace.h"

#define LOG(fmt, args...) qemu_log("%s: " fmt, __func__, ## args)
#define READONLY LOG("0x%"HWADDR_PRIx" is a readonly field\n.", addr)

static void tm4c123_sysctl_reset(DeviceState *dev)
{
    TM4C123SysCtlState *s = TM4C123_SYSCTL(dev);

    s->sysctl_did0 = 0x00000000;
    s->sysctl_did1 = 0x10A1606E;
    s->sysctl_pborctl = 0x00000000;
    s->sysctl_ris = 0x00000000;
    s->sysctl_imc = 0x00000000;
    s->sysctl_misc = 0x00000000;
    s->sysctl_resc = 0x00000000;
    s->sysctl_rcc = 0x078E3AD1;
    s->sysctl_gpiohbctl = 0x00007E00;
    s->sysctl_rcc2 = 0x07C06810;
    s->sysctl_moscctl = 0x00000000;
    s->sysctl_dslpclkcfg = 0x07800000;
    s->sysctl_sysprop = 0x00001D31;
    s->sysctl_piosccal = 0x00000000;
    s->sysctl_pioscstat = 0x00000040;
    s->sysctl_pllfreq0 = 0x00000032;
    s->sysctl_pllfreq1 = 0x00000001;
    s->sysctl_pllstat = 0x00000000;
    s->sysctl_slppwrcfg = 0x00000000;
    s->sysctl_dslppwrcfg = 0x00000000;
    s->sysctl_ldospctl = 0x00000018;
    s->sysctl_ldospcal = 0x00001818;
    s->sysctl_ldodpctl = 0x00000012;
    s->sysctl_ldodpcal = 0x00001212;
    s->sysctl_sdpmst = 0x00000000;
    s->sysctl_ppwd = 0x00000003;
    s->sysctl_pptimer = 0x0000003F;
    s->sysctl_ppgpio = 0x0000003F;
    s->sysctl_ppdma = 0x00000001;
    s->sysctl_pphib = 0x00000001;
    s->sysctl_ppuart = 0x000000FF;
    s->sysctl_ppsi = 0x0000000F;
    s->sysctl_ppi2c = 0x0000000F;
    s->sysctl_ppusb = 0x00000001;
    s->sysctl_ppcan = 0x00000003;
    s->sysctl_ppadc = 0x00000003;
    s->sysctl_ppacmp = 0x00000001;
    s->sysctl_pppwm = 0x00000003;
    s->sysctl_ppqei = 0x00000003;
    s->sysctl_ppeeprom = 0x00000001;
    s->sysctl_ppwtimer = 0x0000003F;
    s->sysctl_srwd = 0x00000000;
    s->sysctl_srtimer = 0x00000000;
    s->sysctl_srgpio = 0x00000000;
    s->sysctl_srdma = 0x00000000;
    s->sysctl_srhib = 0x00000000;
    s->sysctl_sruart = 0x00000000;
    s->sysctl_srssi = 0x00000000;
    s->sysctl_sri2c = 0x00000000;
    s->sysctl_srusb = 0x00000000;
    s->sysctl_srcan = 0x00000000;
    s->sysctl_sradc = 0x00000000;
    s->sysctl_sracmp = 0x00000000;
    s->sysctl_srpwm = 0x00000000;
    s->sysctl_srqei = 0x00000000;
    s->sysctl_sreeprom = 0x00000000;
    s->sysctl_srwtimer = 0x00000000;
    s->sysctl_rcgcwd = 0x00000000;
    s->sysctl_rcgctimer = 0x00000000;
    s->sysctl_rcgcgpio = 0x00000000;
    s->sysctl_rcgcdma = 0x00000000;
    s->sysctl_rcgchib = 0x00000001;
    s->sysctl_rcgcuart = 0x00000000;
    s->sysctl_rcgcssi = 0x00000000;
    s->sysctl_rcgci2c = 0x00000000;
    s->sysctl_rcgcusb = 0x00000000;
    s->sysctl_rcgccan = 0x00000000;
    s->sysctl_rcgcadc = 0x00000000;
    s->sysctl_rcgcacmp = 0x00000000;
    s->sysctl_rcgcpwm = 0x00000000;
    s->sysctl_rcgcqei = 0x00000000;
    s->sysctl_rcgceeprom = 0x00000000;
    s->sysctl_rcgcwtimer = 0x00000000;
    s->sysctl_scgcwd = 0x00000000;
    s->sysctl_scgctimer = 0x00000000;
    s->sysctl_scgcgpio = 0x00000000;
    s->sysctl_scgcdma = 0x00000000;
    s->sysctl_scgchib = 0x00000001;
    s->sysctl_scgcuart = 0x00000000;
    s->sysctl_scgcssi = 0x00000000;
    s->sysctl_scgci2c = 0x00000000;
    s->sysctl_scgcusb = 0x00000000;
    s->sysctl_scgccan = 0x00000000;
    s->sysctl_scgcadc = 0x00000000;
    s->sysctl_scgcacmp = 0x00000000;
    s->sysctl_scgcpwm = 0x00000000;
    s->sysctl_scgcqei = 0x00000000;
    s->sysctl_scgceeprom = 0x00000000;
    s->sysctl_scgcwtimer = 0x00000000;
    s->sysctl_dcgcwd = 0x00000000;
    s->sysctl_dcgctimer = 0x00000000;
    s->sysctl_dcgcgpio = 0x00000000;
    s->sysctl_dcgcdma = 0x00000000;
    s->sysctl_dcgchib = 0x00000001;
    s->sysctl_dcgcuart = 0x00000000;
    s->sysctl_dcgcssi = 0x00000000;
    s->sysctl_dcgci2c = 0x00000000;
    s->sysctl_dcgcusb = 0x00000000;
    s->sysctl_dcgccan = 0x00000000;
    s->sysctl_dcgcadc = 0x00000000;
    s->sysctl_dcgcacmp = 0x00000000;
    s->sysctl_dcgcpwm = 0x00000000;
    s->sysctl_dcgcqei = 0x00000000;
    s->sysctl_dcgceeprom = 0x00000000;
    s->sysctl_dcgcwtime = 0x00000000;
    s->sysctl_prwd = 0x00000000;
    s->sysctl_prtimer = 0x00000000;
    s->sysctl_prgpio = 0x00000000;
    s->sysctl_prdma = 0x00000000;
    s->sysctl_prhib = 0x00000001;
    s->sysctl_pruart = 0x00000000;
    s->sysctl_prssi = 0x00000000;
    s->sysctl_pri2c = 0x00000000;
    s->sysctl_prusb = 0x00000000;
    s->sysctl_prcan = 0x00000000;
    s->sysctl_pradc = 0x00000000;
    s->sysctl_pracmp = 0x00000000;
    s->sysctl_prpwm = 0x00000000;
    s->sysctl_prqei = 0x00000000;
    s->sysctl_preeprom = 0x00000000;
    s->sysctl_prwtimer = 0x00000000;
}

static void tm4c123_sysctl_write(void *opaque, hwaddr addr, uint64_t val64, unsigned int size)
{
    TM4C123SysCtlState *s = opaque;
    uint32_t val32 = val64;

    trace_tm4c123_sysctl_write(addr, val32);

    switch(addr) {
        case SYSCTL_DID0:
            READONLY;
            break;
        case SYSCTL_DID1:
            READONLY;
            break;
        case SYSCTL_PBORCTL:
            s->sysctl_pborctl = val32;
            break;
        case SYSCTL_RIS:
            READONLY;
            break;
        case SYSCTL_IMC:
            s->sysctl_imc = val32;
            /*
             * setting the MISC
             */
            s->sysctl_misc = val32;
            break;
        case SYSCTL_MISC:
            s->sysctl_misc = val32;
            break;
        case SYSCTL_RESC:
            s->sysctl_resc = val32;
            break;
        case SYSCTL_RCC:
            s->sysctl_rcc = val32;
            /*
             * Setting the SYSCTL_RIS manually for now.
             */
            if(s->sysctl_rcc & SYSCTL_RCC_PWRDN && !(s->sysctl_rcc2 & SYSCTL_RCC2_USERCC2)) {
                s->sysctl_ris |= SYSCTL_RIS_PLLRIS;
            }
            break;
        case SYSCTL_GPIOHBCTL:
            s->sysctl_gpiohbctl = val32;
            break;
        case SYSCTL_RCC2:
            s->sysctl_rcc2 = val32;
            /*
             * Setting the SYSCTL_RIS manually for now.
             */
            if(s->sysctl_rcc2 & SYSCTL_RCC2_USERCC2 && !(s->sysctl_rcc2 & SYSCTL_RCC2_PWRDN2)) {
                s->sysctl_ris |= SYSCTL_RIS_PLLRIS;
            }
            break;
        case SYSCTL_MOSCCTL:
            s->sysctl_moscctl = val32;
            break;
        case SYSCTL_DSLPCLKCFG:
            s->sysctl_dslpclkcfg = val32;
            break;
        case SYSCTL_SYSPROP:
            READONLY;
            break;
        case SYSCTL_PIOSCCAL:
            s->sysctl_piosccal = val32;
            break;
        case SYSCTL_PIOSCSTAT:
            READONLY;
            break;
        case SYSCTL_PLLFREQ0:
            READONLY;
            break;
        case SYSCTL_PLLFREQ1:
            READONLY;
            break;
        case SYSCTL_PLLSTAT:
            READONLY;
            break;
        case SYSCTL_SLPPWRCFG:
            s->sysctl_slppwrcfg = val32;
            break;
        case SYSCTL_DSLPPWRCFG:
            s->sysctl_dslppwrcfg = val32;
            break;
        case SYSCTL_LDOSPCTL:
            s->sysctl_ldospctl = val32;
            break;
        case SYSCTL_LDOSPCAL:
            READONLY;
            break;
        case SYSCTL_LDODPCTL:
            s->sysctl_ldodpctl = val32;
            break;
        case SYSCTL_LDODPCAL:
            READONLY;
            break;
        case SYSCTL_SDPMST:
            s->sysctl_sdpmst = val32;
            break;
        case SYSCTL_PPWD:
            READONLY;
            break;
        case SYSCTL_PPTIMER:
            READONLY;
            break;
        case SYSCTL_PPGPIO:
            READONLY;
            break;
        case SYSCTL_PPDMA:
            READONLY;
            break;
        case SYSCTL_PPHIB:
            READONLY;
            break;
        case SYSCTL_PPUART:
            READONLY;
            break;
        case SYSCTL_PPSI:
            READONLY;
            break;
        case SYSCTL_PPI2C:
            READONLY;
            break;
        case SYSCTL_PPUSB:
            READONLY;
            break;
        case SYSCTL_PPCAN:
            READONLY;
            break;
        case SYSCTL_PPADC:
            READONLY;
            break;
        case SYSCTL_PPACMP:
            READONLY;
            break;
        case SYSCTL_PPPWM:
            READONLY;
            break;
        case SYSCTL_PPQEI:
            READONLY;
            break;
        case SYSCTL_PPEEPROM:
            READONLY;
            break;
        case SYSCTL_PPWTIMER:
            READONLY;
            break;
        case SYSCTL_SRWD:
            s->sysctl_srwd = val32;
            break;
        case SYSCTL_SRTIMER:
            s->sysctl_srtimer = val32;
            break;
        case SYSCTL_SRGPIO:
            s->sysctl_srgpio = val32;
            break;
        case SYSCTL_SRDMA:
            s->sysctl_srdma = val32;
            break;
        case SYSCTL_SRHIB:
            s->sysctl_srhib = val32;
            break;
        case SYSCTL_SRUART:
            s->sysctl_sruart = val32;
            break;
        case SYSCTL_SRSSI:
            s->sysctl_srssi = val32;
            break;
        case SYSCTL_SRI2C:
            s->sysctl_sri2c = val32;
            break;
        case SYSCTL_SRUSB:
            s->sysctl_srusb = val32;
            break;
        case SYSCTL_SRCAN:
            s->sysctl_srcan = val32;
            break;
        case SYSCTL_SRADC:
            s->sysctl_sradc = val32;
            break;
        case SYSCTL_SRACMP:
            s->sysctl_sracmp = val32;
            break;
        case SYSCTL_SRPWM:
            s->sysctl_srpwm = val32;
            break;
        case SYSCTL_SRQEI:
            s->sysctl_srqei = val32;
            break;
        case SYSCTL_SREEPROM:
            s->sysctl_sreeprom = val32;
            break;
        case SYSCTL_SRWTIMER:
            s->sysctl_srwtimer = val32;
            break;
        case SYSCTL_RCGCWD:
            s->sysctl_rcgcwd = val32;
            break;
        case SYSCTL_RCGCTIMER:
            s->sysctl_rcgctimer = val32;
            break;
        case SYSCTL_RCGCGPIO:
            s->sysctl_rcgcgpio = val32;
            s->sysctl_prgpio = val32;
            break;
        case SYSCTL_RCGCDMA:
            s->sysctl_rcgcdma = val32;
            s->sysctl_prdma = val32;
            break;
        case SYSCTL_RCGCHIB:
            s->sysctl_rcgchib = val32;
            s->sysctl_prhib = val32;
            break;
        case SYSCTL_RCGCUART:
            s->sysctl_rcgcuart = val32;
            s->sysctl_pruart = val32;
            break;
        case SYSCTL_RCGCSSI:
            s->sysctl_rcgcssi = val32;
            s->sysctl_prssi = val32;
            break;
        case SYSCTL_RCGCI2C:
            s->sysctl_rcgci2c = val32;
            s->sysctl_pri2c = val32;
            break;
        case SYSCTL_RCGCUSB:
            s->sysctl_rcgcusb = val32;
            s->sysctl_prusb = val32;
            break;
        case SYSCTL_RCGCCAN:
            s->sysctl_rcgccan = val32;
            s->sysctl_prcan = val32;
            break;
        case SYSCTL_RCGCADC:
            s->sysctl_rcgcadc = val32;
            s->sysctl_pradc = val32;
            break;
        case SYSCTL_RCGCACMP:
            s->sysctl_rcgcacmp = val32;
            s->sysctl_pracmp = val32;
            break;
        case SYSCTL_RCGCPWM:
            s->sysctl_rcgcpwm = val32;
            s->sysctl_prpwm = val32;
            break;
        case SYSCTL_RCGCQEI:
            s->sysctl_rcgcqei = val32;
            s->sysctl_prqei = val32;
            break;
        case SYSCTL_RCGCEEPROM:
            s->sysctl_rcgceeprom = val32;
            s->sysctl_preeprom = val32;
            break;
        case SYSCTL_RCGCWTIMER:
            s->sysctl_rcgcwtimer = val32;
            s->sysctl_prwtimer = val32;
            break;
        case SYSCTL_SCGCWD:
            s->sysctl_scgcwd = val32;
            break;
        case SYSCTL_SCGCTIMER:
            s->sysctl_scgctimer = val32;
            break;
        case SYSCTL_SCGCGPIO:
            s->sysctl_scgcgpio = val32;
            break;
        case SYSCTL_SCGCDMA:
            s->sysctl_scgcdma = val32;
            break;
        case SYSCTL_SCGCHIB:
            s->sysctl_scgchib = val32;
            break;
        case SYSCTL_SCGCUART:
            s->sysctl_scgcuart = val32;
            break;
        case SYSCTL_SCGCSSI:
            s->sysctl_scgcssi = val32;
            break;
        case SYSCTL_SCGCI2C:
            s->sysctl_scgci2c = val32;
            break;
        case SYSCTL_SCGCUSB:
            s->sysctl_scgcusb = val32;
            break;
        case SYSCTL_SCGCCAN:
            s->sysctl_scgccan = val32;
            break;
        case SYSCTL_SCGCADC:
            s->sysctl_scgcadc = val32;
            break;
        case SYSCTL_SCGCACMP:
            s->sysctl_scgcacmp = val32;
            break;
        case SYSCTL_SCGCPWM:
            s->sysctl_scgcpwm = val32;
            break;
        case SYSCTL_SCGCQEI:
            s->sysctl_scgcqei = val32;
            break;
        case SYSCTL_SCGCEEPROM:
            s->sysctl_scgceeprom = val32;
            break;
        case SYSCTL_SCGCWTIMER:
            s->sysctl_scgcwtimer = val32;
            break;
        case SYSCTL_DCGCWD:
            s->sysctl_dcgcwd = val32;
            break;
        case SYSCTL_DCGCTIMER:
            s->sysctl_dcgctimer = val32;
            break;
        case SYSCTL_DCGCGPIO:
            s->sysctl_dcgcgpio = val32;
            break;
        case SYSCTL_DCGCDMA:
            s->sysctl_dcgcdma = val32;
            break;
        case SYSCTL_DCGCHIB:
            s->sysctl_dcgchib = val32;
            break;
        case SYSCTL_DCGCUART:
            s->sysctl_dcgcuart = val32;
            break;
        case SYSCTL_DCGCSSI:
            s->sysctl_dcgcssi = val32;
            break;
        case SYSCTL_DCGCI2C:
            s->sysctl_dcgci2c = val32;
            break;
        case SYSCTL_DCGCUSB:
            s->sysctl_dcgcusb = val32;
            break;
        case SYSCTL_DCGCCAN:
            s->sysctl_dcgccan = val32;
            break;
        case SYSCTL_DCGCADC:
            s->sysctl_dcgcadc = val32;
            break;
        case SYSCTL_DCGCACMP:
            s->sysctl_dcgcacmp = val32;
            break;
        case SYSCTL_DCGCPWM:
            s->sysctl_dcgcpwm = val32;
            break;
        case SYSCTL_DCGCQEI:
            s->sysctl_dcgcqei = val32;
            break;
        case SYSCTL_DCGCEEPROM:
            s->sysctl_dcgceeprom = val32;
            break;
        case SYSCTL_DCGCWTIME:
            s->sysctl_dcgcwtime = val32;
            break;
        case SYSCTL_PRWD:
            READONLY;
            break;
        case SYSCTL_PRTIMER:
            READONLY;
            break;
        case SYSCTL_PRGPIO:
            READONLY;
            break;
        case SYSCTL_PRDMA:
            READONLY;
            break;
        case SYSCTL_PRHIB:
            READONLY;
            break;
        case SYSCTL_PRUART:
            READONLY;
            break;
        case SYSCTL_PRSSI:
            READONLY;
            break;
        case SYSCTL_PRI2C:
            READONLY;
            break;
        case SYSCTL_PRUSB:
            READONLY;
            break;
        case SYSCTL_PRCAN:
            READONLY;
            break;
        case SYSCTL_PRADC:
            READONLY;
            break;
        case SYSCTL_PRACMP:
            READONLY;
            break;
        case SYSCTL_PRPWM:
            READONLY;
            break;
        case SYSCTL_PRQEI:
            READONLY;
            break;
        case SYSCTL_PREEPROM:
            READONLY;
            break;
        case SYSCTL_PRWTIMER:
            READONLY;
            break;
        default:
            LOG("Bad address 0x%"HWADDR_PRIx"\n", addr);
            break;
    }
}

static uint64_t tm4c123_sysctl_read(void *opaque, hwaddr addr, unsigned int size)
{
    TM4C123SysCtlState *s = opaque;

    trace_tm4c123_sysctl_read(addr);

    switch(addr) {
        case SYSCTL_DID0:
            return s->sysctl_did0;
        case SYSCTL_DID1:
            return s->sysctl_did1;
        case SYSCTL_PBORCTL:
            return s->sysctl_pborctl;
        case SYSCTL_RIS:
            return s->sysctl_ris;
        case SYSCTL_IMC:
            return s->sysctl_imc;
        case SYSCTL_MISC:
            return s->sysctl_misc;
        case SYSCTL_RESC:
            return s->sysctl_resc;
        case SYSCTL_RCC:
            return s->sysctl_rcc;
        case SYSCTL_GPIOHBCTL:
            return s->sysctl_gpiohbctl;
        case SYSCTL_RCC2:
            return s->sysctl_rcc2;
        case SYSCTL_MOSCCTL:
            return s->sysctl_moscctl;
        case SYSCTL_DSLPCLKCFG:
            return s->sysctl_dslpclkcfg;
        case SYSCTL_SYSPROP:
            return s->sysctl_sysprop;
        case SYSCTL_PIOSCCAL:
            return s->sysctl_piosccal;
        case SYSCTL_PIOSCSTAT:
            return s->sysctl_pioscstat;
        case SYSCTL_PLLFREQ0:
            return s->sysctl_pllfreq0;
        case SYSCTL_PLLFREQ1:
            return s->sysctl_pllfreq1;
        case SYSCTL_PLLSTAT:
            return s->sysctl_pllstat;
        case SYSCTL_SLPPWRCFG:
            return s->sysctl_slppwrcfg;
        case SYSCTL_DSLPPWRCFG:
            return s->sysctl_dslppwrcfg;
        case SYSCTL_LDOSPCTL:
            return s->sysctl_ldospctl;
        case SYSCTL_LDOSPCAL:
            return s->sysctl_ldospcal;
        case SYSCTL_LDODPCTL:
            return s->sysctl_ldodpctl;
        case SYSCTL_LDODPCAL:
            return s->sysctl_ldodpcal;
        case SYSCTL_SDPMST:
            return s->sysctl_sdpmst;
        case SYSCTL_PPWD:
            return s->sysctl_ppwd;
        case SYSCTL_PPTIMER:
            return s->sysctl_pptimer;
        case SYSCTL_PPGPIO:
            return s->sysctl_ppgpio;
        case SYSCTL_PPDMA:
            return s->sysctl_ppdma;
        case SYSCTL_PPHIB:
            return s->sysctl_pphib;
        case SYSCTL_PPUART:
            return s->sysctl_ppuart;
        case SYSCTL_PPSI:
            return s->sysctl_ppsi;
        case SYSCTL_PPI2C:
            return s->sysctl_ppi2c;
        case SYSCTL_PPUSB:
            return s->sysctl_ppusb;
        case SYSCTL_PPCAN:
            return s->sysctl_ppcan;
        case SYSCTL_PPADC:
            return s->sysctl_ppadc;
        case SYSCTL_PPACMP:
            return s->sysctl_ppacmp;
        case SYSCTL_PPPWM:
            return s->sysctl_pppwm;
        case SYSCTL_PPQEI:
            return s->sysctl_ppqei;
        case SYSCTL_PPEEPROM:
            return s->sysctl_ppeeprom;
        case SYSCTL_PPWTIMER:
            return s->sysctl_ppwtimer;
        case SYSCTL_SRWD:
            return s->sysctl_srwd;
        case SYSCTL_SRTIMER:
            return s->sysctl_srtimer;
        case SYSCTL_SRGPIO:
            return s->sysctl_srgpio;
        case SYSCTL_SRDMA:
            return s->sysctl_srdma;
        case SYSCTL_SRHIB:
            return s->sysctl_srhib;
        case SYSCTL_SRUART:
            return s->sysctl_sruart;
        case SYSCTL_SRSSI:
            return s->sysctl_srssi;
        case SYSCTL_SRI2C:
            return s->sysctl_sri2c;
        case SYSCTL_SRUSB:
            return s->sysctl_srusb;
        case SYSCTL_SRCAN:
            return s->sysctl_srcan;
        case SYSCTL_SRADC:
            return s->sysctl_sradc;
        case SYSCTL_SRACMP:
            return s->sysctl_sracmp;
        case SYSCTL_SRPWM:
            return s->sysctl_srpwm;
        case SYSCTL_SRQEI:
            return s->sysctl_srqei;
        case SYSCTL_SREEPROM:
            return s->sysctl_sreeprom;
        case SYSCTL_SRWTIMER:
            return s->sysctl_srwtimer;
        case SYSCTL_RCGCWD:
            return s->sysctl_rcgcwd;
        case SYSCTL_RCGCTIMER:
            return s->sysctl_rcgctimer;
        case SYSCTL_RCGCGPIO:
            return s->sysctl_rcgcgpio;
        case SYSCTL_RCGCDMA:
            return s->sysctl_rcgcdma;
        case SYSCTL_RCGCHIB:
            return s->sysctl_rcgchib;
        case SYSCTL_RCGCUART:
            return s->sysctl_rcgcuart;
        case SYSCTL_RCGCSSI:
            return s->sysctl_rcgcssi;
        case SYSCTL_RCGCI2C:
            return s->sysctl_rcgci2c;
        case SYSCTL_RCGCUSB:
            return s->sysctl_rcgcusb;
        case SYSCTL_RCGCCAN:
            return s->sysctl_rcgccan;
        case SYSCTL_RCGCADC:
            return s->sysctl_rcgcadc;
        case SYSCTL_RCGCACMP:
            return s->sysctl_rcgcacmp;
        case SYSCTL_RCGCPWM:
            return s->sysctl_rcgcpwm;
        case SYSCTL_RCGCQEI:
            return s->sysctl_rcgcqei;
        case SYSCTL_RCGCEEPROM:
            return s->sysctl_rcgceeprom;
        case SYSCTL_RCGCWTIMER:
            return s->sysctl_rcgcwtimer;
        case SYSCTL_SCGCWD:
            return s->sysctl_scgcwd;
        case SYSCTL_SCGCTIMER:
            return s->sysctl_scgctimer;
        case SYSCTL_SCGCGPIO:
            return s->sysctl_scgcgpio;
        case SYSCTL_SCGCDMA:
            return s->sysctl_scgcdma;
        case SYSCTL_SCGCHIB:
            return s->sysctl_scgchib;
        case SYSCTL_SCGCUART:
            return s->sysctl_scgcuart;
        case SYSCTL_SCGCSSI:
            return s->sysctl_scgcssi;
        case SYSCTL_SCGCI2C:
            return s->sysctl_scgci2c;
        case SYSCTL_SCGCUSB:
            return s->sysctl_scgcusb;
        case SYSCTL_SCGCCAN:
            return s->sysctl_scgccan;
        case SYSCTL_SCGCADC:
            return s->sysctl_scgcadc;
        case SYSCTL_SCGCACMP:
            return s->sysctl_scgcacmp;
        case SYSCTL_SCGCPWM:
            return s->sysctl_scgcpwm;
        case SYSCTL_SCGCQEI:
            return s->sysctl_scgcqei;
        case SYSCTL_SCGCEEPROM:
            return s->sysctl_scgceeprom;
        case SYSCTL_SCGCWTIMER:
            return s->sysctl_scgcwtimer;
        case SYSCTL_DCGCWD:
            return s->sysctl_dcgcwd;
        case SYSCTL_DCGCTIMER:
            return s->sysctl_dcgctimer;
        case SYSCTL_DCGCGPIO:
            return s->sysctl_dcgcgpio;
        case SYSCTL_DCGCDMA:
            return s->sysctl_dcgcdma;
        case SYSCTL_DCGCHIB:
            return s->sysctl_dcgchib;
        case SYSCTL_DCGCUART:
            return s->sysctl_dcgcuart;
        case SYSCTL_DCGCSSI:
            return s->sysctl_dcgcssi;
        case SYSCTL_DCGCI2C:
            return s->sysctl_dcgci2c;
        case SYSCTL_DCGCUSB:
            return s->sysctl_dcgcusb;
        case SYSCTL_DCGCCAN:
            return s->sysctl_dcgccan;
        case SYSCTL_DCGCADC:
            return s->sysctl_dcgcadc;
        case SYSCTL_DCGCACMP:
            return s->sysctl_dcgcacmp;
        case SYSCTL_DCGCPWM:
            return s->sysctl_dcgcpwm;
        case SYSCTL_DCGCQEI:
            return s->sysctl_dcgcqei;
        case SYSCTL_DCGCEEPROM:
            return s->sysctl_dcgceeprom;
        case SYSCTL_DCGCWTIME:
            return s->sysctl_dcgcwtime;
        case SYSCTL_PRWD:
            return s->sysctl_prwd;
        case SYSCTL_PRTIMER:
            return s->sysctl_prtimer;
        case SYSCTL_PRGPIO:
            return s->sysctl_prgpio;
        case SYSCTL_PRDMA:
            return s->sysctl_prdma;
        case SYSCTL_PRHIB:
            return s->sysctl_prhib;
        case SYSCTL_PRUART:
            return s->sysctl_pruart;
        case SYSCTL_PRSSI:
            return s->sysctl_prssi;
        case SYSCTL_PRI2C:
            return s->sysctl_pri2c;
        case SYSCTL_PRUSB:
            return s->sysctl_prusb;
        case SYSCTL_PRCAN:
            return s->sysctl_prcan;
        case SYSCTL_PRADC:
            return s->sysctl_pradc;
        case SYSCTL_PRACMP:
            return s->sysctl_pracmp;
        case SYSCTL_PRPWM:
            return s->sysctl_prpwm;
        case SYSCTL_PRQEI:
            return s->sysctl_prqei;
        case SYSCTL_PREEPROM:
            return s->sysctl_preeprom;
        case SYSCTL_PRWTIMER:
            return s->sysctl_prwtimer;
        default:
            LOG("Bad address 0x%"HWADDR_PRIx"\n", addr);
            break;
    }
    return 0;
}

static const MemoryRegionOps tm4c123_sysctl_ops = {
    .read = tm4c123_sysctl_read,
    .write = tm4c123_sysctl_write,
    .endianness = DEVICE_NATIVE_ENDIAN,
};

static void tm4c123_sysctl_init(Object *obj) {
    TM4C123SysCtlState *s = TM4C123_SYSCTL(obj);

    memory_region_init_io(&s->mmio, obj, &tm4c123_sysctl_ops, s, TYPE_TM4C123_SYSCTL, 0xFFF);
    sysbus_init_mmio(SYS_BUS_DEVICE(obj), &s->mmio);
}

static void tm4c123_sysctl_class_init(ObjectClass *kclass, void *data)
{
    DeviceClass *dc = DEVICE_CLASS(kclass);
    dc->reset = tm4c123_sysctl_reset;
}

static const TypeInfo tm4c123_sysctl_info = {
    .name = TYPE_TM4C123_SYSCTL,
    .parent = TYPE_SYS_BUS_DEVICE,
    .instance_size = sizeof(TM4C123SysCtlState),
    .instance_init = tm4c123_sysctl_init,
    .class_init = tm4c123_sysctl_class_init,
};

static void tm4c123_sysctl_register_types(void)
{
    type_register_static(&tm4c123_sysctl_info);
}

type_init(tm4c123_sysctl_register_types)
