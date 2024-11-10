#include <stdio.h>
#include <ctype.h>
#include <stdarg.h>
#include <limits.h>
#include <stddef.h>
#include <string.h>
#include <console.h>

void putchar(char c)
{
    consputc(c);
}

int puts(const char* s)
{
    while(*s) {
        putchar(*s);
        s++;
    }
    //putchar('\n'); // uncomment this later

    return 0;
}

static int print(const char* data, size_t length) {
	const unsigned char* bytes = (const unsigned char*) data;
	for (size_t i = 0; i < length; i++) putchar(bytes[i]);
	return 1;
}

int printf(const char* restrict format, ...) {
	va_list parameters;
	va_start(parameters, format);
    int written = 0;

	while(*format != '\0') {
		if(format[0] != '%' || format[1] == '%') {
			if(format[0] == '%') format++;
			size_t amount = 1;
			while(format[amount] && format[amount] != '%') amount++;
			print(format, amount);
			format += amount;
            written += amount;
			continue;
		}

		const char* format_begun_at = format++;

		if(*format == 'c') {
			format++;
			char c = (char) va_arg(parameters, int);
			putchar(c);
            written++;
		}
        
        else if(*format == 's') {
			format++;
			const char* str = va_arg(parameters, const char*);
            puts(str);
            written += strlen(str);
		}
        
        else if(*format == 'd') {
			format++;
			int64_t n = va_arg(parameters, int64_t);
			written += printd(n);
		}

        else if(*format == 'u') {
            format++;
            uint64_t n = va_arg(parameters, uint64_t);
            written += printud(n);
        }

        else if(*format == 'x') {
            // assume unsigned
            format++;
            uint64_t n = va_arg(parameters, uint64_t);
            written += printx(n);
        }
        
        else{
			format = format_begun_at;
			puts(format);
			int len = strlen(format);
            format += len;
            written += len;
		}
	}

	va_end(parameters);
    return written;
}

char s[64];

int printd(int64_t n)
{
    if(!n) {
        putchar('0');
        return 1;
    }

    memset(s, 0, 64);
    int i = 62;

    if(n < 0) {
        putchar('-');
        n = -n;
    }

    while(n) {
        s[i--] = (n % 10) + '0';
        n /= 10;
    }

    puts(s+i+1);
    return strlen(s+i+1);
}

int printud(uint64_t n)
{
    if(!n) {
        putchar('0');
        return 1;
    }

    memset(s, 0, 64);
    int i = 62;
    while(n) {
        s[i--] = (n % 10) + '0';
        n /= 10;
    }

    puts(s+i+1);
    return strlen(s+i+1);
}

int printx(uint64_t n)
{
    if(!n) {
        putchar('0');
        return 1;
    }

    memset(s, 0, 64);
    int i = 62;

    while(n) {
        uint64_t digit = n % 16;
        if(digit < 10) s[i--] = '0' + digit;
        else s[i--] = 'A' + (digit - 10);
        n /= 16;
    }

    puts(s+i+1);
    return strlen(s+i+1);
}
