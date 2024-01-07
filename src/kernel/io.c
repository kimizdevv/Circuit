#include "io.h"

// define a mapping from scancodes to ASCII characters.
const char scancode_to_ascii[] = {
        0,    0,    '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-',  '=',
        '\b', '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[',  ']',
        '\n', 0,    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
        0,    '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0,    '*',
        0,    ' ',  0,   0,   0,   0,   0,   0,   '7', '8', '9', '-', '4',  '5',
        '6',  '+',  0,   '2', '3', 0,   '.', 0,   0,   0, // arrow keys are removed
};
//'6',  '+',  '1', '2', '3', '0', '.', 0,   0,   0 // (previous)

const char shifted_scancode_to_ascii[] = {
        0,    0,    '!',  '@',  '#',  '$',  '%',  '^',  '&',  '*',  '(',  ')',
        '_',  '+',  0,    0x09, 'Q',  'W',  'E',  'R',  'T',  'Y',  'U',  'I',
        'O',  'P',  '{',  '}',  0,    0,    'A',  'S',  'D',  'F',  'G',  'H',
        'J',  'K',  'L',  ':',  '"',  '~',  0,    '|',  'Z',  'X',  'C',  'V',
        'B',  'N',  'M',  '<',  '>',  '?',  0,    '*',  0x0F, ' ',  0,    0,
        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
        0,    0,    0,    0,    0,    0,    0,    0,    0x1B, 0,    0,    0,
        0,    0,    0,    0,    0,    0,    0,    0x0E, 0x1C, 0,    0,    0,
        0,    0,    0,    0,    0,    '?',  0,    0,    0,    0,    0,    0,
        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
        0x1E, 0x1F, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0,
        0,    0,    0,    0,    0,    0,    0,    0x2C
};

inline uint8_t inb(const uint16_t port)
{
        uint8_t data;
        __asm__ volatile("inb %1, %0" : "=a"(data) : "dN"(port));
        return data;
}

inline void outb(const uint16_t port, const uint8_t data)
{
        __asm__ volatile("outb %0, %1" : : "a"(data), "Nd"(port));
}

char ascii_from_scancode(const uint8_t sc, const _Bool shifted)
{
        const char ret = (char)(sc < sizeof scancode_to_ascii ?
                                        scancode_to_ascii[sc] :
                                        0);
        const char shifted_ret = (char)(sc < sizeof shifted_scancode_to_ascii ?
                                                shifted_scancode_to_ascii[sc] :
                                                0);

        return (char)(shifted ? shifted_ret : ret);
}
