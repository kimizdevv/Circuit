#ifndef INCLUDE_TERMINAL_H
#define INCLUDE_TERMINAL_H

#include "vga.h"
#include <stddef.h>
#include <stdint.h>

struct terminal {
        int backspace_limit;
        size_t width, height;
        size_t row, column;
        uint8_t color;
        uint16_t *buffer;
};

struct terminal term_init(void);

void term_clear(struct terminal *term);

void term_movecursor(struct terminal *term, uint8_t x, uint8_t y);

void term_putentry(struct terminal *term, char c);

void term_putchr(struct terminal *term, char c);

void term_putstr(struct terminal *term, const char *s);

void term_putstr_rgb(struct terminal *term, const char *s,
                     enum vga_color color);

void term_puterr(struct terminal *term, const char *msg);

void term_putcmdprefix(struct terminal *term);

#endif
