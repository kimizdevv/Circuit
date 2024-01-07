#include "terminal.h"
#include "io.h"
#include "psf.h"
#include "../lib/sys/string.h"

#define FONTSTART _binary_src_fonts_lat9_16_psf_start

extern char FONTSTART[];

struct terminal term_init(struct framebuffer *fb)
{
        // TODO: dont hard-code font dimensions
        const uint32_t WIDTH = fb->width / 9;
        const uint32_t HEIGHT = fb->height / 16;

        struct terminal term = {
                .width = WIDTH,
                .height = HEIGHT,
                .row = 0,
                .column = 0,
                .fg = to_rgba(0xFFa8a8a8),
                .bg = to_rgba(0x000000),
                .fb = fb,
        };

        term_clear(&term);

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
        uint32_t *buf = (uint32_t *)term->fb->bufadr;
        const size_t fw = term->fb->width;

        for (size_t py = 0; py < 16; ++py)
                for (size_t px = 0; px < 9; ++px) {
                        const size_t oldi = getbufidx(x + px, y + py, fw);
                        const size_t newi = getbufidx(x + px, y - 16 + py, fw);

                        buf[newi] = buf[oldi];
                }

        //const size_t oldi = getbufidx(x, y, term->width);
        //const size_t newi = getbufidx(x, y - 1, term->width);
        //term->fb[newi] = term->fb[oldi];
}

static void term_scrolldown(struct terminal *term)
{
        const size_t w = term->width;
        const size_t h = term->height;
        for (size_t y = 1; y <= h; ++y)
                for (size_t x = 0; x < w; ++x)
                        term_move_char_up(term, x * 9, y * 16);
}

static void term_updatecursor(struct terminal *term)
{
        //term_movecursor(term, (uint8_t)term->column, (uint8_t)term->row);
}

void term_newline(struct terminal *term)
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
        if (term->column > 0)
                term->column--;
        else {
                term->column = term->width - 1;
                term->row--;
        }

        term_clrchr_at(term, term->column, term->row);

        term_updatecursor(term);
}

static void term_autowrap(struct terminal *term)
{
        if (term->column == term->width)
                term_newline(term);
}

inline void term_putpx(struct terminal *term, uint32_t x, uint32_t y,
                       uint32_t color)
{
        ((uint32_t *)(term->fb->bufadr))[x + y * term->fb->width] = color;
}

// https://wiki.osdev.org/PC_Screen_Font
void term_putchr_at(struct terminal *term, const unsigned char c,
                    const uint32_t cx, const uint32_t cy)
{
        const struct framebuffer *fb = term->fb;
        const uint32_t scanline = fb->width * fb->bpp;

        const PSF_font *font = (PSF_font *)&FONTSTART;

        if (font->magic != PSF_FONT_MAGIC || font->version != 0)
                return;

        const uint32_t bytesperline = (font->width + 7) / 8;

        const unsigned char *glyph =
                (unsigned char *)&FONTSTART + font->headersize +
                (size_t)(c > 0 && c < font->numglyph ? c : 0) *
                        font->bytesperglyph;

        uint64_t offs =
                ((unsigned long)cy * font->height * scanline) +
                ((unsigned long)cx * (font->width + 1) * sizeof(uint32_t));

        uint16_t x;
        uint16_t y;
        uint64_t line;
        uint32_t mask;
        for (y = 0; y < font->height; y++) {
                line = offs;
                mask = 1 << (font->width - 1);

                for (x = 0; x < font->width; x++) {
                        term_putpx(term, cx * 9 + x, cy * 16 + y,
                                   *((unsigned char *)glyph) & mask ?
                                           term->fg.v :
                                           term->bg.v);

                        /* adjust to the next pixel */
                        mask >>= 1;
                        line += sizeof(uint32_t);
                }

                /* adjust to the next line */
                glyph += bytesperline;
                offs += scanline;
        }
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

        term_putchr_at(term, (unsigned char)c, term->column++, term->row);

        term_autowrap(term);

        term_updatecursor(term);
}

void term_putstr(struct terminal *term, const char *s)
{
        for (size_t i = 0; i < strlen(s); ++i)
                term_putchr(term, s[i]);
}

void term_putstr_rgb(struct terminal *term, const char *s, union rgba color)
{
        const union rgba saved = term->fg;
        term->fg = color;
        term_putstr(term, s);
        term->fg = saved;
}

void term_puterr(struct terminal *term, const char *msg)
{
        term_putstr_rgb(term, "error", from_rgb(230, 80, 80));
        term_putstr(term, ": ");
        term_putstr(term, msg);
}

void term_putcmdprefix(struct terminal *term)
{
        if (term->column != 0)
                term_newline(term);

        term_putstr(term, "> ");
        term_putstr_rgb(term, "$", from_rgb(40, 230, 80));
        term_putstr(term, " ");
}

void term_clrchr_at(struct terminal *term, uint32_t cx, uint32_t cy)
{
        for (uint32_t y = cy * 16; y <= cy * 16 + 16; ++y)
                for (uint32_t x = cx * 9; x <= cx * 9 + 9; ++x)
                        term_putpx(term, x, y, term->bg.v);
}

void term_clear(struct terminal *term)
{
        // TODO: optimize
        for (uint32_t y = 0; y < term->height; ++y)
                for (uint32_t x = 0; x < term->width; ++x)
                        term_clrchr_at(term, x, y);

        term->column = 0;
        term->row = 0;
        term_updatecursor(term);
}
