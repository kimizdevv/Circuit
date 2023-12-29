#ifndef INCLUDE_TERMINAL_H
#define INCLUDE_TERMINAL_H

#include <stddef.h>
#include <stdint.h>

struct terminal {
        size_t width, height;
        size_t row, column;
        uint8_t color;
        uint16_t *buffer;
};

struct terminal term_init(void);

void term_putentryat(struct terminal term, char c);

void term_putstr(struct terminal term, const char *s);

#endif
