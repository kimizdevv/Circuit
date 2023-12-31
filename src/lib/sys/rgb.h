#ifndef INCLUDE_RGB_H
#define INCLUDE_RGB_H

#include <stdint.h>

union rgba {
        struct {
                uint8_t b, g, r, a;
        };
        uint32_t v;
};

union rgba to_rgba(uint32_t v);

union rgba from_rgba(uint8_t r, uint8_t g, uint8_t b, uint8_t a);

union rgba from_rgb(uint8_t r, uint8_t g, uint8_t b);

#endif
