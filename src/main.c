/* 
 * Memory Allocation Simulator - Main Entry Point
 * 
 * Supports 3 modes:
 * - Interactive Menu (default): ./memory_allocator
 * - Interactive Shell: ./memory_allocator -i
 * - Batch Mode: ./memory_allocator -b <file>
 * 
 * Date: December 20, 2025
 * Course: Systems Programming (Unix-like)
 */

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "core/heap_manager.h"
#include "core/algorithm_type.h"
#include "ui/cli.h"
#include "ui/shell.h"
#include "utils/input_utils.h"
#include "utils/benchmark.h"

/**
 * @brief Display help message
 */
static void print_usage(const char* prog_name) {
    printf("\nMemory Allocator v2.0 - Memory Allocation Algorithms Simulator\n");
    printf("===============================================================\n\n");
    printf("Usage:\n");
    printf("  %s                   Interactive menu mode (default)\n", prog_name);
    printf("  %s -i                Interactive shell mode (REPL)\n", prog_name);
    printf("  %s -b <file>         Batch mode (execute script file)\n", prog_name);
    printf("  %s --benchmark <s>   Run benchmark (scenarios: random, worst, web, all)\n", prog_name);
    printf("  %s --help            Display this help message\n", prog_name);
    printf("  %s --version         Display version information\n\n", prog_name);
    printf("Interactive Shell Commands:\n");
    printf("  init <size>           Initialize heap with size (bytes)\n");
    printf("  alloc <PID>:<size>    Allocate memory for process\n");
    printf("  free <PID>            Free memory of process\n");
    printf("  show                  Display memory map\n");
    printf("  stat                  Display heap statistics\n");
    printf("  frag                  Display fragmentation report\n");
    printf("  algo <ff|nf|bf|wf>    Set allocation algorithm\n");
    printf("  export <file>         Export memory state to JSON\n");
    printf("  reset                 Reset heap to initial state\n");
    printf("  help                  Display help message\n");
    printf("  exit                  Exit shell\n\n");
    printf("Algorithms:\n");
    printf("  ff   First Fit   - Find first free block large enough\n");
    printf("  nf   Next Fit    - Start from last allocated position\n");
    printf("  bf   Best Fit    - Find smallest sufficient free block\n");
    printf("  wf   Worst Fit   - Find largest free block\n\n");
    printf("Benchmark Scenarios:\n");
    printf("  random    - Random allocation/deallocation pattern\n");
    printf("  worst     - Worst case fragmentation test\n");
    printf("  web       - Web server simulation\n");
    printf("  all       - Run all scenarios and compare\n\n");
    printf("Examples:\n");
    printf("  %s -i\n", prog_name);
    printf("  allocator> init 1024\n");
    printf("  allocator> alloc P1:100\n");
    printf("  allocator> show\n");
    printf("  allocator> export state.json\n\n");
    printf("  %s --benchmark all\n", prog_name);
    printf("  %s -b scenarios/test_shell.txt\n\n", prog_name);
    printf("For more information, see docs/RECIPE_CMD.md\n\n");
}

/**
 * @brief Display version information
 */
static void print_version(void) {
    printf("Memory Allocator v3.0 (Phase 3)\n");
    printf("Algorithms: First Fit, Next Fit, Best Fit, Worst Fit\n");
    printf("Features: Interactive Shell, Batch Mode, Benchmark, JSON Export\n");
}

/** * @brief Run benchmark mode
 */
static int run_benchmark(const char* scenario_name) {
    BenchmarkStats results[4];
    BenchmarkScenario* scenario = NULL;
    
    if (strcmp(scenario_name, "random") == 0) {
        scenario = &SCENARIO_RANDOM;
    } else if (strcmp(scenario_name, "worst") == 0) {
        scenario = &SCENARIO_WORST_CASE;
    } else if (strcmp(scenario_name, "web") == 0) {
        scenario = &SCENARIO_WEB_SERVER;
    } else if (strcmp(scenario_name, "all") == 0) {
        /* Run all scenarios */
        printf("\n╔═══════════════════════════════════════════════════════════════╗\n");
        printf("║           RUNNING ALL BENCHMARK SCENARIOS                  ║\n");
        printf("╚═══════════════════════════════════════════════════════════════╝\n\n");
        
        BenchmarkScenario* scenarios[] = {
            &SCENARIO_RANDOM,
            &SCENARIO_WORST_CASE,
            &SCENARIO_WEB_SERVER
        };
        
        for (int i = 0; i < 3; i++) {
            printf("\n═══════════════════════════════════════════════════════════════\n");
            printf("  SCENARIO %d: %s\n", i + 1, scenarios[i]->name);
            printf("═══════════════════════════════════════════════════════════════\n\n");
            
            benchmark_compare_all(scenarios[i], results);
            benchmark_print_comparison(results);
            
            /* Export results */
            char filename[256];
            snprintf(filename, sizeof(filename), "benchmark_%s.json", 
                    i == 0 ? "random" : i == 1 ? "worst" : "web");
            benchmark_export_json(filename, results, scenarios[i]);
        }
        
        printf("\n✓ All benchmarks completed!\n\n");
        return 0;
    } else {
        fprintf(stderr, "Error: Unknown scenario '%s'\n", scenario_name);
        fprintf(stderr, "Available scenarios: random, worst, web, all\n");
        return 1;
    }
    
    /* Run single scenario */
    benchmark_compare_all(scenario, results);
    benchmark_print_comparison(results);
    
    /* Export results */
    char filename[256];
    snprintf(filename, sizeof(filename), "benchmark_%s.json", scenario_name);
    benchmark_export_json(filename, results, scenario);
    
    return 0;
}

/** * @brief Main entry point with mode selection
 */
int main(int argc, char* argv[]) {
    /* Parse command-line arguments */
    
    /* --help */
    if (argc == 2 && (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0)) {
        print_usage(argv[0]);
        return EXIT_SUCCESS;
    }
    
    /* --version */
    if (argc == 2 && (strcmp(argv[1], "--version") == 0 || strcmp(argv[1], "-v") == 0)) {
        print_version();
        return EXIT_SUCCESS;
    }
    
    /* -i or --interactive: Interactive Shell Mode */
    if (argc == 2 && (strcmp(argv[1], "-i") == 0 || strcmp(argv[1], "--interactive") == 0)) {
        run_interactive_shell(0); /* 0 = ask user for size */
        return EXIT_SUCCESS;
    }
    
    /* -b or --batch: Batch Mode */
    if (argc == 3 && (strcmp(argv[1], "-b") == 0 || strcmp(argv[1], "--batch") == 0)) {
        int result = run_batch_mode(argv[2]);
        return result == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
    }
    
    /* --benchmark: Benchmark Mode */
    if (argc == 3 && strcmp(argv[1], "--benchmark") == 0) {
        int result = run_benchmark(argv[2]);
        return result == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
    }
    
    /* Default: Interactive Menu Mode (Phase 1 - backward compatible) */
    if (argc == 1) {
        size_t initial_size = 1024; /* Default size */
        
        printf("=== Memory Allocation Simulator ===\n");
        
        /* Optional: Ask user for initial heap size */
        int size_input = read_int("Enter initial heap size (bytes) [Default 1024]: ");
        if (size_input > 0) {
            initial_size = (size_t)size_input;
        }
        
        printf("Initializing heap with %zu bytes...\n", initial_size);
        HeapManager* heap = heap_init(initial_size);
        
        if (heap == NULL) {
            fprintf(stderr, "Failed to initialize heap\n");
            return EXIT_FAILURE;
        }
        
        /* Start the Command Line Interface (Menu Mode) */
        run_cli(heap);
        
        /* Cleanup */
        heap_destroy(heap);
        
        return EXIT_SUCCESS;
    }
    
    /* Invalid arguments */
    fprintf(stderr, "Error: Invalid arguments\n");
    print_usage(argv[0]);
    return EXIT_FAILURE;
}
