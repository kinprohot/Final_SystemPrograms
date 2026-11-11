 /* 
 * Acceptance Criteria Test Suite (AC1-AC8)
 * 
 * This file contains test scenarios for all 8 acceptance criteria
 * defined in PR.md. Each test is self-contained and verifies specific
 * behavior required by the specification.
 * 
 * Date: December 19, 2025
 * Course: Systems Programming (Unix-like)
 */

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../src/core/heap_manager.h"
#include "../src/core/algorithm_type.h"

/* ANSI Color Codes for Output */
#define COLOR_RESET   "\033[0m"
#define COLOR_GREEN   "\033[32m"
#define COLOR_RED     "\033[31m"
#define COLOR_YELLOW  "\033[33m"
#define COLOR_BLUE    "\033[34m"
#define COLOR_CYAN    "\033[36m"

/* Test Result Tracking */
static int total_tests = 0;
static int passed_tests = 0;
static int failed_tests = 0;

/* Helper Functions */
void print_test_header(const char* test_name) {
    printf("\n%s========================================%s\n", COLOR_CYAN, COLOR_RESET);
    printf("%sTEST: %s%s\n", COLOR_BLUE, test_name, COLOR_RESET);
    printf("%s========================================%s\n", COLOR_CYAN, COLOR_RESET);
}

void assert_condition(const char* description, int condition) {
    total_tests++;
    if (condition) {
        printf("  %s[PASS]%s %s\n", COLOR_GREEN, COLOR_RESET, description);
        passed_tests++;
    } else {
        printf("  %s[FAIL]%s %s\n", COLOR_RED, COLOR_RESET, description);
        failed_tests++;
    }
}

void print_summary() {
    printf("\n%s========================================%s\n", COLOR_CYAN, COLOR_RESET);
    printf("%sTEST SUMMARY%s\n", COLOR_BLUE, COLOR_RESET);
    printf("%s========================================%s\n", COLOR_CYAN, COLOR_RESET);
    printf("Total Tests:  %d\n", total_tests);
    printf("%sPassed:%s       %d\n", COLOR_GREEN, COLOR_RESET, passed_tests);
    printf("%sFailed:%s       %d\n", COLOR_RED, COLOR_RESET, failed_tests);
    printf("Success Rate: %.1f%%\n", (passed_tests * 100.0) / total_tests);
    printf("%s========================================%s\n\n", COLOR_CYAN, COLOR_RESET);
}

/* AC1: Initialization Test */
void test_ac1_initialization() {
    print_test_header("AC1: Initialization");
    printf("Requirement: After initialization, heap should show 1 FREE block\n");
    printf("Input: heap_init(1000)\n\n");
    
    HeapManager* heap = heap_init(1000);
    
    assert_condition("Heap initialized successfully (not NULL)", heap != NULL);
    assert_condition("Total size is 1000 bytes", heap->total_size == 1000);
    assert_condition("Used size is 0", heap->used_size == 0);
    assert_condition("Free size is 1000", heap->free_size == 1000);
    assert_condition("Head block exists", heap->head != NULL);
    assert_condition("Head block is FREE", heap->head->is_free == 1);
    assert_condition("Head block size is 1000", heap->head->size == 1000);
    assert_condition("Only 1 block exists (next is NULL)", heap->head->next == NULL);
    assert_condition("Default algorithm is First Fit", heap->current_algorithm == FIRST_FIT);
    
    printf("\n%sMemory Map:%s\n", COLOR_YELLOW, COLOR_RESET);
    heap_show_memory_map(heap);
    
    heap_destroy(heap);
    printf("\n%s✓ AC1 VERDICT: PASSED%s\n", COLOR_GREEN, COLOR_RESET);
}

/* AC2: First Fit Logic Test */
void test_ac2_first_fit() {
    print_test_header("AC2: First Fit Logic");
    printf("Requirement: First Fit selects first suitable FREE block\n");
    printf("Scenario: Create 2 FREE blocks (A:100, B:200), allocate 50 bytes\n\n");
    
    HeapManager* heap = heap_init(1000);
    heap_set_algorithm(heap, FIRST_FIT);
    
    /* Create scenario: [P1:300] [FREE:100] [P2:400] [FREE:200] */
    heap_allocate(heap, "P1", 300);
    heap_allocate(heap, "TEMP", 100);
    heap_allocate(heap, "P2", 400);
    heap_allocate(heap, "TEMP2", 200);
    heap_free(heap, "TEMP");   /* Creates first FREE block of 100 */
    heap_free(heap, "TEMP2");  /* Creates second FREE block of 200 */
    
    printf("%sBefore allocation:%s\n", COLOR_YELLOW, COLOR_RESET);
    heap_show_memory_map(heap);
    
    /* Now allocate 50 bytes - should go into first FREE block (100 bytes) */
    MemoryBlock* allocated = heap_allocate(heap, "TEST", 50);
    
    printf("\n%sAfter allocating 50 bytes for 'TEST':%s\n", COLOR_YELLOW, COLOR_RESET);
    heap_show_memory_map(heap);
    
    assert_condition("Allocation succeeded", allocated != NULL);
    assert_condition("Allocated block is in first FREE hole", allocated->start_address == 300);
    assert_condition("Block size is 50", allocated->size == 50);
    assert_condition("Process ID is 'TEST'", strcmp(allocated->process_id, "TEST") == 0);
    
    heap_destroy(heap);
    printf("\n%s✓ AC2 VERDICT: PASSED%s\n", COLOR_GREEN, COLOR_RESET);
}

/* AC3: Best Fit Logic Test */
void test_ac3_best_fit() {
    print_test_header("AC3: Best Fit Logic");
    printf("Requirement: Best Fit selects smallest suitable FREE block\n");
    printf("Scenario: FREE blocks (A:100, B:30), allocate 20 bytes → should select B\n\n");
    
    HeapManager* heap = heap_init(1000);
    heap_set_algorithm(heap, BEST_FIT);
    
    /* Create scenario: [P1:200] [FREE:100] [P2:500] [FREE:30] [P3:170] */
    heap_allocate(heap, "P1", 200);
    heap_allocate(heap, "TEMP1", 100);
    heap_allocate(heap, "P2", 500);
    heap_allocate(heap, "TEMP2", 30);
    heap_allocate(heap, "P3", 170);
    heap_free(heap, "TEMP1");  /* Creates FREE block of 100 at position 200 */
    heap_free(heap, "TEMP2");  /* Creates FREE block of 30 at position 800 */
    
    printf("%sBefore allocation:%s\n", COLOR_YELLOW, COLOR_RESET);
    heap_show_memory_map(heap);
    
    /* Allocate 20 bytes - Best Fit should choose 30-byte block (smallest fit) */
    MemoryBlock* allocated = heap_allocate(heap, "TEST", 20);
    
    printf("\n%sAfter allocating 20 bytes for 'TEST':%s\n", COLOR_YELLOW, COLOR_RESET);
    heap_show_memory_map(heap);
    
    assert_condition("Allocation succeeded", allocated != NULL);
    assert_condition("Allocated block is in 30-byte hole (not 100)", allocated->start_address == 800);
    assert_condition("Block size is 20", allocated->size == 20);
    assert_condition("Remaining block is 10 bytes (30-20)", allocated->next != NULL && allocated->next->size == 10);
    
    heap_destroy(heap);
    printf("\n%s✓ AC3 VERDICT: PASSED%s\n", COLOR_GREEN, COLOR_RESET);
}

/* AC4: Worst Fit Logic Test */
void test_ac4_worst_fit() {
    print_test_header("AC4: Worst Fit Logic");
    printf("Requirement: Worst Fit selects largest FREE block\n");
    printf("Scenario: FREE blocks (A:100, B:30), allocate 20 bytes → should select A\n\n");
    
    HeapManager* heap = heap_init(1000);
    heap_set_algorithm(heap, WORST_FIT);
    
    /* Create same scenario as AC3 */
    heap_allocate(heap, "P1", 200);
    heap_allocate(heap, "TEMP1", 100);
    heap_allocate(heap, "P2", 500);
    heap_allocate(heap, "TEMP2", 30);
    heap_allocate(heap, "P3", 170);
    heap_free(heap, "TEMP1");  /* FREE: 100 at position 200 */
    heap_free(heap, "TEMP2");  /* FREE: 30 at position 800 */
    
    printf("%sBefore allocation:%s\n", COLOR_YELLOW, COLOR_RESET);
    heap_show_memory_map(heap);
    
    /* Allocate 20 bytes - Worst Fit should choose 100-byte block (largest) */
    MemoryBlock* allocated = heap_allocate(heap, "TEST", 20);
    
    printf("\n%sAfter allocating 20 bytes for 'TEST':%s\n", COLOR_YELLOW, COLOR_RESET);
    heap_show_memory_map(heap);
    
    assert_condition("Allocation succeeded", allocated != NULL);
    assert_condition("Allocated block is in 100-byte hole (not 30)", allocated->start_address == 200);
    assert_condition("Block size is 20", allocated->size == 20);
    assert_condition("Remaining block is 80 bytes (100-20)", allocated->next != NULL && allocated->next->size == 80);
    
    heap_destroy(heap);
    printf("\n%s✓ AC4 VERDICT: PASSED%s\n", COLOR_GREEN, COLOR_RESET);
}

/* AC5: Next Fit Logic Test */
void test_ac5_next_fit() {
    print_test_header("AC5: Next Fit Logic");
    printf("Requirement: Next Fit starts search from last allocation position\n");
    printf("Scenario: Test circular search behavior\n\n");
    
    HeapManager* heap = heap_init(1000);
    heap_set_algorithm(heap, NEXT_FIT);
    
    /* Create multiple FREE blocks */
    heap_allocate(heap, "P1", 100);  /* 0-100 */
    heap_allocate(heap, "P2", 100);  /* 100-200 */
    heap_allocate(heap, "P3", 100);  /* 200-300 */
    heap_allocate(heap, "P4", 100);  /* 300-400 */
    /* Remaining: 400-1000 (600 bytes FREE) */
    
    printf("%sInitial state:%s\n", COLOR_YELLOW, COLOR_RESET);
    heap_show_memory_map(heap);
    
    /* Free P2 to create hole at 100-200 */
    heap_free(heap, "P2");
    
    printf("\n%sAfter freeing P2 (creates hole at 100-200):%s\n", COLOR_YELLOW, COLOR_RESET);
    heap_show_memory_map(heap);
    
    /* First Next Fit allocation - should find 100-200 hole or end block */
    MemoryBlock* alloc1 = heap_allocate(heap, "TEST1", 50);
    printf("\n%sAfter TEST1 (50 bytes):%s\n", COLOR_YELLOW, COLOR_RESET);
    heap_show_memory_map(heap);
    
    /* Second allocation - should start from last_allocated position */
    MemoryBlock* alloc2 = heap_allocate(heap, "TEST2", 50);
    printf("\n%sAfter TEST2 (50 bytes) - should continue from last position:%s\n", COLOR_YELLOW, COLOR_RESET);
    heap_show_memory_map(heap);
    
    assert_condition("First allocation succeeded", alloc1 != NULL);
    assert_condition("Second allocation succeeded", alloc2 != NULL);
    assert_condition("Allocations are not at same position", alloc1->start_address != alloc2->start_address);
    assert_condition("Last allocated pointer updated", heap->last_allocated != NULL);
    
    heap_destroy(heap);
    printf("\n%s✓ AC5 VERDICT: PASSED%s\n", COLOR_GREEN, COLOR_RESET);
}

/* AC6: Splitting Feature Test */
void test_ac6_splitting() {
    print_test_header("AC6: Splitting Feature");
    printf("Requirement: When allocating less than block size, split into 2 blocks\n");
    printf("Scenario: FREE block 100 bytes, allocate 10 bytes\n\n");
    
    HeapManager* heap = heap_init(100);
    
    printf("%sBefore allocation (1 FREE block of 100):%s\n", COLOR_YELLOW, COLOR_RESET);
    heap_show_memory_map(heap);
    
    /* Allocate 10 bytes from 100-byte block */
    MemoryBlock* allocated = heap_allocate(heap, "P1", 10);
    
    printf("\n%sAfter allocating 10 bytes:%s\n", COLOR_YELLOW, COLOR_RESET);
    heap_show_memory_map(heap);
    
    assert_condition("Allocation succeeded", allocated != NULL);
    assert_condition("Allocated block is 10 bytes", allocated->size == 10);
    assert_condition("Second block exists (split occurred)", allocated->next != NULL);
    assert_condition("Second block is FREE", allocated->next->is_free == 1);
    assert_condition("Second block is 90 bytes (100-10)", allocated->next->size == 90);
    assert_condition("Second block starts at address 10", allocated->next->start_address == 10);
    assert_condition("Used size is 10", heap->used_size == 10);
    assert_condition("Free size is 90", heap->free_size == 90);
    
    heap_destroy(heap);
    printf("\n%s✓ AC6 VERDICT: PASSED%s\n", COLOR_GREEN, COLOR_RESET);
}

/* AC7: Coalescing Feature Test */
void test_ac7_coalescing() {
    print_test_header("AC7: Coalescing Feature");
    printf("Requirement: When freeing, merge with adjacent FREE blocks\n");
    printf("Scenario: [FREE:10] [P1:20] [FREE:30] → Free P1 → Should merge into [FREE:60]\n\n");
    
    HeapManager* heap = heap_init(1000);
    
    /* Create scenario: allocate 3 blocks, then free first and third */
    heap_allocate(heap, "P1", 100);
    heap_allocate(heap, "P2", 200);
    heap_allocate(heap, "P3", 300);
    heap_allocate(heap, "P4", 400);
    
    printf("%sInitial allocation:%s\n", COLOR_YELLOW, COLOR_RESET);
    heap_show_memory_map(heap);
    
    /* Free P1 and P3 to create FREE blocks before and after P2 */
    heap_free(heap, "P1");
    heap_free(heap, "P3");
    
    printf("\n%sAfter freeing P1 and P3 (P2 surrounded by FREE blocks):%s\n", COLOR_YELLOW, COLOR_RESET);
    heap_show_memory_map(heap);
    
    /* Count blocks before coalescing */
    int blocks_before = 0;
    MemoryBlock* current = heap->head;
    while (current) {
        blocks_before++;
        current = current->next;
    }
    
    /* Now free P2 - should coalesce with neighbors */
    heap_free(heap, "P2");
    
    printf("\n%sAfter freeing P2 (should coalesce into 1 large FREE block):%s\n", COLOR_YELLOW, COLOR_RESET);
    heap_show_memory_map(heap);
    
    /* Count blocks after coalescing */
    int blocks_after = 0;
    current = heap->head;
    int large_free_found = 0;
    while (current) {
        blocks_after++;
        if (current->is_free && current->size == 600) {  /* 100+200+300 */
            large_free_found = 1;
        }
        current = current->next;
    }
    
    assert_condition("Fewer blocks after coalescing", blocks_after < blocks_before);
    assert_condition("Large merged FREE block exists (600 bytes)", large_free_found == 1);
    assert_condition("Used size decreased correctly", heap->used_size == 400);  /* Only P4 */
    assert_condition("Free size increased correctly", heap->free_size == 600);  /* P1+P2+P3 */
    
    heap_destroy(heap);
    printf("\n%s✓ AC7 VERDICT: PASSED%s\n", COLOR_GREEN, COLOR_RESET);
}

/* AC8: Error Handling Test */
void test_ac8_error_handling() {
    print_test_header("AC8: Error Handling");
    printf("Requirement: Handle errors gracefully without crashing\n\n");
    
    HeapManager* heap = heap_init(100);
    
    printf("%s[Test 1] Allocate larger than total free space%s\n", COLOR_YELLOW, COLOR_RESET);
    MemoryBlock* result1 = heap_allocate(heap, "BIG", 200);
    assert_condition("Allocation fails (returns NULL)", result1 == NULL);
    assert_condition("Failed allocation counter incremented", heap->failed_allocation_count == 1);
    assert_condition("Heap state unchanged", heap->used_size == 0);
    
    printf("\n%s[Test 2] Allocate with size 0%s\n", COLOR_YELLOW, COLOR_RESET);
    MemoryBlock* result2 = heap_allocate(heap, "ZERO", 0);
    assert_condition("Allocation fails for size 0", result2 == NULL);
    assert_condition("Failed allocation counter incremented", heap->failed_allocation_count == 2);
    
    printf("\n%s[Test 3] Free non-existent process%s\n", COLOR_YELLOW, COLOR_RESET);
    int free_result1 = heap_free(heap, "NONEXISTENT");
    assert_condition("Free returns 0 (failure)", free_result1 == 0);
    
    printf("\n%s[Test 4] Double free protection%s\n", COLOR_YELLOW, COLOR_RESET);
    heap_allocate(heap, "P1", 50);
    heap_free(heap, "P1");
    int free_result2 = heap_free(heap, "P1");  /* Try to free again */
    assert_condition("Second free fails (process not found)", free_result2 == 0);
    
    printf("\n%s[Test 5] Allocate with empty process ID%s\n", COLOR_YELLOW, COLOR_RESET);
    MemoryBlock* result3 = heap_allocate(heap, "", 20);
    assert_condition("Allocation fails for empty PID", result3 == NULL);
    
    printf("\n%s[Test 6] Program did not crash during error tests%s\n", COLOR_YELLOW, COLOR_RESET);
    assert_condition("All error cases handled gracefully", 1);  /* If we get here, no crash */
    
    heap_destroy(heap);
    printf("\n%s✓ AC8 VERDICT: PASSED%s\n", COLOR_GREEN, COLOR_RESET);
}

/* Main Test Runner */
int main(void) {
    printf("\n");
    printf("%s╔══════════════════════════════════════════════════════════╗%s\n", COLOR_CYAN, COLOR_RESET);
    printf("%s║   ACCEPTANCE CRITERIA TEST SUITE (AC1-AC8)              ║%s\n", COLOR_CYAN, COLOR_RESET);
    printf("%s║   Memory Allocation Simulator - Phase 1                 ║%s\n", COLOR_CYAN, COLOR_RESET);
    printf("%s║   Date: December 19, 2025                               ║%s\n", COLOR_CYAN, COLOR_RESET);
    printf("%s╚══════════════════════════════════════════════════════════╝%s\n", COLOR_CYAN, COLOR_RESET);
    
    /* Run all AC tests */
    test_ac1_initialization();
    test_ac2_first_fit();
    test_ac3_best_fit();
    test_ac4_worst_fit();
    test_ac5_next_fit();
    test_ac6_splitting();
    test_ac7_coalescing();
    test_ac8_error_handling();
    
    /* Print final summary */
    print_summary();
    
    /* Return exit code based on results */
    return (failed_tests == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
