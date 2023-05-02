===========
QEMU_TivaC README
===========

QEMU_TivaC aims to provide a TivaC support for the QEMU project. The project is far from being complete (so far) but im working on it.

Building
========

QEMU is multi-platform software intended to be buildable on all modern
Linux platforms, OS-X, Win32 (via the Mingw64 toolchain) and a variety
of other UNIX targets. The simple steps to build QEMU are:


.. code-block:: shell

  git clone git@github.com:moesay/qemu_TivaC.git
  cd qemu_TivaC
  mkdir build
  cd build
  ../configure --target-list=arm-softmmu
  make

Additional information can also be found online via the QEMU website:

* `<https://wiki.qemu.org/Hosts/Linux>`_
* `<https://wiki.qemu.org/Hosts/Mac>`_
* `<https://wiki.qemu.org/Hosts/W32>`_


Usage
=====

After building QEMU_TivaC, you can use it with gdb through the provided gdbstub.

.. code-block:: shell

   qemu-system-arm -M tivac -kernel <your elf file path> -s -S -D log_file.txt -d unimp -serial file:qemu_serial_op.txt -monitor stdio -nographic

Now, QEMU_TivaC is ready to be attached to gdb. In a different terminal run arm gdb and attach QEMU_TivaC to it.

.. code-block:: shell

   arm-none-eabi-gdb <your elf file path>
   (gdb) target remote :1234

For the original QEMU readme, go to `<https://github.com/qemu/qemu>`_
