#include "string.h"
#include <stdint.h>

_Bool is_chr_visible(const char c)
{
        return c >= 32 && c <= 126;
}

_Bool strequ(const char *s1, const char *s2)
{
        while (*s1 && (*s1 == *s2))
                s1++, s2++;

        return *(unsigned char *)s1 == *(unsigned char *)s2;
}

static char let_upr(const char let)
{
        return (char)((let >= 'a' && let <= 'z') ? let - 32 : let);
}
_Bool strequ_upr(const char *s1, const char *s2)
{
        while (*s1 && (let_upr(*s1) == let_upr(*s2)))
                s1++, s2++;

        return *(unsigned char *)s1 == *(unsigned char *)s2;
}

void strtoupr(char *s)
{
        while (*s) {
                *s = let_upr(*s);
                *s++;
        }
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
        *dst = 0;

        return dst;
}

char *strncpy(char *dst, const char *src, const size_t n)
{
        size_t i = 0;
        for (; i < n; ++i)
                dst[i] = src[i];
        dst[i] = 0;

        return dst;
}

void strclr(char *s)
{
        do {
                *s++ = 0;
        } while (*s != 0);
}

void *memset(void *s, const int c, const size_t n)
{
        uint8_t *p = s;
        for (size_t i = 0; i < n; ++i, *p++)
                *p = (uint8_t)c;
        return p;
}

int stoi(const char *s, int *status)
{
        int res = 0;
        *status = 0;
        _Bool neg = 0;
        _Bool found_num = 0;

        while (*s) {
                const char c = *s++;

                if (!found_num && c == '-') {
                        neg = !neg;
                        continue;
                }

                if (c < '0' || c > '9') {
                        *status = -1;
                        break;
                }

                found_num = 1;

                const int d = c - '0';
                res = res * 10 + d;
        }

        if (!found_num)
                *status = -2;

        return neg ? -res : res;
}
