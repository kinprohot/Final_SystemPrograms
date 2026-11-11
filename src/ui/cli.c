#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cli.h"
#include "visualizer.h"
#include "../utils/input_utils.h"
#include "../utils/ansi_colors.h"
#include "../core/algorithm_type.h"

void print_header() {
    printf("\n=============================================\n");
    printf("      MEMORY ALLOCATION SIMULATOR            \n");
    printf("=============================================\n");
}

void print_menu() {
    printf("\nMAIN MENU:\n");
    printf("1. Allocate Memory\n");
    printf("2. Free Memory\n");
    printf("3. Show Memory Map\n");
    printf("4. Change Algorithm\n");
    printf("5. Show Statistics\n");
    printf("6. Show Fragmentation Report\n");
    printf("7. Toggle Color Mode\n");
    printf("0. Exit\n");
    printf("---------------------------------------------\n");
}

const char* get_algo_name(AlgorithmType type) {
    switch(type) {
        case FIRST_FIT: return "First Fit";
        case NEXT_FIT: return "Next Fit";
        case BEST_FIT: return "Best Fit";
        case WORST_FIT: return "Worst Fit";
        default: return "Unknown";
    }
}

void handle_allocation(HeapManager* heap) {
    char pid[32];
    read_string("Enter Process ID: ", pid, sizeof(pid));
    
    if (strlen(pid) == 0) {
        printf("Error: Process ID cannot be empty.\n");
        return;
    }

    int size = read_int("Enter Size (bytes): ");
    if (size <= 0) {
        printf("Error: Size must be positive.\n");
        return;
    }

    MemoryBlock* block = heap_allocate(heap, pid, (size_t)size);
    if (block) {
        printf("Success: Allocated %d bytes for process '%s' at address %zu.\n", size, pid, block->start_address);
    } else {
        printf("Failed: Could not allocate %d bytes for process '%s' (Not enough memory).\n", size, pid);
    }
}

void handle_free(HeapManager* heap) {
    char pid[32];
    read_string("Enter Process ID to free: ", pid, sizeof(pid));

    if (strlen(pid) == 0) {
        printf("Error: Process ID cannot be empty.\n");
        return;
    }

    int result = heap_free(heap, pid);
    if (result == 1) {
        printf("Success: Memory for process '%s' has been freed.\n", pid);
    } else {
        printf("Error: Process '%s' not found in memory.\n", pid);
    }
}

void handle_change_algo(HeapManager* heap) {
    printf("\nSELECT ALGORITHM:\n");
    printf("1. First Fit\n");
    printf("2. Next Fit\n");
    printf("3. Best Fit\n");
    printf("4. Worst Fit\n");
    
    int choice = read_int("Choose algorithm (1-4): ");
    AlgorithmType new_algo;

    switch (choice) {
        case 1: new_algo = FIRST_FIT; break;
        case 2: new_algo = NEXT_FIT; break;
        case 3: new_algo = BEST_FIT; break;
        case 4: new_algo = WORST_FIT; break;
        default:
            printf("Invalid choice. Algorithm unchanged.\n");
            return;
    }

    heap_set_algorithm(heap, new_algo);
    printf("Algorithm changed to: %s\n", get_algo_name(heap->current_algorithm));
}

void handle_stats(HeapManager* heap) {
    printf("\n--- HEAP STATISTICS ---\n");
    printf("Total Size:      %zu bytes\n", heap->total_size);
    printf("Used Size:       %zu bytes\n", heap->used_size);
    printf("Free Size:       %zu bytes\n", heap->free_size);
    printf("Current Algo:    %s\n", get_algo_name(heap->current_algorithm));
    printf("Allocations:     %d\n", heap->allocation_count);
    printf("Deallocations:   %d\n", heap->deallocation_count);
    printf("Failed Allocs:   %d\n", heap->failed_allocation_count);
}

void run_cli(HeapManager* heap) {
    int running = 1;
    
    /* Initialize color support */
    g_use_colors = supports_color();
    
    print_header();
    
    if (g_use_colors) {
        printf("\n%s[Color mode enabled]%s\n", COLOR_GREEN, COLOR_RESET);
    }

    while (running) {
        /* Phase 2: Real-time statistics bar */
        print_stats_bar(heap);
        
        print_menu();
        int choice = read_int("Enter your choice: ");

        switch (choice) {
            case 1:
                handle_allocation(heap);
                break;
            case 2:
                handle_free(heap);
                break;
            case 3:
                /* Phase 2: Enhanced ASCII visualization */
                draw_memory_bar(heap);
                break;
            case 4:
                handle_change_algo(heap);
                break;
            case 5:
                handle_stats(heap);
                break;
            case 6:
                /* Phase 2: Fragmentation report */
                show_fragmentation_report(heap);
                break;
            case 7:
                /* Phase 2: Toggle color mode */
                g_use_colors = !g_use_colors;
                printf("\nColor mode: %s\n", g_use_colors ? "ENABLED" : "DISABLED");
                break;
            case 0:
                running = 0;
                printf("\nExiting simulator. Goodbye!\n");
                break;
            default:
                printf("\nInvalid choice. Please try again.\n");
        }
        
        if (running && choice != 0) {
            printf("\nPress Enter to continue...");
            getchar();  /* Wait for Enter key */
        }
    }
}
