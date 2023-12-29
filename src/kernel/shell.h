#ifndef INCLUDE_SHELL_H
#define INCLUDE_SHELL_H

#include <stddef.h>

#define MAX_CMD_LEN 128

struct shell {
        // TODO: a single shell shouldn't be limited to just one terminal
        struct terminal *term;
};

struct shell shell_init(struct terminal *term);

void shell_await_input(char *c);

void shell_await_input_str(struct shell *shell, char *buf, size_t max,
                           _Bool print);

void shell_await_command(struct shell *shell);

int shell_process_command(struct shell *shell, const char *input);

#endif
