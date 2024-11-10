#include <stdlib.h>

void __attribute__((__noreturn__)) abort(void)
{
    asm volatile("cli" : : : "memory");
    hlt:
    asm volatile("hlt" : : : "memory");
    goto hlt;
}