#include "entry.h"
#include "terminal.h"
#include "shell.h"

extern void kernel_main(void)
{
        struct terminal term = term_init();
        term_putstr(&term, "Hello world!\nYes\n");

        struct shell shell = shell_init(&term);

        for (;;)
                shell_await_command(&shell);
}
