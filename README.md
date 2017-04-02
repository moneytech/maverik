# maverik OS

maverik OS is a very simple OS for the Raspberry PI 2

#### How to compile

Make sure you build the toolchain for ARM.

In my case I build a arm-none-eabi toolchain.
If you build any other toolchain, you need to change the Makefile.

    make

#### qemu

Maybe qemu needs 0x10000 instead of 0x8000 in the linker script.

    qemu-system-arm -m 256 -M raspi2 -serial stdio -kernel kernel.img