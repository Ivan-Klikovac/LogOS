#include <stdio.h>
#include <stdlib.h>
#include <console.h>
#include <vm.h>
#include <gdt.h>

extern BOOTBOOT bootboot;
extern unsigned char environment[4096];
extern uint32_t fb[];

extern volatile unsigned char _binary_font_psf_start;

// entry point
void _start()
{
    /*** NOTE: this code runs on all cores in parallel ***/

    console_init();

    int x, y, s=bootboot.fb_scanline / 4, w=bootboot.fb_width, h=bootboot.fb_height;

    if(s) {
        for (int y = 0; y < h; y++) fb[y * s + w / 2] = 0x00ffffff;
        for (int x = 0; x < w; x++) fb[(h / 2) * s + x] = 0x00ffffff;

        for(y=0;y<20;y++) { for(x=0;x<20;x++) { fb[(y + 20) * s + x + 20] = 0x00FF0000; } }
        for(y=0;y<20;y++) { for(x=0;x<20;x++) { fb[(y + 20) * s + x + 50] = 0x0000FF00; } }
        for(y=0;y<20;y++) { for(x=0;x<20;x++) { fb[(y + 20) * s + x + 80] = 0x000000FF; } }

        // say hello
        puts("--- hello ---\n\n\n\n");
    }

    volatile int gdb = 0;
    gdb_wait:
    //if(!gdb) goto gdb_wait;
    
    printf("--- TESTING VM ---\n");
    vm_test();

    printf("\nsetting up GDT...\n");
    gdt_init();

    printf("setting up TSS...\n");
    tss_init();

    printf("setting up IDT...\n");
    idt_init();

    printf("\n--- TESTING INTERRUPTS ---\n\n");
    
    hlt:
    asm volatile("hlt" : : : "memory");
    goto hlt;
}
