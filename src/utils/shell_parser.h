#ifndef SHELL_PARSER_H
#define SHELL_PARSER_H

#include <sys/types.h>
#include <stddef.h>

/**
 * @file shell_parser.h
 * @brief Interactive shell command parser
 * 
 * Parses user commands in the interactive shell mode.
 * Supports commands like: init, alloc, free, show, stat, frag, algo, reset, help, exit
 */

/**
 * @enum CommandType
 * @brief Types of shell commands
 */
typedef enum {
    CMD_INIT,       /**< init <size> - Initialize heap */
    CMD_ALLOC,      /**< alloc <PID>:<size> - Allocate memory */
    CMD_FREE,       /**< free <PID> - Free memory */
    CMD_SHOW,       /**< show - Display memory map */
    CMD_STAT,       /**< stat - Display statistics */
    CMD_FRAG,       /**< frag - Display fragmentation report */
    CMD_ALGO,       /**< algo <ff|nf|bf|wf> - Set algorithm */
    CMD_EXPORT,     /**< export <file> - Export memory state to JSON */
    CMD_RESET,      /**< reset - Reset heap to initial state */
    CMD_HELP,       /**< help - Display help message */
    CMD_EXIT,       /**< exit/quit - Exit shell */
    CMD_EMPTY,      /**< Empty line (just Enter) */
    CMD_UNKNOWN     /**< Invalid command */
} CommandType;

/**
 * @struct ParsedCommand
 * @brief Represents a parsed command with arguments
 */
typedef struct {
    CommandType type;       /**< Command type */
    char arg1[64];          /**< First argument (PID, algorithm, etc.) */
    size_t arg2;            /**< Second argument (size) */
} ParsedCommand;

/**
 * @brief Parse a command string
 * @param input Command string from user
 * @return ParsedCommand structure
 */
ParsedCommand parse_shell_command(const char* input);

/**
 * @brief Get command name as string
 * @param type Command type
 * @return Command name string
 */
const char* get_command_name(CommandType type);

#endif /* SHELL_PARSER_H */
