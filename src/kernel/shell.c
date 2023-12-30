#include "shell.h"
#include "terminal.h"
#include "io.h"
#include "../lib/sys/string.h"

#define MAX_ARGS 8
#define MAX_ARG_LEN 64

static _Bool shift_pressed = 0;

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

                const uint8_t sc = inb(KEYBOARD_DATA_PORT);

                // check shift
                if (shift_pressed == 1 && (sc & 0x80) && (sc & 0x7F) == 42) {
                        shift_pressed = 0;
                        continue;
                }
                if (sc == 42) {
                        shift_pressed = 1;
                        continue;
                }

                const char temp = ascii_from_scancode(sc, shift_pressed);
                if (temp == 0)
                        continue;

                *c = temp;
                break;
        }
}

void shell_await_input_str(struct shell *shell, char *buf, size_t max,
                           _Bool print)
{
        for (size_t i = 0; i < max;) {
                char c;
                shell_await_input(&c);

                const _Bool is_bs = c == '\b';

                if (is_bs && i > 0) {
                        buf[--i] = 0;
                        term_putchr(shell->term, '\b');
                } else if (!is_bs && print)
                        term_putchr(shell->term, c);

                if (is_bs)
                        continue;

                if (c == '\n')
                        break;

                buf[i++] = c;
        }
}

void shell_await_command(struct shell *shell)
{
        char cmd[MAX_CMD_LEN] = { 0 };
        term_putcmdprefix(shell->term);
        shell_await_input_str(shell, cmd, MAX_CMD_LEN, 1);
        shell_process_command(shell, cmd);
}

static void interpret_command(const char *input,
                              char args[MAX_ARGS][MAX_ARG_LEN],
                              size_t *arg_count)
{
        _Bool enclosed = 0; // is put in single quotes ''

        char buf[MAX_ARG_LEN] = { 0 };
        size_t len = 0;

        for (size_t i = 0; i <= strlen(input); ++i) {
                const char c = input[i];

                if (c == '\0' || (c == ' ' && !enclosed)) {
                        if (len == 0)
                                continue;

                        strcpy(args[(*arg_count)++], buf);
                        strclr(buf);
                        len = 0;

                        continue;
                }

                if (c == '\'')
                        enclosed = !enclosed;

                buf[len++] = c;
        }
}

int shell_process_command(struct shell *shell, const char *input)
{
        if (strlen(input) == 0)
                return 0;

        struct terminal *term = shell->term;

        char args[MAX_ARGS][MAX_ARG_LEN] = { 0 };
        size_t arg_count = 0;
        interpret_command(input, args, &arg_count);

        const char *cmd = args[0];

        if (strequ(cmd, "test")) {
                term_putstr(term, "OK!");
        } else if (strequ(cmd, "lengthy")) {
                for (int c = '!'; c <= '~'; ++c)
                        term_putchr(term, (char)c);
        } else if (strequ(cmd, "echo")) {
                for (size_t i = 1; i < arg_count; ++i) {
                        term_putstr(term, args[i]);
                        if (i != arg_count)
                                term_putchr(term, ' ');
                }
        } else if (strequ(cmd, "clear")) {
                term_clear(term);
        } else {
                term_puterr(term, "invalid command: '");
                term_putstr(term, cmd);
                term_putstr(term, "'.\n");
        }

        return 0;
}
