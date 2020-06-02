#Presentation

WazaBee is an attack allowing to transmit and receive 802.15.4 packets by diverting Bluetooth Low Energy chips. This software is a basic proof of concept implementing the two communication primitives used by this attack. This source code is not dedicated to an offensive usage, and it is only provided for research purposes. It is released as an open-source project under MIT license.

This repository contains the WazaBee PoC firmware for the following development board: **Texas Instruments CC1352-R1**. This firmware exposes a basic API that can be manipulated using the WazaBee Command Line Interface.

#Building and flashing this firmware

* Download and install *SIMPLELINK-CC13X2-26X2-SDK*: <https://www.ti.com/tool/LAUNCHXL-CC1352R1>

* Download *GNU ARM Embedded Toolchain*: <https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm/downloads>

* In the SDK directory, there is a file named *"imports.mak"*. Modify it to match your build environment:
```
XDC_INSTALL_DIR        ?= /home/$(USER)/ti/xdctools_3_61_00_16_core
SYSCONFIG_TOOL         ?= /home/$(USER)/ti/sysconfig_1.4.0/sysconfig_cli.sh


CCS_ARMCOMPILER        ?= /home/username/ti/ccs1000/ccs/tools/compiler/ti-cgt-arm_20.2.0.LTS
GCC_ARMCOMPILER        ?= /home/$(USER)/ti/gcc-arm-none-eabi-9-2020-q2-update-x86_64-linux/gcc-arm-none-eabi-9-2020-q2-update
```

* Go to the SDK directory, then enter the following commands:
```
$ cd kernel/tirtos/builds/CC1352R1_LAUNCHXL/release/gcc
$ make
```

* Download and install *UniFlash* from *TI*: <http://www.ti.com/tool/download/UNIFLASH>

* Add the following directory in your *PATH*: *$(UNIFLASH_DIRECTORY)/deskdb/content/TICloudAgent/linux/ccs_base/DebugServer/bin/*
```
export PATH=$PATH:/home/$USER/ti/uniflash_6.0.0/deskdb/content/TICloudAgent/linux/ccs_base/DebugServer/bin/
```

* Go to the *wazabee\_ti* directory. If needed, modify the *SIMPLELINK_CC13X2_26X2_SDK_INSTALL_DIR* variable in the makefile to match your build environment.

* Enter the following command to compile:
```
$ mkdir build
$ make
```

* Plug the CC1352-R1 board, then enter the following command to flash the firmware:
```
$ make load
```
