#include "terminal.h"
#include "vga.h"
#include "io.h"
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
                        term_putentry(&term, ' ');

        return term;
}

static inline size_t getbufidx(const size_t x, const size_t y,
                               const size_t width)
{
        return y * width + x;
}

static inline size_t term_getbufidx(struct terminal *term)
{
        return getbufidx(term->column, term->row, term->width);
}

void term_movecursor(struct terminal *term, const uint8_t x, const uint8_t y)
{
        const uint16_t VGA_PORT_CTRL = 0x3D4;
        const uint16_t VGA_PORT_DATA = 0x3D5;
        const uint8_t VGA_CURSOR_HIGH = 0x0E;
        const uint8_t VGA_CURSOR_LOW = 0x0F;

        const uint16_t pos = (uint16_t)getbufidx(x, y, (uint8_t)term->width);

        // set high cursor byte
        outb(VGA_PORT_CTRL, VGA_CURSOR_HIGH);
        outb(VGA_PORT_DATA, pos >> 8);

        // set low cursor byte
        outb(VGA_PORT_CTRL, VGA_CURSOR_LOW);
        outb(VGA_PORT_DATA, pos & 0xFF);
}

static void term_move_char_up(struct terminal *term, const size_t x,
                              const size_t y)
{
        const size_t oldi = getbufidx(x, y, term->width);
        const size_t newi = getbufidx(x, y - 1, term->width);
        const uint16_t *old = &(term->buffer[oldi]);
        const uint16_t *new = &(term->buffer[newi]);
        term->buffer[newi] = term->buffer[oldi];

        //*old = *new;
}

static void term_scrolldown(struct terminal *term)
{
        for (size_t y = 1; y <= term->height; ++y)
                for (size_t x = 0; x < term->width; ++x)
                        term_move_char_up(term, x, y);
}

static void term_updatecursor(struct terminal *term)
{
        term_movecursor(term, (uint8_t)term->column, (uint8_t)term->row);
}

static void term_newline(struct terminal *term)
{
        term->column = 0;

        if (term->row + 1 == term->height)
                term_scrolldown(term);
        else
                term->row++;

        term_updatecursor(term);
}

static void term_backspace(struct terminal *term)
{
        term->buffer[term_getbufidx(term) - 1] = vga_entry(' ', term->color);

        if (term->column > 0)
                term->column--;
        else if (term->column == 0) {
                term->column = term->width;
                term->row--;
        }

        term_updatecursor(term);
}

static void term_autowrap(struct terminal *term)
{
        if (term->column == term->width)
                term_newline(term);
}

void term_putentry(struct terminal *term, const char c)
{
        term->buffer[term_getbufidx(term)] = vga_entry(c, term->color);
}

void term_putchr(struct terminal *term, const char c)
{
        if (c == '\n') {
                term_newline(term);
                return;
        }

        if (c == '\b') {
                term_backspace(term);
                return;
        }

        term_putentry(term, c);
        term->column++;

        term_autowrap(term);

        term_updatecursor(term);
}

void term_putstr(struct terminal *term, const char *s)
{
        for (size_t i = 0; i < strlen(s); ++i)
                term_putchr(term, s[i]);
}

void term_puterr(struct terminal *term, const char *msg)
{
        const uint8_t saved = (uint8_t)term->color;

        term->color = VGA_COLOR_LIGHT_RED;
        term_putstr(term, "ERROR");
        term->color = saved;
        term_putstr(term, ": ");
        term_putstr(term, msg);
}

void term_putcmdprefix(struct terminal *term)
{
        if (term->column != 0)
                term_newline(term);

        const uint8_t saved = (uint8_t)term->color;

        term_putstr(term, "> ");
        term->color = VGA_COLOR_LIGHT_GREEN;
        term_putchr(term, '$');
        term->color = saved;
        term_putstr(term, " ");
}
