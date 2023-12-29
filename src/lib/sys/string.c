#include "string.h"

_Bool strequ(const char *s1, const char *s2)
{
        for (size_t i = 0; i <= strlen(s1); ++i)
                if (s1[i] != s2[i])
                        return 0;
        return 1;
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
        for (size_t i = 0; i <= strlen(src); ++i)
                dst[i] = src[i];

        return dst;
}

void strclr(char *s)
{
        char *c = s;
        do {
                *c++ = 0;
        } while (*c != 0);
}
