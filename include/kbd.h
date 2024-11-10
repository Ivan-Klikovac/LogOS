#ifndef _DRIVERS_KBD_H
#define _DRIVERS_KBD_H

#include <stdint.h>

#define LSHIFT 0x2A
#define RSHIFT 0x36
#define LCTRL  0x1D
#define RCTRL  0x38
#define CAPS   0x3A

extern int shift;
extern int ctrl;
extern int caps;

void kbdint(uint8_t);

#endif