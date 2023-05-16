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

#include "qemu/osdep.h"
#include "qapi/error.h"
#include "qemu/module.h"
#include "hw/arm/boot.h"
#include "exec/address-spaces.h"
#include "hw/arm/tm4c123gh6pm_soc.h"
#include "hw/qdev-properties.h"
#include "hw/qdev-clock.h"
#include "hw/misc/unimp.h"
#include "sysemu/sysemu.h"

static const uint32_t gpio_addrs[GPIO_COUNT] = {
    0x40004000,
    0x40005000,
    0x40006000,
    0x40007000,
    0x40024000,
    0x40025000
};

static const uint32_t usart_addrs[USART_COUNT] =
{
    0x4000C000,
    0x4000D000,
    0x4000E000,
    0x4000F000,
    0x40010000,
    0x40011000,
    0x40012000,
    0x40013000
};

static const uint32_t wdt_addrs[WDT_COUNT] = {
    0x40000000,
    0x40001000
};

static const uint32_t gptm_addrs[GPTM_COUNT] = {
    0x40030000,
    0x40031000,
    0x40032000,
    0x40033000,
    0x40034000,
    0x40035000,
    0x40036000,
    0x40037000,
    0x4003C800,
    0x4003D000,
    0x4003E000,
    0x4003F000,
};

static const uint16_t usart_irqs[USART_COUNT] = {5, 6, 33, 59, 60, 61, 62, 63};
static const uint16_t gpio_irqs[GPIO_COUNT] = {0, 1, 2, 3, 4, 30};
static const uint16_t wdt_irqs[WDT_COUNT] = {18, 18};
static const uint16_t gptm_irqs[GPTM_COUNT * 2] =
{19, 20, 21, 22, 23, 24, 35, 36, 70, 71, 92, 93,
 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105};

static void tm4c123gh6pm_soc_initfn(Object *obj)
{
    int i;
    TM4C123GH6PMState *s = TM4C123GH6PM_SOC(obj);

    object_initialize_child(obj, "armv7m", &s->armv7m, TYPE_ARMV7M);
    object_initialize_child(obj, "sysctl", &s->sysctl, TYPE_TM4C123_SYSCTL);

    for(i = 0; i < USART_COUNT; i++) {
        object_initialize_child(obj, "usart[*]", &s->usart[i], TYPE_TM4C123_USART);
    }

    for(i = 0; i < GPIO_COUNT; i++) {
        object_initialize_child(obj, "gpio[*]", &s->gpio[i], TYPE_TM4C123_GPIO);
    }

    for(i = 0; i < WDT_COUNT; i++) {
        object_initialize_child(obj, "watchdog-timer[*]", &s->wdt[i], TYPE_TM4C123_WATCHDOG);
    }

    for(i = 0; i < GPTM_COUNT; i++) {
        object_initialize_child(obj, "gptm[*]", &s->gptm[i], TYPE_TM4C123_GPTM);
    }
}

static void tm4c123gh6pm_soc_realize(DeviceState *dev_soc, Error **errp)
{
    TM4C123GH6PMState *s = TM4C123GH6PM_SOC(dev_soc);
    DeviceState *armv7m;
    DeviceState *dev;
    SysBusDevice *busdev;
    int i;

    MemoryRegion *system_memory = get_system_memory();

    //init flash memory
    memory_region_init_rom(&s->flash, OBJECT(dev_soc), "TM4C123GH6PM.flash", FLASH_SIZE, &error_fatal);
    memory_region_add_subregion(system_memory, FLASH_BASE_ADDRESS, &s->flash);

    //init sram and the sram alias region
    memory_region_init_ram(&s->sram, OBJECT(dev_soc), "TM4C123GH6PM.sram", SRAM_SIZE, &error_fatal);
    memory_region_add_subregion(system_memory, SRAM_BASE_ADDRESS, &s->sram);

    /* Init ARMv7m */
    armv7m = DEVICE(&s->armv7m);
    qdev_prop_set_uint32(armv7m, "num-irq", 138);
    qdev_prop_set_string(armv7m, "cpu-type", s->cpu_type);
    qdev_prop_set_bit(armv7m, "enable-bitband", true);
    qdev_connect_clock_in(armv7m, "cpuclk", s->sysctl.mainclk);
    qdev_connect_clock_in(armv7m, "refclk", s->sysctl.mainclk);
    object_property_set_link(OBJECT(&s->armv7m), "memory",
                             OBJECT(get_system_memory()), &error_abort);

    if(!sysbus_realize(SYS_BUS_DEVICE(&s->armv7m), errp)) {
        return;
    }

    /* USART */
    for(i = 0; i < USART_COUNT; i++) {
        dev = DEVICE(&(s->usart[i]));
        s->usart[i].sysctl = &s->sysctl;
        qdev_prop_set_chr(dev, "chardev", serial_hd(i));
        if(!sysbus_realize(SYS_BUS_DEVICE(&s->usart[i]), errp)) {
            return;
        }
        busdev = SYS_BUS_DEVICE(dev);
        sysbus_mmio_map(busdev, 0, usart_addrs[i]);
        sysbus_connect_irq(busdev, 0, qdev_get_gpio_in(armv7m, usart_irqs[i]));
    }

    /* GPIO */
    for(i = 0; i < GPIO_COUNT; i++) {
        dev = DEVICE(&(s->gpio[i]));
        s->gpio[i].sysctl = &s->sysctl;
        if(!sysbus_realize(SYS_BUS_DEVICE(&s->gpio[i]), errp)) {
            return;
        }
        busdev = SYS_BUS_DEVICE(dev);
        sysbus_mmio_map(busdev, 0, gpio_addrs[i]);
        sysbus_connect_irq(busdev, 0, qdev_get_gpio_in(armv7m, gpio_irqs[i]));
    }

    /* Watchdog Timers */
    for(i = 0; i < WDT_COUNT; i++) {
        dev = DEVICE(&(s->wdt[i]));
        s->wdt[i].sysctl = &s->sysctl;
        if(!sysbus_realize(SYS_BUS_DEVICE(&s->wdt[i]), errp)) {
            return;
        }
        busdev = SYS_BUS_DEVICE(dev);
        sysbus_mmio_map(busdev, 0, wdt_addrs[i]);
        sysbus_connect_irq(busdev, 0, qdev_get_gpio_in(armv7m, wdt_irqs[i]));
    }

    /* General purpose timers */
    int j = 0;
    for(i = 0, j = 0; i < GPTM_COUNT; i++, j+=2) {
        dev = DEVICE(&(s->gptm[i]));
        s->gptm[i].sysctl = &s->sysctl;
        if(!sysbus_realize(SYS_BUS_DEVICE(&s->gptm[i]), errp)) {
            return;
        }
        busdev = SYS_BUS_DEVICE(dev);
        sysbus_mmio_map(busdev, 0, gptm_addrs[i]);
        sysbus_connect_irq(busdev, 0, qdev_get_gpio_in(armv7m, gptm_irqs[j]));
        sysbus_connect_irq(busdev, 1, qdev_get_gpio_in(armv7m, gptm_irqs[j+1]));
    }

    /* SYSCTL */
    dev = DEVICE(&(s->sysctl));
    if(!sysbus_realize(SYS_BUS_DEVICE(&s->sysctl), errp)) {
        return;
    }
    busdev = SYS_BUS_DEVICE(dev);
    sysbus_mmio_map(busdev, 0, SYSCTL_ADDR);


    create_unimplemented_device("SSI_0", 0x40008000, 0xFFF);
    create_unimplemented_device("SSI_1", 0x40009000, 0xFFF);
    create_unimplemented_device("SSI_2", 0x4000A000, 0xFFF);
    create_unimplemented_device("SSI_3", 0x4000B000, 0xFFF);

    create_unimplemented_device("I2C_0", 0x40020000, 0xFFF);
    create_unimplemented_device("I2C_1", 0x40021000, 0xFFF);
    create_unimplemented_device("I2C_2", 0x40022000, 0xFFF);
    create_unimplemented_device("I2C_3", 0x40023000, 0xFFF);

    create_unimplemented_device("PWM_0", 0x40028000, 0xFFF);
    create_unimplemented_device("PWM_1", 0x40029000, 0xFFF);

    create_unimplemented_device("QEI_0", 0x4002C000, 0xFFF);
    create_unimplemented_device("QEI_1", 0x4002D000, 0xFFF);

    create_unimplemented_device("ADC_0", 0x40038000, 0xFFF);
    create_unimplemented_device("ADC_1", 0x40039000, 0xFFF);

    create_unimplemented_device("ANALOG_CMP", 0x4003C000, 0xFFF);

    create_unimplemented_device("CAN_0", 0x40040000, 0xFFF);
    create_unimplemented_device("CAN_1", 0x40041000, 0xFFF);

    create_unimplemented_device("USB", 0x40050000, 0xFFF);

    create_unimplemented_device("GPIO_A_AHB", 0x40058000, 0xFFF);
    create_unimplemented_device("GPIO_B_AHB", 0x40059000, 0xFFF);
    create_unimplemented_device("GPIO_C_AHB", 0x4005A000, 0xFFF);
    create_unimplemented_device("GPIO_D_AHB", 0x4005B000, 0xFFF);
    create_unimplemented_device("GPIO_E_AHB", 0x4005C000, 0xFFF);
    create_unimplemented_device("GPIO_F_AHB", 0x4005D000, 0xFFF);

    create_unimplemented_device("EEPROM", 0x400AF000, 0xFFF);
    create_unimplemented_device("SYS_EXC", 0x400F9000, 0xFFF);
    create_unimplemented_device("HIBERNATION_MOD", 0x400FC000, 0xFFF);
    create_unimplemented_device("FLASH_CONT", 0x400FD000, 0xFFF);
    create_unimplemented_device("SYS_CONT", 0x400FE000, 0xFFF);

    create_unimplemented_device("uDMA", 0x400FF000, 0xFFF);
}

static Property tm4c123gh6pm_soc_properties[] = {
    DEFINE_PROP_STRING("cpu-type", TM4C123GH6PMState, cpu_type),
    DEFINE_PROP_END_OF_LIST(),
};

static void tm4c123gh6pm_soc_class_init(ObjectClass *klass, void *data)
{
    DeviceClass *dc = DEVICE_CLASS(klass);

    dc->realize = tm4c123gh6pm_soc_realize;
    device_class_set_props(dc, tm4c123gh6pm_soc_properties);
}

static const TypeInfo tm4c123gh6pm_soc_info = {
    .name          = TYPE_TM4C123GH6PM_SOC,
    .parent        = TYPE_SYS_BUS_DEVICE,
    .instance_size = sizeof(TM4C123GH6PMState),
    .instance_init = tm4c123gh6pm_soc_initfn,
    .class_init    = tm4c123gh6pm_soc_class_init,
};

static void tm4c123gh6pm_soc_types(void)
{
    type_register_static(&tm4c123gh6pm_soc_info);
}


type_init(tm4c123gh6pm_soc_types)
