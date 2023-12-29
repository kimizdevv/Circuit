#include "entry.h"
#include "terminal.h"
#include "shell.h"
#include "vga.h"

#define PUTSTR(S) term_putstr(term, S)

static void print_welcome_message(struct terminal *term)
{
        term->color = VGA_COLOR_WHITE;
        PUTSTR("=============================================================\n");
        PUTSTR("==                  Welcome to CircuitOS                   ==\n");
        PUTSTR("=============================================================\n");
        PUTSTR("\n");
        term->color = VGA_COLOR_LIGHT_GREY;
}

extern void kernel_main(void)
{
        struct terminal term_o = term_init();
        struct terminal *term = &term_o;

        PUTSTR("Initiating shell...\n");

        struct shell shell = shell_init(term);

        PUTSTR("Success..\n");
        PUTSTR("\n");

        print_welcome_message(term);

        for (;;)
                shell_await_command(&shell);
}
