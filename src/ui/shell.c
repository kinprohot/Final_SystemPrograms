/* 
 * Interactive Command-Line Shell
 * 
 * REPL (Read-Eval-Print Loop) for interactive command execution.
 * Provides stateful session with persistent heap state.
 * 
 * Date: December 20, 2025
 */

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../core/heap_manager.h"
#include "../core/algorithm_type.h"
#include "../utils/shell_parser.h"
#include "../utils/ansi_colors.h"
#include "../utils/export.h"
#include "visualizer.h"
#include "shell.h"

/* Global shell state */
static HeapManager* g_shell_heap = NULL;
static size_t g_shell_initial_size = 1024;

/**
 * @brief Display welcome banner
 */
static void print_welcome_banner(void) {
    const char* cyan = supports_color() ? COLOR_CYAN : "";
    const char* reset = supports_color() ? COLOR_RESET : "";
    
    printf("\n");
    printf("%s", cyan);
    printf("╔═══════════════════════════════════════════════════════════════╗\n");
    printf("║       Memory Allocator Interactive Shell v2.0                ║\n");
    printf("║       Algorithms: First Fit, Next Fit, Best Fit, Worst Fit   ║\n");
    printf("╚═══════════════════════════════════════════════════════════════╝\n");
    printf("%s", reset);
    printf("\nType 'help' for commands, 'exit' to quit\n\n");
}

/**
 * @brief Display help message
 */
static void print_help(void) {
    printf("\nAvailable Commands:\n");
    printf("  init <size>            Initialize heap with size (bytes)\n");
    printf("  alloc <PID>:<size>     Allocate memory for process\n");
    printf("  free <PID>             Free memory of process\n");
    printf("  show [mode]            Display memory map\n");
    printf("                         - show         : Beautiful visualization (default)\n");
    printf("                         - show simple  : Text-only view\n");
    printf("                         - show detail  : Full analysis (stats+viz+frag)\n");
    printf("  stat                   Display heap statistics\n");
    printf("  frag                   Display fragmentation report\n");
    printf("  algo <ff|nf|bf|wf>     Set allocation algorithm\n");
    printf("                         (ff=First Fit, nf=Next Fit,\n");
    printf("                          bf=Best Fit, wf=Worst Fit)\n");
    printf("  export <file>          Export memory state to JSON\n");
    printf("  reset                  Reset heap to initial state\n");
    printf("  help                   Display this help message\n");
    printf("  exit                   Exit shell\n");
    printf("\nExamples:\n");
    printf("  init 1024\n");
    printf("  alloc P1:100\n");
    printf("  alloc P2:200\n");
    printf("  show              # Beautiful visualization\n");
    printf("  show detail       # Full analysis\n");
    printf("  free P1\n");
    printf("  algo bf\n");
    printf("  export state.json # Export current state\n");
    printf("\n");
}

/**
 * @brief Handle init command
 */
static int handle_init(size_t size) {
    if (size == 0) {
        printf("✗ Error: Invalid size. Usage: init <size>\n");
        return -1;
    }
    
    if (g_shell_heap != NULL) {
        /* Free existing heap */
        heap_destroy(g_shell_heap);
        g_shell_heap = NULL;
    }
    
    g_shell_heap = heap_init(size);
    if (g_shell_heap == NULL) {
        printf("✗ Error: Failed to initialize heap\n");
        return -1;
    }
    
    g_shell_initial_size = size;
    printf("✓ Heap initialized: %zu bytes\n", size);
    return 0;
}

/**
 * @brief Handle alloc command
 */
static int handle_alloc(const char* pid, size_t size) {
    if (!g_shell_heap) {
        printf("✗ Error: Heap not initialized. Use 'init <size>' first\n");
        return -1;
    }
    
    if (pid[0] == '\0') {
        printf("✗ Error: Missing process ID. Usage: alloc <PID>:<size>\n");
        return -1;
    }
    
    if (size == 0) {
        printf("✗ Error: Missing size. Usage: alloc <PID>:<size>\n");
        return -1;
    }
    
    MemoryBlock* block = heap_allocate(g_shell_heap, pid, size);
    if (block) {
        printf("✓ Allocated %zu bytes for %s at address %zu\n", 
               size, pid, block->start_address);
        return 0;
    } else {
        printf("✗ Error: Not enough memory (requested: %zu, available: %zu)\n", 
               size, g_shell_heap->free_size);
        return -1;
    }
}

/**
 * @brief Handle free command
 */
static int handle_free(const char* pid) {
    if (!g_shell_heap) {
        printf("✗ Error: Heap not initialized\n");
        return -1;
    }
    
    if (pid[0] == '\0') {
        printf("✗ Error: Missing process ID. Usage: free <PID>\n");
        return -1;
    }
    
    int result = heap_free(g_shell_heap, pid);
    if (result == 1) {
        // Success message already printed by heap_free
        return 0;
    } else {
        // Error already printed
        return -1;
    }
}

/**
 * @brief Handle show command with variants
 * @param mode "" = default (beautiful), "simple" = text only, "detail" = with stats
 */
static int handle_show(const char* mode) {
    if (!g_shell_heap) {
        printf("✗ Error: Heap not initialized\n");
        return -1;
    }
    
    /* Simple text mode */
    if (mode && strcmp(mode, "simple") == 0) {
        printf("\n");
        heap_show_memory_map(g_shell_heap);
        printf("\n");
        return 0;
    }
    
    /* Detail mode: stats + beautiful visualization + fragmentation */
    if (mode && strcmp(mode, "detail") == 0) {
        print_stats_bar(g_shell_heap);
        draw_memory_bar(g_shell_heap);
        show_fragmentation_report(g_shell_heap);
        return 0;
    }
    
    /* Default: beautiful visualization only */
    printf("\n");
    draw_memory_bar(g_shell_heap);
    printf("\n");
    return 0;
}

/**
 * @brief Handle stat command
 */
static int handle_stat(void) {
    if (!g_shell_heap) {
        printf("✗ Error: Heap not initialized\n");
        return -1;
    }
    
    printf("\n");
    print_stats_bar(g_shell_heap);
    printf("\n");
    return 0;
}

/**
 * @brief Handle frag command
 */
static int handle_frag(void) {
    if (!g_shell_heap) {
        printf("✗ Error: Heap not initialized\n");
        return -1;
    }
    
    printf("\n");
    show_fragmentation_report(g_shell_heap);
    printf("\n");
    return 0;
}

/**
 * @brief Handle algo command
 */
static int handle_algo(const char* algo_str) {
    if (!g_shell_heap) {
        printf("✗ Error: Heap not initialized\n");
        return -1;
    }
    
    if (algo_str[0] == '\0') {
        printf("✗ Error: Missing algorithm. Usage: algo <ff|nf|bf|wf>\n");
        return -1;
    }
    
    AlgorithmType algo = FIRST_FIT;
    const char* algo_name = NULL;
    
    if (strcmp(algo_str, "ff") == 0) {
        algo = FIRST_FIT;
        algo_name = "First Fit";
    } else if (strcmp(algo_str, "nf") == 0) {
        algo = NEXT_FIT;
        algo_name = "Next Fit";
    } else if (strcmp(algo_str, "bf") == 0) {
        algo = BEST_FIT;
        algo_name = "Best Fit";
    } else if (strcmp(algo_str, "wf") == 0) {
        algo = WORST_FIT;
        algo_name = "Worst Fit";
    } else {
        printf("✗ Error: Invalid algorithm '%s'. Use: ff, nf, bf, wf\n", algo_str);
        return -1;
    }
    
    heap_set_algorithm(g_shell_heap, algo);
    printf("✓ Algorithm set to: %s\n", algo_name);
    return 0;
}

/**
 * @brief Handle reset command
 */
static int handle_reset(void) {
    if (!g_shell_heap) {
        printf("✗ Error: Heap not initialized\n");
        return -1;
    }
    
    heap_destroy(g_shell_heap);
    g_shell_heap = heap_init(g_shell_initial_size);
    
    if (g_shell_heap == NULL) {
        printf("✗ Error: Failed to reset heap\n");
        return -1;
    }
    
    printf("✓ Heap reset to initial state (%zu bytes)\n", g_shell_initial_size);
    return 0;
}

/**
 * @brief Handle export command
 */
static int handle_export(const char* filename) {
    if (!g_shell_heap) {
        printf("✗ Error: Heap not initialized\n");
        return -1;
    }
    
    if (!filename || strlen(filename) == 0) {
        printf("✗ Error: Filename required. Usage: export <filename>\n");
        return -1;
    }
    
    /* Check extension: export to HTML if .html, else JSON */
    int result = 0;
    size_t len = strlen(filename);
    if ((len >= 5 && strcmp(filename + len - 5, ".html") == 0) ||
        (len >= 4 && strcmp(filename + len - 4, ".htm") == 0)) {
        result = export_snapshot_to_html(g_shell_heap, filename);
    } else {
        result = export_snapshot_to_json(g_shell_heap, filename);
    }
    
    if (result == 0) {
        printf("✓ Memory state exported to: %s\n", filename);
    } else {
        printf("✗ Error: Failed to export to %s\n", filename);
    }
    
    return result;
}

/**
 * @brief Execute parsed command
 */
static int execute_command(ParsedCommand* cmd) {
    switch (cmd->type) {
        case CMD_EMPTY:
            return 0;
            
        case CMD_EXIT:
            return 1; /* Signal to exit */
            
        case CMD_HELP:
            print_help();
            return 0;
            
        case CMD_INIT:
            return handle_init(cmd->arg2);
            
        case CMD_ALLOC:
            return handle_alloc(cmd->arg1, cmd->arg2);
            
        case CMD_FREE:
            return handle_free(cmd->arg1);
            
        case CMD_SHOW:
            return handle_show(cmd->arg1);
            
        case CMD_STAT:
            return handle_stat();
            
        case CMD_FRAG:
            return handle_frag();
            
        case CMD_ALGO:
            return handle_algo(cmd->arg1);
            
        case CMD_EXPORT:
            return handle_export(cmd->arg1);
            
        case CMD_RESET:
            return handle_reset();
            
        case CMD_UNKNOWN:
            printf("✗ Unknown command. Type 'help' for available commands\n");
            return 0;
            
        default:
            printf("✗ Invalid command\n");
            return 0;
    }
}

/**
 * @brief Run interactive shell mode
 */
void run_interactive_shell(size_t initial_size) {
    print_welcome_banner();
    
    /* Auto-initialize heap if size provided */
    if (initial_size > 0) {
        handle_init(initial_size);
    }
    
    int running = 1;
    char input_buffer[256];
    
    while (running) {
        /* Print prompt */
        const char* cyan = supports_color() ? COLOR_CYAN : "";
        const char* reset = supports_color() ? COLOR_RESET : "";
        printf("%sallocator>%s ", cyan, reset);
        fflush(stdout);
        
        /* Read input */
        if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
            /* EOF or error */
            printf("\n");
            break;
        }
        
        /* Remove newline */
        size_t len = strlen(input_buffer);
        if (len > 0 && input_buffer[len - 1] == '\n') {
            input_buffer[len - 1] = '\0';
        }
        
        /* Parse and execute */
        ParsedCommand cmd = parse_shell_command(input_buffer);
        int result = execute_command(&cmd);
        
        /* Check for exit signal */
        if (result == 1) {
            running = 0;
        }
    }
    
    /* Cleanup */
    if (g_shell_heap) {
        heap_destroy(g_shell_heap);
        g_shell_heap = NULL;
    }
    
    printf("Goodbye!\n");
}

/**
 * @brief Run batch mode from file
 */
int run_batch_mode(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error: Cannot open file '%s'\n", filename);
        return -1;
    }
    
    printf("Executing batch file: %s\n\n", filename);
    
    char line_buffer[256];
    int line_number = 0;
    int error_count = 0;
    
    while (fgets(line_buffer, sizeof(line_buffer), file)) {
        line_number++;
        
        /* Remove newline */
        size_t len = strlen(line_buffer);
        if (len > 0 && line_buffer[len - 1] == '\n') {
            line_buffer[len - 1] = '\0';
        }
        
        /* Parse command */
        ParsedCommand cmd = parse_shell_command(line_buffer);
        
        /* Skip empty lines and comments */
        if (cmd.type == CMD_EMPTY) {
            continue;
        }
        
        /* Echo command */
        printf("[Line %d] %s\n", line_number, line_buffer);
        
        /* Execute */
        int result = execute_command(&cmd);
        
        if (result < 0) {
            error_count++;
            fprintf(stderr, "Error on line %d\n", line_number);
        }
        
        /* Exit command */
        if (result == 1) {
            break;
        }
    }
    
    fclose(file);
    
    /* Cleanup */
    if (g_shell_heap) {
        heap_destroy(g_shell_heap);
        g_shell_heap = NULL;
    }
    
    printf("\nBatch execution complete: %d errors\n", error_count);
    return error_count > 0 ? -1 : 0;
}
