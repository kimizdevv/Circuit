#include "string.h"

_Bool strequ(const char *s1, const char *s2)
{
        while (*s1 && (*s1 == *s2)) {
                s1++;
                s2++;
        }

        return *(unsigned char *)s1 == *(unsigned char *)s2;
}

size_t strlen(const char *s)
{
        size_t len = 0;

        while (s[len])
                len++;

        return len;
}

char *strcpy(char *dst, const char *src)
{
        do {
                *dst++ = *src++;
        } while (*src != 0);

        return dst;
}

void strclr(char *s)
{
        do {
                *s++ = 0;
        } while (*s != 0);
}
