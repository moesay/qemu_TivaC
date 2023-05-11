/*
 * TM4C123GH6PM SoC
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

#ifndef HW_ARM_TM4C123GH6PM_SOC_H
#define HW_ARM_TM4C123GH6PM_SOC_H

#include "hw/arm/armv7m.h"
#include "qom/object.h"
#include "hw/clock.h"
#include "hw/char/tm4c123_usart.h"
#include "hw/misc/tm4c123_sysctl.h"
#include "hw/gpio/tm4c123_gpio.h"
#include "hw/watchdog/tm4c123_watchdog.h"

#define TYPE_TM4C123GH6PM_SOC "tm4c123gh6pm-soc"

OBJECT_DECLARE_SIMPLE_TYPE(TM4C123GH6PMState, TM4C123GH6PM_SOC)

#define FLASH_BASE_ADDRESS 0x00000000
#define FLASH_SIZE (256 * 1024)
#define SRAM_BASE_ADDRESS 0x20000000
#define SRAM_SIZE (32 * 1024)

#define SYSCTL_ADDR 0x400FE000

#define USART_COUNT 8
#define GPIO_COUNT 6
#define WDT_COUNT 2

struct TM4C123GH6PMState {
    SysBusDevice parent_obj;

    char *cpu_type;

    ARMv7MState armv7m;

    TM4C123USARTState usart[USART_COUNT];
    TM4C123SysCtlState sysctl;
    TM4C123GPIOState gpio[GPIO_COUNT];
    TM4C123WatchdogState wdt[WDT_COUNT];

    MemoryRegion sram;
    MemoryRegion alias_region;
    MemoryRegion flash;
};

#endif
