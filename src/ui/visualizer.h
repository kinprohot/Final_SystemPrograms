/* 
 * Memory Visualizer - Enhanced ASCII Memory Map Display
 * 
 * This file provides functions for visualizing heap memory state
 * with improved ASCII graphics, including:
 * - Horizontal memory bar with scale
 * - Box-drawing characters for borders
 * - Visual representation of allocated vs free blocks
 * 
 * Date: December 20, 2025
 */

#ifndef VISUALIZER_H
#define VISUALIZER_H

#include "../core/heap_manager.h"

/**
 * @brief Draw enhanced ASCII memory map with visual scale
 * @param heap Pointer to HeapManager
 * 
 * Example output:
 * Memory Layout (1000 bytes):
 * ┌────────────────────────────────────────────────────────────┐
 * │[P1:100]░[FREE:50]░[P2:200]░░░[FREE:150]░░[P3:300]░░░░░░░░░│
 * └────────────────────────────────────────────────────────────┘
 *  0      100    150      350        500      800            1000
 */
void draw_memory_bar(HeapManager* heap);

/**
 * @brief Count number of free blocks in heap
 * @param heap Pointer to HeapManager
 * @return Number of free blocks
 */
int count_free_blocks(HeapManager* heap);

/**
 * @brief Find largest free block size
 * @param heap Pointer to HeapManager
 * @return Size of largest free block (0 if no free blocks)
 */
size_t find_largest_free_block(HeapManager* heap);

/**
 * @brief Calculate external fragmentation percentage
 * @param heap Pointer to HeapManager
 * @return Fragmentation percentage (0.0 - 100.0)
 * 
 * Formula: (Total_Free - Largest_Free) / Total_Free × 100%
 */
float calculate_fragmentation(HeapManager* heap);

/**
 * @brief Display fragmentation analysis report
 * @param heap Pointer to HeapManager
 */
void show_fragmentation_report(HeapManager* heap);

/**
 * @brief Print real-time statistics bar
 * @param heap Pointer to HeapManager
 * 
 * Displays always-visible statistics at top of menu
 */
void print_stats_bar(HeapManager* heap);

#endif /* VISUALIZER_H */
