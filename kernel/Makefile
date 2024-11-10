CFLAGS = -O2 -mgeneral-regs-only -mno-red-zone -Wall -fpic -ffreestanding -fno-strict-aliasing -fno-stack-protector -nostdinc -nostdlib -mno-mmx -mno-sse -mno-sse2 -I../include -I../cross/lib/gcc/x86_64-elf/14.2.0/include
CFLAGS-DEBUG = $(CFLAGS) -Og -ggdb
LDFLAGS =  -nostdlib -n -T link.ld
STRIPFLAGS =  -s -K mmio -K fb -K bootboot -K environment -K initstack

all: kernel

kernel: kernel.c
	x86_64-elf-gcc $(CFLAGS) -c kernel.c -o kernel.o
	x86_64-elf-gcc $(CFLAGS) -c vm.c -o vm.o
	x86_64-elf-gcc $(CFLAGS) -c gdt.c -o gdt.o
	x86_64-elf-gcc $(CFLAGS) -c drivers/kbd.c -o drivers/kbd.o
	x86_64-elf-gcc $(CFLAGS) -c drivers/console.c -o drivers/console.o
	x86_64-elf-gcc ../libc/stdio.c $(CFLAGS) -o ../libc/stdio.o -c
	x86_64-elf-gcc ../libc/ctype.c $(CFLAGS) -o ../libc/ctype.o -c
	x86_64-elf-gcc ../libc/stdlib.c $(CFLAGS) -o ../libc/stdlib.o -c
	x86_64-elf-gcc ../libc/string.c $(CFLAGS) -o ../libc/string.o -c
	x86_64-elf-ld -r -b binary -o font.o font.psf
	x86_64-elf-ld $(LDFLAGS) *.o drivers/*.o ../libc/*.o -o kernel.elf
	x86_64-elf-strip $(STRIPFLAGS) -o ../boot/sys/kernel.elf kernel.elf

debug: kernel.c
	x86_64-elf-gcc $(CFLAGS-DEBUG) -c kernel.c -o kernel.o
	x86_64-elf-gcc $(CFLAGS-DEBUG) -c vm.c -o vm.o
	x86_64-elf-gcc $(CFLAGS-DEBUG) -c gdt.c -o gdt.o
	x86_64-elf-gcc $(CFLAGS-DEBUG) -c drivers/kbd.c -o drivers/kbd.o
	x86_64-elf-gcc $(CFLAGS-DEBUG) -c drivers/console.c -o drivers/console.o
	x86_64-elf-gcc ../libc/stdio.c $(CFLAGS-DEBUG) -o ../libc/stdio.o -c
	x86_64-elf-gcc ../libc/ctype.c $(CFLAGS-DEBUG) -o ../libc/ctype.o -c
	x86_64-elf-gcc ../libc/stdlib.c $(CFLAGS-DEBUG) -o ../libc/stdlib.o -c
	x86_64-elf-gcc ../libc/string.c $(CFLAGS-DEBUG) -o ../libc/string.o -c
	x86_64-elf-ld -r -b binary -o font.o font.psf
	x86_64-elf-ld $(LDFLAGS) *.o drivers/*.o ../libc/*.o -o ../boot/sys/kernel.elf

clean:
	rm *.o *.elf drivers/*.o ../libc/*.o
