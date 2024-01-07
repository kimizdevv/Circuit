#include "stack.h"

struct stack stack(const uint16_t w, const uint16_t h)
{
        uint8_t st[h][w];
        uint8_t *st_p = *st;
        uint8_t **st_p2 = &st_p;

        struct stack st_o = {
                .w = w,
                .h = h,
                .s = st_p2,
        };
        return st_o;
}
