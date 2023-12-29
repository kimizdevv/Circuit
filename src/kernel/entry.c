#include "entry.h"
#include "terminal.h"

extern void kernel_main(void)
{
        struct terminal term = term_init();
        term_putstr(&term, "Hello world!\nYes");
}
