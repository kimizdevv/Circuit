#include "vga.h"

inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg)
{
        return (uint8_t)fg | (uint8_t)(bg << 4);
}

inline uint16_t vga_entry(char c, uint8_t color)
{
        return (uint16_t)c | (uint16_t)(color << 8);
}
