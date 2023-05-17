Texas Instruments EK-TM4C123GXL Evaluation Board, ``Tiva C``
========================================================================================

The `Tiva C`_ board is an evaluation platform for ARM Cortex-M4-based microcontrollers.
Its based on the `TM4C123GH6PM`_ microcontroller by Texas Instruments.

.. _Tiva C: https://www.ti.com/tool/EK-TM4C123GXL
.. _TM4C123GH6PM: https://www.ti.com/product/TM4C123GH6PM

Supported modules
-----------------

 * ARM Cortex-M4
 * General Purpose Input/Output (GPIO)
 * General Purpose Timers (GPTM)
 * Serial Ports (USART)
 * System Control (SYSCTL)
 * Watchdog Timers (WDT)

Missing modules
---------------

 * Dynamic Memory Access (uDMA)
 * Analog to Digital Converter (ADC)
 * Synchronous Serial Interface (SSI)
 * Inter-Integrated Circuit Interface (I2C)
 * Controller Area Network (CAN)
 * USB Controller
 * Analog Comparators
 * Pulse Width Modulator (PWM)
 * Quadrature Encoder Interface (QEI)

Boot options
------------

The Tiva C machines could be started using the ``-kernel`` option to load a binary file.

.. code-block:: bash

  $ qemu-system-arm -M tivac -kernel binary.elf -s -S

The ``-s -S`` switches are for debugging, in another terminal window you can do:

.. code-block:: bash

   $ arm-none-eabi-gdb binary.elf
   (gdb) target remote :1234
