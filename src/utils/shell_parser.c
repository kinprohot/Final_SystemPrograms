/* 
 * Shell Command Parser
 * 
 * Parses user input in interactive shell mode.
 * 
 * Date: December 20, 2025
 */

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "shell_parser.h"

/**
 * @brief Trim whitespace from both ends of a string (in-place)
 */
static void trim(char* str) {
    if (!str) return;
    
    /* Trim leading whitespace */
    char* start = str;
    while (*start && isspace((unsigned char)*start)) {
        start++;
    }
    
    /* Trim trailing whitespace */
    char* end = start + strlen(start) - 1;
    while (end > start && isspace((unsigned char)*end)) {
        end--;
    }
    
    /* Write back */
    size_t len = (size_t)(end - start + 1);
    memmove(str, start, len);
    str[len] = '\0';
}

/**
 * @brief Convert string to lowercase (in-place)
 */
static void to_lower(char* str) {
    if (!str) return;
    for (size_t i = 0; str[i]; i++) {
        str[i] = (char)tolower((unsigned char)str[i]);
    }
}

/**
 * @brief Parse command string into ParsedCommand structure
 */
ParsedCommand parse_shell_command(const char* input) {
    ParsedCommand cmd;
    cmd.type = CMD_UNKNOWN;
    cmd.arg1[0] = '\0';
    cmd.arg2 = 0;
    
    if (!input) {
        cmd.type = CMD_EMPTY;
        return cmd;
    }
    
    /* Create mutable copy */
    char buffer[256];
    strncpy(buffer, input, sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0';
    
    /* Trim whitespace */
    trim(buffer);
    
    /* Empty line */
    if (buffer[0] == '\0') {
        cmd.type = CMD_EMPTY;
        return cmd;
    }
    
    /* Comment line */
    if (buffer[0] == '#') {
        cmd.type = CMD_EMPTY;
        return cmd;
    }
    
    /* Extract command word */
    char command[32];
    char args[224];
    
    /* Split command and arguments */
    const char* space_pos = strchr(buffer, ' ');
    if (space_pos) {
        size_t cmd_len = (size_t)(space_pos - buffer);
        if (cmd_len >= sizeof(command)) cmd_len = sizeof(command) - 1;
        strncpy(command, buffer, cmd_len);
        command[cmd_len] = '\0';
        
        strncpy(args, space_pos + 1, sizeof(args) - 1);
        args[sizeof(args) - 1] = '\0';
        trim(args);
    } else {
        strncpy(command, buffer, sizeof(command) - 1);
        command[sizeof(command) - 1] = '\0';
        args[0] = '\0';
    }
    
    to_lower(command);
    
    /* Parse commands */
    
    /* exit, quit, q */
    if (strcmp(command, "exit") == 0 || strcmp(command, "quit") == 0 || strcmp(command, "q") == 0) {
        cmd.type = CMD_EXIT;
        return cmd;
    }
    
    /* help, ? */
    if (strcmp(command, "help") == 0 || strcmp(command, "?") == 0) {
        cmd.type = CMD_HELP;
        return cmd;
    }
    
    /* show [simple|detail], s */
    if (strcmp(command, "show") == 0 || strcmp(command, "s") == 0) {
        cmd.type = CMD_SHOW;
        if (args[0] != '\0') {
            strncpy(cmd.arg1, args, sizeof(cmd.arg1) - 1);
            cmd.arg1[sizeof(cmd.arg1) - 1] = '\0';
            to_lower(cmd.arg1);
            trim(cmd.arg1);
        }
        return cmd;
    }
    
    /* stat, st */
    if (strcmp(command, "stat") == 0 || strcmp(command, "st") == 0) {
        cmd.type = CMD_STAT;
        return cmd;
    }
    
    /* frag, fr */
    if (strcmp(command, "frag") == 0 || strcmp(command, "fr") == 0) {
        cmd.type = CMD_FRAG;
        return cmd;
    }
    
    /* reset */
    if (strcmp(command, "reset") == 0) {
        cmd.type = CMD_RESET;
        return cmd;
    }
    
    /* export <filename> */
    if (strcmp(command, "export") == 0) {
        cmd.type = CMD_EXPORT;
        if (args[0] != '\0') {
            strncpy(cmd.arg1, args, sizeof(cmd.arg1) - 1);
            cmd.arg1[sizeof(cmd.arg1) - 1] = '\0';
            trim(cmd.arg1);
        }
        return cmd;
    }
    
    /* init <size> */
    if (strcmp(command, "init") == 0) {
        cmd.type = CMD_INIT;
        if (args[0] != '\0') {
            long size = strtol(args, NULL, 10);
            if (size > 0) {
                cmd.arg2 = (size_t)size;
            }
        }
        return cmd;
    }
    
    /* alloc <PID>:<size> or alloc <PID> <size> */
    if (strcmp(command, "alloc") == 0 || strcmp(command, "a") == 0) {
        cmd.type = CMD_ALLOC;
        if (args[0] != '\0') {
            /* Try format: PID:size */
            char* colon = strchr(args, ':');
            if (colon) {
                size_t pid_len = (size_t)(colon - args);
                if (pid_len >= sizeof(cmd.arg1)) pid_len = sizeof(cmd.arg1) - 1;
                strncpy(cmd.arg1, args, pid_len);
                cmd.arg1[pid_len] = '\0';
                
                long size = strtol(colon + 1, NULL, 10);
                if (size > 0) {
                    cmd.arg2 = (size_t)size;
                }
            } else {
                /* Try format: PID size (space-separated) */
                char* space = strchr(args, ' ');
                if (space) {
                    size_t pid_len = (size_t)(space - args);
                    if (pid_len >= sizeof(cmd.arg1)) pid_len = sizeof(cmd.arg1) - 1;
                    strncpy(cmd.arg1, args, pid_len);
                    cmd.arg1[pid_len] = '\0';
                    
                    long size = strtol(space + 1, NULL, 10);
                    if (size > 0) {
                        cmd.arg2 = (size_t)size;
                    }
                } else {
                    /* Only PID provided */
                    strncpy(cmd.arg1, args, sizeof(cmd.arg1) - 1);
                    cmd.arg1[sizeof(cmd.arg1) - 1] = '\0';
                }
            }
        }
        return cmd;
    }
    
    /* free <PID> */
    if (strcmp(command, "free") == 0 || strcmp(command, "f") == 0) {
        cmd.type = CMD_FREE;
        if (args[0] != '\0') {
            strncpy(cmd.arg1, args, sizeof(cmd.arg1) - 1);
            cmd.arg1[sizeof(cmd.arg1) - 1] = '\0';
            trim(cmd.arg1);
        }
        return cmd;
    }
    
    /* algo <ff|nf|bf|wf> */
    if (strcmp(command, "algo") == 0) {
        cmd.type = CMD_ALGO;
        if (args[0] != '\0') {
            strncpy(cmd.arg1, args, sizeof(cmd.arg1) - 1);
            cmd.arg1[sizeof(cmd.arg1) - 1] = '\0';
            to_lower(cmd.arg1);
            trim(cmd.arg1);
        }
        return cmd;
    }
    
    /* Unknown command */
    cmd.type = CMD_UNKNOWN;
    return cmd;
}

/**
 * @brief Get command name as string
 */
const char* get_command_name(CommandType type) {
    switch (type) {
        case CMD_INIT: return "init";
        case CMD_ALLOC: return "alloc";
        case CMD_FREE: return "free";
        case CMD_SHOW: return "show";
        case CMD_STAT: return "stat";
        case CMD_FRAG: return "frag";
        case CMD_ALGO: return "algo";
        case CMD_RESET: return "reset";
        case CMD_HELP: return "help";
        case CMD_EXIT: return "exit";
        case CMD_EMPTY: return "(empty)";
        case CMD_UNKNOWN: return "(unknown)";
        default: return "(invalid)";
    }
}
