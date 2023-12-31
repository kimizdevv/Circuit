#ifndef INCLUDE_TERMINAL_H
#define INCLUDE_TERMINAL_H

#include "entry.h"
#include "../lib/sys/rgb.h"
#include <stddef.h>
#include <stdint.h>

struct terminal {
        int backspace_limit;
        uint32_t width, height;
        uint32_t row, column;
        union rgba fg, bg;
        struct framebuffer *fb;
};

struct terminal term_init(struct framebuffer *fb);

void term_movecursor(struct terminal *term, uint8_t x, uint8_t y);

void term_newline(struct terminal *term);

void term_putpx(struct terminal *term, uint32_t x, uint32_t y, uint32_t color);

void term_putchr_at(struct terminal *term, unsigned short int c, uint32_t cx,
                    uint32_t cy);

void term_putchr(struct terminal *term, unsigned short int c);

void term_putstr(struct terminal *term, const char *s);

void term_putstr_rgb(struct terminal *term, const char *s, union rgba color);

void term_puterr(struct terminal *term, const char *msg);

void term_putcmdprefix(struct terminal *term);

void term_clrchr_at(struct terminal *term, uint32_t cx, uint32_t cy);

void term_clear(struct terminal *term);

#endif
