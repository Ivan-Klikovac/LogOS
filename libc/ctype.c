#include <ctype.h>

int isalnum(int arg)
{
    unsigned char c = (unsigned char) arg;
    return ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'));
}

int isalpha(int arg)
{
    unsigned char c = (unsigned char) arg;
    return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'));
}

int islower(int arg)
{
    unsigned char c = (unsigned char) arg;
    return (c >= 'a' && c <= 'z');
}

int isupper(int arg)
{
    unsigned char c = (unsigned char) arg;
    return (c >= 'A' && c <= 'Z');
}

int isdigit(int arg)
{
    unsigned char c = (unsigned char) arg;
    return (c >= '0' && c <= '9');
}

int isxdigit(int arg)
{
    unsigned char c = (unsigned char) arg;
    return (isdigit(c) || (c >= 'a' && c <= 'f'));
}

int iscntrl(int arg)
{
    unsigned char c = (unsigned char) arg;
    return ((c >= 0 && c <= 31) || c == 127);
}

int isgraph(int arg)
{
    unsigned char c = (unsigned char) arg;
    return (c >= 33 && c <= 126);
}

int isspace(int arg)
{
    unsigned char c = (unsigned char) arg;
    return ((c >= 9 && c <= 13) || c == 32);
}

int isblank(int arg)
{
    unsigned char c = (unsigned char) arg;
    return (c == 9 || c == 32);
}

int isprint(int arg)
{
    unsigned char c = (unsigned char) arg;
    return (c >= 32 && c <= 126);
}

int ispunct(int arg)
{
    unsigned char c = (unsigned char) arg;
    return ((c >= 33 && c <= 47) || (c >= 58 && c <= 64) || (c >= 91 && c <= 96) || (c >= 123 && c <= 126));
}

int tolower(int arg)
{
    unsigned char c = (unsigned char) arg;
    if(isupper(c)) return c + ('a' - 'A');
    return arg;
}

int toupper(int arg)
{
    unsigned char c = (unsigned char) arg;
    if(islower(c)) return c - ('a' - 'A');
    return arg;
}