#ifndef INCLUDE_STRING_H
#define INCLUDE_STRING_H

#include <stddef.h>

_Bool strequ(const char *s1, const char *s2);

size_t strlen(const char *s);

char *strcpy(char *dst, const char *src);

void strclr(char *s);

#endif
