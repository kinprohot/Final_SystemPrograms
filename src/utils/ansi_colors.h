/* 
 * ANSI Color Codes for Terminal Output
 * 
 * This file provides ANSI escape codes for colored terminal output.
 * Includes auto-detection of terminal color support.
 * 
 * Date: December 20, 2025
 */

#ifndef ANSI_COLORS_H
#define ANSI_COLORS_H

#include <stdio.h>
#include <unistd.h>

/* ANSI Color Codes */
#define COLOR_RESET   "\033[0m"
#define COLOR_BOLD    "\033[1m"

/* Foreground Colors */
#define COLOR_RED     "\033[31m"
#define COLOR_GREEN   "\033[32m"
#define COLOR_YELLOW  "\033[33m"
#define COLOR_BLUE    "\033[34m"
#define COLOR_MAGENTA "\033[35m"
#define COLOR_CYAN    "\033[36m"
#define COLOR_WHITE   "\033[37m"

/* Bold + Foreground Colors */
#define COLOR_BOLD_RED     "\033[1;31m"
#define COLOR_BOLD_GREEN   "\033[1;32m"
#define COLOR_BOLD_YELLOW  "\033[1;33m"
#define COLOR_BOLD_BLUE    "\033[1;34m"
#define COLOR_BOLD_CYAN    "\033[1;36m"

/* Background Colors */
#define BG_RED     "\033[41m"
#define BG_GREEN   "\033[42m"
#define BG_YELLOW  "\033[43m"
#define BG_BLUE    "\033[44m"

/* Box Drawing Characters (UTF-8) */
#define BOX_HORIZONTAL "─"
#define BOX_VERTICAL   "│"
#define BOX_TOP_LEFT   "╔"
#define BOX_TOP_RIGHT  "╗"
#define BOX_BOTTOM_LEFT "╚"
#define BOX_BOTTOM_RIGHT "╝"
#define BOX_T_DOWN     "╦"
#define BOX_T_UP       "╩"
#define BOX_T_RIGHT    "╠"
#define BOX_T_LEFT     "╣"
#define BOX_CROSS      "╬"

/* Check if terminal supports colors */
static inline int supports_color(void) {
    /* Check if stdout is a terminal */
    if (!isatty(STDOUT_FILENO)) {
        return 0;
    }
    
    /* Check TERM environment variable */
    const char* term = getenv("TERM");
    if (term == NULL) {
        return 0;
    }
    
    /* Simple check for common color-supporting terminals */
    if (strstr(term, "color") != NULL ||
        strstr(term, "xterm") != NULL ||
        strstr(term, "screen") != NULL ||
        strstr(term, "linux") != NULL) {
        return 1;
    }
    
    return 0;
}

/* Global flag for color support (set once at startup) */
extern int g_use_colors;

/* Macros for conditional color output */
#define C_RED(text)    (g_use_colors ? COLOR_RED text COLOR_RESET : text)
#define C_GREEN(text)  (g_use_colors ? COLOR_GREEN text COLOR_RESET : text)
#define C_YELLOW(text) (g_use_colors ? COLOR_YELLOW text COLOR_RESET : text)
#define C_BLUE(text)   (g_use_colors ? COLOR_BLUE text COLOR_RESET : text)
#define C_CYAN(text)   (g_use_colors ? COLOR_CYAN text COLOR_RESET : text)

#endif /* ANSI_COLORS_H */
