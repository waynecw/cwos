cwos
====

CWOS: A Tiny ARM-based OS 

Function
--------
CWOS is still under development. Currently it can echo user's input through UART channel to demonstrate its IRQ handling mechanism and UART driver.

Environment
-----------

* Target platform: QEMU emulated ARM Versatile Platform Board 
  - CPU: ARM 926EJ-S
  - Memory: 128 MB
* Host environment
  - Verified host OS
      * Ubuntu 12.04 LTS x86
      * Ubuntu 13.04 x64
  - QEMU emulator
      * Ubuntu default build
  - Cross compile toolchain
      * Mentor Sourcery Codebench Lite Edition for EABI 2012.09-63

Features
--------

* Relocation at boot time
* IRQ handling
* Virtual memory enablement
* Basic UART driver
* Partial C-style string library implementation
* printk() support for debugging
* Non-recursive Make design


Memory Layout
-------------

* Kernel space (128M): `0xC0000000 ~ 0xC8000000` maps to `0 ~ 0x08000000`
* IO space: `0xD0000000 ~ 0xD01F5000` maps to `0x10000000 ~ 0x101F5000`


Install & Execute
-----------------
1.  Download Sourcery Codebench installer (2012.09-63) from [Mentor Graphic][1]
2.  Execute the installer to instal toolchain
  *  Type `/bin/sh /path/to/package.bin` to start installer in GUI
  *  Or type `/bin/sh -i /path/to/package.bin` to start installer in CLI
  *  *Notice*: In Ubuntu, `/bin/sh` is a soft link of `/bin/dash`. Use `dpkg-reconfigure -plow /bin/sh` to change the default setting.
3.  `git clone https://github.com/waynecw/cwos .` to retreive source code
4.  `cd cwos; make`
5.  `make qemu` to execute CWOS in QEMU
6.  `make qemu-debug` to execute CWOS in QEMU for attaching GDB.

[1]: http://www.mentor.com/embedded-software/sourcery-tools/sourcery-codebench/overview
