#include "entry.h"
#include "terminal.h"

extern void kernel_main(void)
{
        struct terminal term = term_get();
        term_putstr(
                term,
                "Hello world! This is quite a bit of a longer one to test the text wrapping capabilities.");
}
