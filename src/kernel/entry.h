#ifndef INCLUDE_ENTRY_H
#define INCLUDE_ENTRY_H

#define CIRCUIT_SYSTEM_VERSION "0.1.0-DEV"

#include <stdint.h>

struct framebuffer {
        uintptr_t bufadr;
        uint32_t pitch;
        uint32_t width;
        uint32_t height;
        uint32_t bpp;
        uint8_t type;
        uint16_t ncol;
};

void kernel_main(unsigned long magic, unsigned long addr);

#endif
