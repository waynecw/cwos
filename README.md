cwos
====

CWOS: A Tiny ARM-based OS 

Function
--------
CWOS is still under development. Currently it can echo user's input through UART channel to demonstrate its IRQ handling mechanism and UART driver with MMU enabled.

Environment
-----------

* Target platform: QEMU emulated ARM Versatile Platform Board 
  - CPU: ARM 926EJ-S
  - Memory: 128 MB
* Host environment
  - Verified host OS
      * Ubuntu 14.04 x64
  - QEMU emulator
      * Ubuntu default build
  - Cross compile toolchain
      * Ubuntu package `gcc-arm-none-eabi`

Features
--------

* Relocation at boot time
* IRQ handling
* Basic UART driver
* Partial C-style string library implementation
* printk() support for debugging
* Non-recursive Make design


Memory Layout
-------------

* Kernel space (128M): `0 ~ 0x08000000`
* IO space: `0xD0000000 ~ 0xD01F5000`


Install & Execute
-----------------
1.  Download toolchain `apt-get install gcc-arm-none-eabi` in Ubuntu.
2.  `git clone https://github.com/waynecw/cwos .` to retreive source code
3.  `cd cwos; make`
4.  `make qemu` to execute CWOS in QEMU
5.  `make qemu-debug` to execute CWOS in QEMU for attaching GDB.

