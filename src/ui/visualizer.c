/* 
 * Memory Visualizer Implementation
 * 
 * Enhanced visualization functions for heap memory state.
 * 
 * Date: December 20, 2025
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "visualizer.h"
#include "../core/algorithm_type.h"
#include "../utils/ansi_colors.h"

/* Global color support flag */
int g_use_colors = 0;

/**
 * Count number of free blocks
 */
int count_free_blocks(HeapManager* heap) {
    int count = 0;
    MemoryBlock* current = heap->head;
    
    while (current != NULL) {
        if (current->is_free) {
            count++;
        }
        current = current->next;
    }
    
    return count;
}

/**
 * Find largest free block size
 */
size_t find_largest_free_block(HeapManager* heap) {
    size_t largest = 0;
    MemoryBlock* current = heap->head;
    
    while (current != NULL) {
        if (current->is_free && current->size > largest) {
            largest = current->size;
        }
        current = current->next;
    }
    
    return largest;
}

/**
 * Calculate external fragmentation
 */
float calculate_fragmentation(HeapManager* heap) {
    if (heap->free_size == 0) {
        return 0.0f;
    }
    
    size_t largest = find_largest_free_block(heap);
    
    if (largest == 0) {
        return 0.0f;
    }
    
    /* External Fragmentation = (Total_Free - Largest_Free) / Total_Free × 100% */
    float frag = ((float)(heap->free_size - largest) / (float)heap->free_size) * 100.0f;
    
    return frag;
}

/**
 * Print real-time statistics bar
 */
void print_stats_bar(HeapManager* heap) {
    int usage_percent = 0;
    if (heap->total_size > 0) {
        usage_percent = (int)((heap->used_size * 100) / heap->total_size);
    }
    
    int free_blocks = count_free_blocks(heap);
    
    printf("\n");
    if (g_use_colors) {
        printf("%s╔═══════════════════════════════════════════════════════════════════════╗%s\n", COLOR_CYAN, COLOR_RESET);
        printf("%s║%s  Memory: %s%zu/%zu%s (%s%d%%%s)  │  Algorithm: %s%-15s%s           %s║%s\n",
               COLOR_CYAN, COLOR_RESET,
               COLOR_BOLD_BLUE, heap->used_size, heap->total_size, COLOR_RESET,
               (usage_percent > 80 ? COLOR_BOLD_RED : (usage_percent > 50 ? COLOR_BOLD_YELLOW : COLOR_BOLD_GREEN)),
               usage_percent, COLOR_RESET,
               COLOR_BOLD_CYAN, algorithm_type_to_string(heap->current_algorithm), COLOR_RESET,
               COLOR_CYAN, COLOR_RESET);
        printf("%s║%s  Allocs: %s%-3d%s  │  Deallocs: %s%-3d%s  │  Fails: %s%-3d%s  │  Free Blocks: %s%-3d%s  %s║%s\n",
               COLOR_CYAN, COLOR_RESET,
               COLOR_GREEN, heap->allocation_count, COLOR_RESET,
               COLOR_BLUE, heap->deallocation_count, COLOR_RESET,
               COLOR_RED, heap->failed_allocation_count, COLOR_RESET,
               COLOR_YELLOW, free_blocks, COLOR_RESET,
               COLOR_CYAN, COLOR_RESET);
        printf("%s╚═══════════════════════════════════════════════════════════════════════╝%s\n", COLOR_CYAN, COLOR_RESET);
    } else {
        printf("╔═══════════════════════════════════════════════════════════════════════╗\n");
        printf("║  Memory: %zu/%zu (%d%%)  │  Algorithm: %-15s           ║\n",
               heap->used_size, heap->total_size, usage_percent,
               algorithm_type_to_string(heap->current_algorithm));
        printf("║  Allocs: %-3d  │  Deallocs: %-3d  │  Fails: %-3d  │  Free Blocks: %-3d  ║\n",
               heap->allocation_count, heap->deallocation_count,
               heap->failed_allocation_count, free_blocks);
        printf("╚═══════════════════════════════════════════════════════════════════════╝\n");
    }
}

/**
 * Draw enhanced ASCII memory bar
 */
void draw_memory_bar(HeapManager* heap) {
    printf("\n");
    printf("═══════════════════════════════════════════════════════════════════════════\n");
    printf("                        MEMORY LAYOUT\n");
    printf("═══════════════════════════════════════════════════════════════════════════\n\n");
    
    /* Draw scale */
    printf("Scale: ");
    int num_markers = 11; /* 0, 10%, 20%, ..., 100% */
    for (int i = 0; i < num_markers; i++) {
        size_t pos = (heap->total_size * i) / (num_markers - 1);
        printf("%zu", pos);
        if (i < num_markers - 1) {
            printf("     ");
        }
    }
    printf("\n");
    
    /* Draw top border */
    printf("       ┌");
    for (size_t i = 0; i < 60; i++) {
        printf("─");
    }
    printf("┐\n");
    
    /* Draw memory blocks */
    printf("       │");
    
    MemoryBlock* current = heap->head;
    size_t position = 0;
    const size_t bar_width = 60;
    
    while (current != NULL) {
        /* Calculate width for this block */
        int width = (int)((current->size * bar_width) / heap->total_size);
        if (width < 1 && current->size > 0) {
            width = 1; /* Minimum 1 char for visibility */
        }
        
        if (current->is_free) {
            if (g_use_colors) {
                printf("%s", COLOR_GREEN);
            }
            /* Draw free block */
            for (int i = 0; i < width; i++) {
                printf("░");
            }
            if (g_use_colors) {
                printf("%s", COLOR_RESET);
            }
        } else {
            /* Draw allocated block with separator */
            if (g_use_colors) {
                printf("%s", COLOR_RED);
            }
            /* Draw allocated block */
            for (int i = 0; i < width; i++) {
                printf("█");
            }
            if (g_use_colors) {
                printf("%s", COLOR_RESET);
            }
            /* Add separator between adjacent allocated blocks */
            if (current->next && !current->next->is_free) {
                printf("%s│%s", g_use_colors ? COLOR_YELLOW : "", g_use_colors ? COLOR_RESET : "");
            }
        }
        
        position += current->size;
        current = current->next;
    }
    
    printf("│\n");
    
    /* Draw bottom border */
    printf("       └");
    for (size_t i = 0; i < 60; i++) {
        printf("─");
    }
    printf("┘\n");
    
    /* Legend */
    printf("\n");
    if (g_use_colors) {
        printf("       Legend:  %s█%s Allocated    %s░%s Free\n",
               COLOR_RED, COLOR_RESET, COLOR_GREEN, COLOR_RESET);
    } else {
        printf("       Legend:  █ Allocated    ░ Free\n");
    }
    
    /* Block details */
    printf("\n");
    printf("       Block Details:\n");
    printf("       ───────────────────────────────────────────────────────\n");
    
    current = heap->head;
    int block_num = 1;
    while (current != NULL) {
        if (current->is_free) {
            if (g_use_colors) {
                printf("       %sBlock %d:%s [Addr: %-4zu | Size: %-4zu | %sFREE%s]\n",
                       COLOR_BOLD, block_num, COLOR_RESET,
                       current->start_address, current->size,
                       COLOR_BOLD_GREEN, COLOR_RESET);
            } else {
                printf("       Block %d: [Addr: %-4zu | Size: %-4zu | FREE]\n",
                       block_num, current->start_address, current->size);
            }
        } else {
            if (g_use_colors) {
                printf("       %sBlock %d:%s [Addr: %-4zu | Size: %-4zu | PID: %s%s%s]\n",
                       COLOR_BOLD, block_num, COLOR_RESET,
                       current->start_address, current->size,
                       COLOR_BOLD_RED, current->process_id, COLOR_RESET);
            } else {
                printf("       Block %d: [Addr: %-4zu | Size: %-4zu | PID: %s]\n",
                       block_num, current->start_address, current->size,
                       current->process_id);
            }
        }
        block_num++;
        current = current->next;
    }
    
    printf("═══════════════════════════════════════════════════════════════════════════\n");
}

/**
 * Show fragmentation report
 */
void show_fragmentation_report(HeapManager* heap) {
    float frag = calculate_fragmentation(heap);
    size_t largest = find_largest_free_block(heap);
    int free_blocks = count_free_blocks(heap);
    
    printf("\n");
    if (g_use_colors) {
        printf("%s╔═══════════════════════════════════════════════════════════════════════╗%s\n", COLOR_CYAN, COLOR_RESET);
        printf("%s║%s              %sFRAGMENTATION ANALYSIS%s                                    %s║%s\n",
               COLOR_CYAN, COLOR_RESET, COLOR_BOLD_YELLOW, COLOR_RESET, COLOR_CYAN, COLOR_RESET);
        printf("%s╠═══════════════════════════════════════════════════════════════════════╣%s\n", COLOR_CYAN, COLOR_RESET);
        printf("%s║%s  Total Free Space:        %s%-6zu bytes%s                                 %s║%s\n",
               COLOR_CYAN, COLOR_RESET, COLOR_BOLD_GREEN, heap->free_size, COLOR_RESET, COLOR_CYAN, COLOR_RESET);
        printf("%s║%s  Largest Free Block:      %s%-6zu bytes%s                                 %s║%s\n",
               COLOR_CYAN, COLOR_RESET, COLOR_BOLD_BLUE, largest, COLOR_RESET, COLOR_CYAN, COLOR_RESET);
        printf("%s║%s  Number of Free Blocks:   %s%-3d%s                                        %s║%s\n",
               COLOR_CYAN, COLOR_RESET, COLOR_BOLD, free_blocks, COLOR_RESET, COLOR_CYAN, COLOR_RESET);
        printf("%s║%s  External Fragmentation:  ", COLOR_CYAN, COLOR_RESET);
        
        /* Color-code severity */
        if (frag < 30.0f) {
            printf("%s%.1f%%%s (Good)", COLOR_BOLD_GREEN, frag, COLOR_RESET);
        } else if (frag < 60.0f) {
            printf("%s%.1f%%%s (Moderate)", COLOR_BOLD_YELLOW, frag, COLOR_RESET);
        } else {
            printf("%s%.1f%%%s (High)", COLOR_BOLD_RED, frag, COLOR_RESET);
        }
        
        printf("                                   %s║%s\n", COLOR_CYAN, COLOR_RESET);
        printf("%s╚═══════════════════════════════════════════════════════════════════════╝%s\n", COLOR_CYAN, COLOR_RESET);
    } else {
        printf("╔═══════════════════════════════════════════════════════════════════════╗\n");
        printf("║              FRAGMENTATION ANALYSIS                                    ║\n");
        printf("╠═══════════════════════════════════════════════════════════════════════╣\n");
        printf("║  Total Free Space:        %-6zu bytes                                 ║\n", heap->free_size);
        printf("║  Largest Free Block:      %-6zu bytes                                 ║\n", largest);
        printf("║  Number of Free Blocks:   %-3d                                        ║\n", free_blocks);
        printf("║  External Fragmentation:  %.1f%%", frag);
        
        if (frag < 30.0f) {
            printf(" (Good)");
        } else if (frag < 60.0f) {
            printf(" (Moderate)");
        } else {
            printf(" (High)");
        }
        
        printf("                                   ║\n");
        printf("╚═══════════════════════════════════════════════════════════════════════╝\n");
    }
    
    printf("\n");
    printf("Interpretation:\n");
    printf("  • < 30%%:  Good - Memory is well-utilized\n");
    printf("  • 30-60%%: Moderate - Some fragmentation present\n");
    printf("  • > 60%%:  High - Consider defragmentation\n");
    printf("\n");
}
