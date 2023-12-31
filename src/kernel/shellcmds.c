#include "shellcmds.h"
#include "terminal.h"
#include "../lib/sys/string.h"

#define UNUSED(v) ((void)v)
#define PUTSTR(S) term_putstr(shell->term, S)

int shcmd_test(struct shell *shell, const char args[MAX_ARGS][MAX_ARG_LEN],
               size_t argc)
{
        UNUSED(args);
        UNUSED(argc);

        PUTSTR("OK!");

        return 0;
}

int shcmd_welcome(struct shell *shell, const char args[MAX_ARGS][MAX_ARG_LEN],
                  size_t argc)
{
        UNUSED(args);
        UNUSED(argc);

        union rgba saved = shell->term->fg;

        shell->term->fg = to_rgba(0xa8a8a8);
        PUTSTR("=============================================================\n");
        PUTSTR("==                        CircuitOS                        ==\n");
        PUTSTR("== ------------------------------------------------------- ==\n");
        PUTSTR("==            An operating system aiming to be             ==\n");
        PUTSTR("==      extremely lightweight, performant and reliable     ==\n");
        PUTSTR("==                    Fully open-source                    ==\n");
        PUTSTR("== ------------------------------------------------------- ==\n");
        PUTSTR("==                 Made as a hobby project                 ==\n");
        PUTSTR("==             Designed to be used as a utility            ==\n");
        PUTSTR("== ------------------------------------------------------- ==\n");
        PUTSTR("==  NOT INTENDED FOR ENTERTAINMENT. ONLY FOR POWER USERS!  ==\n");
        PUTSTR("==                                                         ==\n");
        PUTSTR("==                                        kimizdevv@github ==\n");
        PUTSTR("=============================================================\n");
        PUTSTR("\n");
        shell->term->fg = saved;

        return 0;
}

int shcmd_help(struct shell *shell, const char args[MAX_ARGS][MAX_ARG_LEN],
               size_t argc)
{
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
