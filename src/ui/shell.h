#ifndef SHELL_H
#define SHELL_H

#include <sys/types.h>
#include "../core/heap_manager.h"

/**
 * @file shell.h
 * @brief Interactive command-line shell interface
 * 
 * Provides REPL (Read-Eval-Print Loop) for interactive command execution.
 */

/**
 * @brief Run interactive shell mode
 * @param initial_size Initial heap size (0 = ask user)
 * 
 * Enters REPL loop, processes commands until 'exit' or EOF.
 */
void run_interactive_shell(size_t initial_size);

/**
 * @brief Run batch mode from file
 * @param filename Path to script file
 * @return 0 on success, non-zero on error
 */
int run_batch_mode(const char* filename);

#endif /* SHELL_H */
