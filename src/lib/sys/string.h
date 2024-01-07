#ifndef INCLUDE_STRING_H
#define INCLUDE_STRING_H

#include <stdint.h>
#include <stddef.h>

_Bool is_chr_visible(char c);

_Bool strequ(const char *s1, const char *s2);
_Bool strequ_upr(const char *s1, const char *s2);

void strtoupr(char *s);

size_t strlen(const char *s);

char *strcpy(char *dst, const char *src);
char *strncpy(char *dst, const char *src, size_t n);

void strclr(char *s);

void *memset(void *s, int c, size_t n);
void *memcpy(void *dst, const void *src, size_t n);

int stoi(const char *s, int *status);
char *itos(int value, char *str, int *status);

#endif
