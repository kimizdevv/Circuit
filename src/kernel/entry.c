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
        PUTSTR("== ------------------------------------------------------- ==\n");
        PUTSTR("==            An operating system aiming to be             ==\n");
        PUTSTR("==     extremely lightweight, performant and reliable      ==\n");
        PUTSTR("==                    Fully open-source                    ==\n");
        PUTSTR("== ------------------------------------------------------- ==\n");
        PUTSTR("==                 Made as a hobby project                 ==\n");
        PUTSTR("==            Designed to be used as an utility            ==\n");
        PUTSTR("== ------------------------------------------------------- ==\n");
        PUTSTR("==  NOT INTENDED FOR ENTERTAINMENT. ONLY FOR POWER USERS!  ==\n");
        PUTSTR("==                                                         ==\n");
        PUTSTR("==                                        kimizdevv@github ==\n");
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

        PUTSTR("Success.\n");
        PUTSTR("\n");

        print_welcome_message(term);

        for (;;)
                shell_await_command(&shell);
}
