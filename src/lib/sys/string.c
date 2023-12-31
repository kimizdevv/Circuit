#include "string.h"
#include "math.h"
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

void *memcpy(void *dst, const void *src, const size_t n)
{
        size_t i;

        /*
        * memcpy does not support overlapping buffers, so always do it
        * forwards. (Don't change this without adjusting memmove.)
        *
        * For speedy copying, optimize the common case where both pointers
        * and the length are word-aligned, and copy word-at-a-time instead
        * of byte-at-a-time. Otherwise, copy by bytes.
        *
        * The alignment logic below should be portable. We rely on
        * the compiler to be reasonably intelligent about optimizing
        * the divides and modulos out. Fortunately, it is.
        */

        if ((uintptr_t)dst % sizeof(long) == 0 &&
            (uintptr_t)src % sizeof(long) == 0 && n % sizeof(long) == 0) {
                long *d = dst;
                const long *s = src;

                for (i = 0; i < n / sizeof(long); i++) {
                        d[i] = s[i];
                }
        } else {
                char *d = dst;
                const char *s = src;

                for (i = 0; i < n; i++) {
                        d[i] = s[i];
                }
        }

        return dst;
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

char *itos(int value, char *str, int *status)
{
        const _Bool neg = value < 0;
        if (neg)
                value *= -1;

        size_t len = neg;
        for (;;) {
                const int d = value % 10;
                value /= 10;
                str[len++] = (char)(d + '0');

                if (value == 0)
                        break;
        }

        if (len > 1)
                for (size_t i = neg; i < len / 2 + neg; ++i) {
                        const size_t ri = len - !neg - i;
                        if (ri == i)
                                continue;
                        const char t = str[i];
                        str[i] = str[ri];
                        str[ri] = t;
                }

        if (neg)
                str[0] = '-';

        str[len] = 0;

        *status = 0;
        return str;
}
