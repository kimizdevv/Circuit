#include "io.h"

// define a mapping from scancodes to ASCII characters.
const char scancode_to_ascii[] = {
        0,    0,    '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-',  '=',
        '\b', '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[',  ']',
        '\n', 0,    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
        0,    '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0,    '*',
        0,    ' ',  0,   0,   0,   0,   0,   0,   '7', '8', '9', '-', '4',  '5',
        '6',  '+',  '1', '2', '3', '0', '.', 0,   0,   0
};

inline uint8_t inb(uint16_t port)
{
        uint8_t data;
        __asm__ volatile("inb %1, %0" : "=a"(data) : "dN"(port));
        return data;
}

inline void outb(uint16_t port, uint8_t data)
{
        __asm__ volatile("outb %0, %1" : : "a"(data), "Nd"(port));
}

char ascii_from_scancode(uint8_t sc)
{
        return (char)(sc < sizeof scancode_to_ascii ? scancode_to_ascii[sc] :
                                                      0);
}
