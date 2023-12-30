#include "shellcmds.h"
#include "terminal.h"
#include "../lib/sys/string.h"

#define UNUSED(v) ((void)v)

int shcmd_test(struct shell *shell, const char args[MAX_ARGS][MAX_ARG_LEN],
               size_t argc)
{
        UNUSED(args);
        UNUSED(argc);

        term_putstr(shell->term, "OK!");

        return 0;
}

int shcmd_lengthy(struct shell *shell, const char args[MAX_ARGS][MAX_ARG_LEN],
                  size_t argc)
{
        int status = 0;
        const int nrep = argc > 1 ? stoi(args[1], &status) : 1;

        if (status < 0) {
                term_puterr(shell->term, "invalid number.");
                return -1;
        }

        if (nrep < 0) {
                term_puterr(shell->term, "negative number not allowed.");
                return -2;
        }

        for (int i = 0; i < nrep; ++i)
                for (int c = '!'; c <= '~'; ++c)
                        term_putchr(shell->term, (char)c);

        return 0;
}

int shcmd_echo(struct shell *shell, const char args[MAX_ARGS][MAX_ARG_LEN],
               size_t argc)
{
        for (size_t i = 1; i < argc; ++i) {
                term_putstr(shell->term, args[i]);
                if (i != argc)
                        term_putchr(shell->term, ' ');
        }

        return 0;
}

int shcmd_clear(struct shell *shell, const char args[MAX_ARGS][MAX_ARG_LEN],
                size_t argc)
{
        UNUSED(args);
        UNUSED(argc);

        term_clear(shell->term);

        return 0;
}
