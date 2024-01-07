#include <stdint.h>

struct stack {
        uint16_t w, h;
        uint8_t **s;
};

struct stack stack(uint16_t w, uint16_t h);
