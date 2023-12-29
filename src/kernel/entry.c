#include "entry.h"
#include "terminal.h"

extern void kernel_main(void)
{
        struct terminal term = term_get();
        term_putentryat(term, 'H', 1, 1);
        term_putstr(term, "Hello world!", 0, 0);
}
