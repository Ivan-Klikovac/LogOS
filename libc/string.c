#include <string.h>

int memcmp(const void* aptr, const void* bptr, size_t size)
{
    const unsigned char* a = (const unsigned char*) aptr;
    const unsigned char* b = (const unsigned char*) bptr;

    for(size_t i = 0; i < size; i++) {
        if(a[i] < b[i]) return -1;
        if(b[i] < a[i]) return 1;
    }

    return 0;
}

void* memcpy(void* restrict dstptr, const void* restrict srcptr, size_t size)
{
    unsigned char* dst = (unsigned char*) dstptr;
    const unsigned char* src = (const unsigned char*) srcptr;

    for(size_t i = 0; i < size; i++) dst[i] = src[i];
    return dstptr;
}

void* memmove(void* dstptr, const void* srcptr, size_t size)
{
    unsigned char* dst = (unsigned char*) dstptr;
    const unsigned char* src = (const unsigned char*) srcptr;

    if(dst < src) for(size_t i = 0; i < size; i++) dst[i] = src[i];
    else for(size_t i = size; i; i--) dst[i-1] = src[i-1];
    return dstptr;
}

void* memset(void* bufptr, int value, size_t size)
{
    unsigned char* buf = (unsigned char*) bufptr;

    for(size_t i = 0; i < size; i++) buf[i] = (unsigned char) value;
    return bufptr;
}

size_t strlen(const char* s)
{
    size_t n = 0;
    
    while(s[n]) n++;
    return n;
}

char* strchr(char* s, char c)
{
    int i = 0;
    for(; (*(s+i) != c) && *(s+i); i++);
    if(!(*(s+i)) && c) return 0;
    return s+i;
}

char* strrchr(char* s, char c)
{
    char* ptr = s + strlen(s);
    while(ptr > s) {
        if(*ptr == c) return ptr;
        ptr--;
    }
    return 0;
}

int strcmp(char* s1, char* s2)
{
    while(*s1 || *s2) {
        if(*s1 > *s2) return 1;
        if(*s1 < *s2) return -1;
        s1++;
        s2++;
    }
    return 0;
}

int strncmp(char* s1, char* s2, size_t n)
{
    while(n && (*s1 || *s2)) {
        if(*s1 > *s2) return 1;
        if(*s1 < *s2) return -1;
        s1++;
        s2++;
        n--;
    }
    return 0;
}