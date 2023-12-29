#include "terminal.h"
#include "vga.h"
#include "../lib/sys/string.h"

struct terminal term_get(void)
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
                        term_putentryat(term, ' ', x, y);

        return term;
}

void term_putentryat(struct terminal term, const char c, const size_t x,
                     const size_t y)
{
        term.buffer[y * term.width + x] = vga_entry(c, term.color);
}

void term_putstr(struct terminal term, const char *s, const size_t x,
                 const size_t y)
{
        // TODO: add text wrapping

        for (size_t i = 0; i < strlen(s); ++i)
                term_putentryat(term, s[i], x + i, y);
}
