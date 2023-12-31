#include "rgb.h"

inline union rgba to_rgba(uint32_t v)
{
        return (union rgba){ .v = v };
}

inline union rgba from_rgba(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
        return (union rgba){ .r = r, .g = g, .b = b, .a = a };
}

inline union rgba from_rgb(uint8_t r, uint8_t g, uint8_t b)
{
        return from_rgba(r, g, b, 0xFF);
}
