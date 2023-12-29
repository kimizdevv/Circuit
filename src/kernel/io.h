#ifndef INCLUDE_IO_H
#define INCLUDE_IO_H

#include <stdint.h>

#define KEYBOARD_DATA_PORT 0x60

uint8_t inb(uint16_t port);

void outb(uint16_t port, uint8_t data);

char ascii_from_scancode(uint8_t sc);

#endif
