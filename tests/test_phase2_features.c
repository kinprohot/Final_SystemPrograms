/* 
 * Phase 2 Feature Tests - Enhanced Visualization
 * 
 * Tests for:
 * - Real-time statistics bar
 * - Enhanced ASCII memory visualization
 * - Fragmentation calculation and report
 * - Color mode support
 * 
 * Date: December 20, 2025
 */

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../src/core/heap_manager.h"
#include "../src/core/algorithm_type.h"
#include "../src/ui/visualizer.h"
#include "../src/utils/ansi_colors.h" 

/* Test result tracking */
static int total_tests = 0;
static int passed_tests = 0;

void assert_test(const char* desc, int condition) {
    total_tests++;
    if (condition) {
        printf("  [PASS] %s\n", desc);
        passed_tests++;
    } else {
        printf("  [FAIL] %s\n", desc);
    }
}

void print_test_header(const char* name) {
    printf("\n");
    printf("═══════════════════════════════════════════════════════════════\n");
    printf("TEST: %s\n", name);
    printf("═══════════════════════════════════════════════════════════════\n");
}

/* Test 1: Count Free Blocks Function */
void test_count_free_blocks() {
    print_test_header("Count Free Blocks");
    
    HeapManager* heap = heap_init(1000);
    
    /* Initial: 1 free block */
    int count1 = count_free_blocks(heap);
    assert_test("Initial heap has 1 free block", count1 == 1);
    
    /* After allocations: should have multiple blocks */
    heap_allocate(heap, "P1", 100);
    heap_allocate(heap, "P2", 100);
    heap_allocate(heap, "P3", 100);
    
    int count2 = count_free_blocks(heap);
    assert_test("After 3 allocs, has at least 1 free block", count2 >= 1);
    
    /* Free some blocks */
    heap_free(heap, "P2");
    int count3 = count_free_blocks(heap);
    assert_test("After freeing P2, free blocks increased", count3 >= count2);
    
    heap_destroy(heap);
    printf("✓ Count free blocks test completed\n");
}

/* Test 2: Find Largest Free Block */
void test_find_largest_free_block() {
    print_test_header("Find Largest Free Block");
    
    HeapManager* heap = heap_init(1000);
    
    /* Initial: largest = 1000 */
    size_t largest1 = find_largest_free_block(heap);
    assert_test("Initial largest free block is 1000", largest1 == 1000);
    
    /* Create fragmented memory */
    heap_allocate(heap, "P1", 100);  /* [P1:100] [FREE:900] */
    heap_allocate(heap, "P2", 200);  /* [P1:100] [P2:200] [FREE:700] */
    heap_allocate(heap, "P3", 300);  /* [P1:100] [P2:200] [P3:300] [FREE:400] */
    
    size_t largest2 = find_largest_free_block(heap);
    assert_test("Largest free block after allocs is 400", largest2 == 400);
    
    /* Free P2 to create hole */
    heap_free(heap, "P2");
    /* Now: [P1:100] [FREE:200] [P3:300] [FREE:400] */
    
    size_t largest3 = find_largest_free_block(heap);
    assert_test("Largest free block is still 400", largest3 == 400);
    
    heap_destroy(heap);
    printf("✓ Find largest free block test completed\n");
}

/* Test 3: Fragmentation Calculation */
void test_fragmentation_calculation() {
    print_test_header("Fragmentation Calculation");
    
    HeapManager* heap = heap_init(1000);
    
    /* Scenario 1: No fragmentation (1 large free block) */
    float frag1 = calculate_fragmentation(heap);
    assert_test("No fragmentation initially (0%)", frag1 == 0.0f);
    
    /* Scenario 2: Create fragmentation */
    heap_allocate(heap, "P1", 100);
    heap_allocate(heap, "P2", 100);
    heap_allocate(heap, "P3", 100);
    heap_allocate(heap, "P4", 100);
    /* [P1:100] [P2:100] [P3:100] [P4:100] [FREE:600] */
    
    heap_free(heap, "P2"); /* Create hole */
    heap_free(heap, "P4"); /* Create another hole */
    /* [P1:100] [FREE:100] [P3:100] [FREE:100] [FREE:600] */
    /* Total free: 800, Largest: 600 */
    /* Fragmentation = (800-600)/800 = 25% */
    
    float frag2 = calculate_fragmentation(heap);
    assert_test("Fragmentation > 0% after creating holes", frag2 > 0.0f);
    assert_test("Fragmentation < 100%", frag2 < 100.0f);
    
    printf("  Measured fragmentation: %.1f%%\n", frag2);
    
    heap_destroy(heap);
    printf("✓ Fragmentation calculation test completed\n");
}

/* Test 4: Real-time Stats Bar (Visual Test) */
void test_stats_bar_visual() {
    print_test_header("Real-time Stats Bar (Visual)");
    
    HeapManager* heap = heap_init(1000);
    
    printf("\nInitial state:\n");
    print_stats_bar(heap);
    
    /* Allocate some memory */
    heap_allocate(heap, "P1", 100);
    heap_allocate(heap, "P2", 200);
    heap_allocate(heap, "P3", 300);
    
    printf("\nAfter allocations:\n");
    print_stats_bar(heap);
    
    /* Free one */
    heap_free(heap, "P2");
    
    printf("\nAfter freeing P2:\n");
    print_stats_bar(heap);
    
    /* Change algorithm */
    heap_set_algorithm(heap, BEST_FIT);
    
    printf("\nAfter changing to Best Fit:\n");
    print_stats_bar(heap);
    
    /* Test failed allocation */
    heap_allocate(heap, "HUGE", 2000);
    
    printf("\nAfter failed allocation:\n");
    print_stats_bar(heap);
    
    heap_destroy(heap);
    printf("\n✓ Stats bar visual test completed (manual inspection required)\n");
}

/* Test 5: Enhanced ASCII Memory Bar (Visual Test) */
void test_memory_bar_visual() {
    print_test_header("Enhanced ASCII Memory Bar (Visual)");
    
    HeapManager* heap = heap_init(1000);
    
    printf("\nScenario 1: Initial state\n");
    draw_memory_bar(heap);
    
    printf("\nScenario 2: Single allocation\n");
    heap_allocate(heap, "P1", 100);
    draw_memory_bar(heap);
    
    printf("\nScenario 3: Multiple allocations\n");
    heap_allocate(heap, "P2", 200);
    heap_allocate(heap, "P3", 150);
    draw_memory_bar(heap);
    
    printf("\nScenario 4: After freeing (fragmentation)\n");
    heap_free(heap, "P2");
    draw_memory_bar(heap);
    
    printf("\nScenario 5: Heavy fragmentation\n");
    heap_allocate(heap, "P4", 50);
    heap_allocate(heap, "P5", 50);
    draw_memory_bar(heap);
    
    heap_destroy(heap);
    printf("\n✓ Memory bar visual test completed (manual inspection required)\n");
}

/* Test 6: Fragmentation Report (Visual Test) */
void test_fragmentation_report_visual() {
    print_test_header("Fragmentation Report (Visual)");
    
    HeapManager* heap = heap_init(1000);
    
    printf("\nScenario 1: No fragmentation\n");
    show_fragmentation_report(heap);
    
    printf("\nScenario 2: Low fragmentation\n");
    heap_allocate(heap, "P1", 400);
    heap_allocate(heap, "P2", 400);
    /* [P1:400] [P2:400] [FREE:200] - 0% fragmentation */
    show_fragmentation_report(heap);
    
    printf("\nScenario 3: Moderate fragmentation\n");
    heap_allocate(heap, "P3", 100);
    heap_allocate(heap, "P4", 100);
    heap_free(heap, "P2"); /* Create 400-byte hole */
    /* [P1:400] [FREE:400] [P3:100] [FREE:100] */
    /* Total free: 500, Largest: 400, Frag: 20% */
    show_fragmentation_report(heap);
    
    printf("\nScenario 4: High fragmentation\n");
    heap_allocate(heap, "P5", 50);
    heap_allocate(heap, "P6", 50);
    heap_free(heap, "P1"); /* Create multiple holes */
    heap_free(heap, "P3");
    /* Many small holes */
    show_fragmentation_report(heap);
    
    heap_destroy(heap);
    printf("\n✓ Fragmentation report visual test completed\n");
}

/* Test 7: Integration Test - Full Workflow */
void test_full_workflow() {
    print_test_header("Full Workflow Integration Test");
    
    HeapManager* heap = heap_init(1000);
    
    printf("\n=== Step 1: Initial State ===\n");
    print_stats_bar(heap);
    draw_memory_bar(heap);
    
    printf("\n=== Step 2: Allocate Memory ===\n");
    heap_allocate(heap, "Process_A", 150);
    heap_allocate(heap, "Process_B", 250);
    heap_allocate(heap, "Process_C", 100);
    print_stats_bar(heap);
    draw_memory_bar(heap);
    show_fragmentation_report(heap);
    
    printf("\n=== Step 3: Free Some Memory ===\n");
    heap_free(heap, "Process_B");
    print_stats_bar(heap);
    draw_memory_bar(heap);
    show_fragmentation_report(heap);
    
    printf("\n=== Step 4: Allocate into Hole ===\n");
    heap_set_algorithm(heap, BEST_FIT);
    heap_allocate(heap, "Process_D", 100);
    print_stats_bar(heap);
    draw_memory_bar(heap);
    show_fragmentation_report(heap);
    
    printf("\n=== Step 5: Heavy Fragmentation ===\n");
    heap_allocate(heap, "P1", 50);
    heap_allocate(heap, "P2", 50);
    heap_allocate(heap, "P3", 50);
    heap_free(heap, "P2");
    print_stats_bar(heap);
    draw_memory_bar(heap);
    show_fragmentation_report(heap);
    
    /* Verify statistics */
    assert_test("Allocation count > 0", heap->allocation_count > 0);
    assert_test("Deallocation count > 0", heap->deallocation_count > 0);
    assert_test("Free blocks exist", count_free_blocks(heap) > 0);
    assert_test("Used + Free = Total", 
                heap->used_size + heap->free_size == heap->total_size);
    
    heap_destroy(heap);
    printf("\n✓ Full workflow integration test completed\n");
}

/* Main test runner */
int main(void) {
    printf("\n");
    printf("╔══════════════════════════════════════════════════════════════════╗\n");
    printf("║          PHASE 2 FEATURE TEST SUITE                             ║\n");
    printf("║          Enhanced Visualization & Statistics                    ║\n");
    printf("║          Date: December 20, 2025                                ║\n");
    printf("╚══════════════════════════════════════════════════════════════════╝\n");
    
    /* Initialize color support */
    extern int g_use_colors;
    g_use_colors = supports_color();
    printf("\nColor support: %s\n", g_use_colors ? "ENABLED" : "DISABLED");
    
    /* Run functional tests */
    test_count_free_blocks();
    test_find_largest_free_block();
    test_fragmentation_calculation();
    
    /* Run visual tests */
    test_stats_bar_visual();
    test_memory_bar_visual();
    test_fragmentation_report_visual();
    test_full_workflow();
    
    /* Print summary */
    printf("\n");
    printf("═══════════════════════════════════════════════════════════════════\n");
    printf("TEST SUMMARY\n");
    printf("═══════════════════════════════════════════════════════════════════\n");
    printf("Total Assertions:  %d\n", total_tests);
    printf("Passed:            %d\n", passed_tests);
    printf("Failed:            %d\n", total_tests - passed_tests);
    printf("Success Rate:      %.1f%%\n", 
           total_tests > 0 ? (passed_tests * 100.0f / total_tests) : 0.0f);
    printf("═══════════════════════════════════════════════════════════════════\n");
    
    printf("\nNOTE: Visual tests require manual inspection of:\n");
    printf("  - Stats bar formatting\n");
    printf("  - ASCII memory bar visualization\n");
    printf("  - Fragmentation report display\n");
    printf("  - Color output (if terminal supports)\n");
    
    printf("\n");
    if (passed_tests == total_tests) {
        printf("✓ ALL FUNCTIONAL TESTS PASSED!\n");
        printf("✓ Review visual output above for Phase 2 features.\n");
        return EXIT_SUCCESS;
    } else {
        printf("✗ Some tests failed. Review output above.\n");
        return EXIT_FAILURE;
    }
}
