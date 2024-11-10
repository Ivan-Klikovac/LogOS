# LogOS

## Description
64-bit x86 "operating system" with no user space meant for testing processor behavior

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