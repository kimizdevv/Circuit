#include "terminal.h"
#include "vga.h"
#include "../lib/sys/string.h"

struct terminal term_init(void)
{
        const size_t VGA_WIDTH = 80;
        const size_t VGA_HEIGHT = 25;

        struct terminal term = {
                .width = VGA_WIDTH,
                .height = VGA_HEIGHT,
                .row = 0,
                .column = 0,
                .color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK),
                .buffer = (uint16_t *)0xB8000,
        };

        for (size_t y = 0; y < term.height; ++y)
                for (size_t x = 0; x < term.width; ++x)
                        term_putentryat(&term, ' ');

        return term;
}

void term_putentryat(struct terminal *term, const char c)
{
        const size_t idx = term->row * term->width + term->column;
        term->buffer[idx] = vga_entry(c, term->color);
}

static void term_newline(struct terminal *term)
{
        term->column = 0;
        term->row++;
}

static void term_autowrap(struct terminal *term)
{
        if (term->column == term->width)
                term_newline(term);
}

static _Bool term_checkspecial(struct terminal *term, const char c)
{
        switch (c) {
        case '\n':
                term_newline(term);
                return 1;
        default:
                return 0;
        }
}

void term_putstr(struct terminal *term, const char *s)
{
        for (size_t i = 0; i < strlen(s); ++i) {
                const char c = s[i];

                if (term_checkspecial(term, c))
                        continue;

                term_autowrap(term);

                term_putentryat(term, c);
                term->column++;
        }
}