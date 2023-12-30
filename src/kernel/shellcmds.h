#ifndef INCLUDE_SHELLCMDS_H
#define INCLUDE_SHELLCMDS_H

#include "shell.h"

int shcmd_test(struct shell *shell, const char args[MAX_ARGS][MAX_ARG_LEN],
               size_t argc);

int shcmd_lengthy(struct shell *shell, const char args[MAX_ARGS][MAX_ARG_LEN],
                  size_t argc);

int shcmd_echo(struct shell *shell, const char args[MAX_ARGS][MAX_ARG_LEN],
               size_t argc);

int shcmd_clear(struct shell *shell, const char args[MAX_ARGS][MAX_ARG_LEN],
                size_t argc);

#endif
