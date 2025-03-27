# LogOS

## Description
64-bit x86 "operating system" with no user space meant for testing processor behavior.
The project arose out of an interest in Christopher Domas' whitepaper on undocumented x86 instructions.
The paper can be found here: https://github.com/xoreaxeaxeax/sandsifter/blob/master/references/domas_breaking_the_x86_isa_wp.pdf

## Requirements
- Toolchain for x86_64-elf (https://wiki.osdev.org/GCC_Cross-Compiler)
- QEMU

## Installation
First create a toolchain for `x86_64-elf` (https://wiki.osdev.org/GCC_Cross-Compiler), compile the code using the given Makefile in the kernel folder (`make kernel` or `make debug`), then run `./run` or `./debug` in the project root folder.

For debugging: 
- uncomment the gdb line in kernel.c
- compile the project with `make debug` (which compiles with debug info and -Og)
- run `./debug` (it runs qemu with -s for remote debugging and some useful flags)
- open `boot/sys/kernel.elf` in gdb
- tell gdb to connect to qemu using `target remote localhost:1234`
- finally, do `set var gdb=1` to let the code execute normally

## To-do list
- Add a filesystem

## License
GPLv3

## Acknowledgements
I've used bzt's BOOTBOOT-compliant bootloader, his mkbootimg, and his example OS as a starting point
https://gitlab.com/bztsrc/bootboot
