#ifndef _LIBC_STDIO_H
#define _LIBC_STDIO_H

#include <stdint.h>
#include <bootboot.h>

extern BOOTBOOT bootboot;
extern unsigned char environment[4096];
extern uint32_t fb[];

extern volatile unsigned char _binary_font_psf_start;

typedef struct {
    uint32_t magic;
    uint32_t version;
    uint32_t headersize;
    uint32_t flags;
    uint32_t numglyph;
    uint32_t bytesperglyph;
    uint32_t height;
    uint32_t width;
    uint8_t glyphs;
} __attribute__((packed)) psf2_t;

void stdio_init(void);
void putchar(char);
int puts(const char*);
int printf(const char* restrict, ...);
int printd(int64_t);
int printud(uint64_t);
int printx(uint64_t);
void stdio_debug(void);

#endif