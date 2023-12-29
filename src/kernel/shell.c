#include "shell.h"
#include "terminal.h"
#include "io.h"

struct shell shell_init(struct terminal *term)
{
        struct shell shell = {
                .term = term,
        };
        return shell;
}

void shell_await_input(char *c)
{
        for (;;) {
                for (; !(inb(0x64) & 1);)
                        ;

                char temp = ascii_from_scancode(inb(KEYBOARD_DATA_PORT));
                if (temp == 0)
                        continue;

                *c = temp;
                break;
        }
}

void shell_await_input_str(struct shell *shell, char *buf, size_t max,
                           _Bool print)
{
        for (size_t i = 0; i < max; ++i) {
                char c;
                shell_await_input(&c);
                buf[i] = c;

                if (print)
                        term_putchr(shell->term, c);

                if (c == '\n')
                        break;
        }
}

void shell_await_command(struct shell *shell)
{
        char cmd[MAX_CMD_LEN] = { 0 };
        term_putstr(shell->term, "> $ ");
        shell_await_input_str(shell, cmd, MAX_CMD_LEN, 1);
        shell_process_command(cmd);
}

int shell_process_command(const char *input)
{
        (void)input; // unused
        return 0;
}
