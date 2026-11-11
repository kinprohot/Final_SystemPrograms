#include "benchmark.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <limits.h>
#include "../core/heap_manager.h"
#include "../core/algorithm_type.h"

/* Predefined scenarios */
BenchmarkScenario SCENARIO_RANDOM = {
    .name = "Random Mix",
    .description = "Random allocation/deallocation pattern",
    .heap_size = 10000,
    .operation_count = 1000,
    .alloc_min_size = 10,
    .alloc_max_size = 500,
    .alloc_probability = 0.6,
    .random_seed = 42
};

BenchmarkScenario SCENARIO_WORST_CASE = {
    .name = "Worst Case Fragmentation",
    .description = "Allocate small blocks, free alternating, try large alloc",
    .heap_size = 10000,
    .operation_count = 200,
    .alloc_min_size = 50,
    .alloc_max_size = 100,
    .alloc_probability = 0.7,
    .random_seed = 123
};

BenchmarkScenario SCENARIO_WEB_SERVER = {
    .name = "Web Server Simulation",
    .description = "Mix of short-lived and long-lived allocations",
    .heap_size = 50000,
    .operation_count = 2000,
    .alloc_min_size = 64,
    .alloc_max_size = 2048,
    .alloc_probability = 0.65,
    .random_seed = 789
};

/* Timer implementation */
void timer_start(Timer* timer) {
    clock_gettime(CLOCK_MONOTONIC, &timer->start);
}

double timer_stop(Timer* timer) {
    clock_gettime(CLOCK_MONOTONIC, &timer->end);
    double start_ms = timer->start.tv_sec * 1000.0 + timer->start.tv_nsec / 1000000.0;
    double end_ms = timer->end.tv_sec * 1000.0 + timer->end.tv_nsec / 1000000.0;
    return end_ms - start_ms;
}

/* Benchmark stats functions */
void benchmark_stats_init(BenchmarkStats* stats, const char* algo_name) {
    memset(stats, 0, sizeof(BenchmarkStats));
    stats->algorithm_name = algo_name;
    stats->min_allocation_time_ms = 1e9;  /* Very large initial value */
}

void benchmark_record_allocation(BenchmarkStats* stats, int success, double time_ms) {
    stats->total_allocations++;
    if (success) {
        stats->successful_allocations++;
        stats->total_allocation_time_ms += time_ms;
        if (time_ms > stats->max_allocation_time_ms) {
            stats->max_allocation_time_ms = time_ms;
        }
        if (time_ms < stats->min_allocation_time_ms) {
            stats->min_allocation_time_ms = time_ms;
        }
    } else {
        stats->failed_allocations++;
    }
}

void benchmark_record_deallocation(BenchmarkStats* stats, double time_ms) {
    stats->total_deallocations++;
    stats->total_deallocation_time_ms += time_ms;
}

void benchmark_finalize(BenchmarkStats* stats, HeapManager* heap) {
    /* Calculate averages */
    if (stats->successful_allocations > 0) {
        stats->avg_allocation_time_ms = stats->total_allocation_time_ms / stats->successful_allocations;
    }
    if (stats->total_deallocations > 0) {
        stats->avg_deallocation_time_ms = stats->total_deallocation_time_ms / stats->total_deallocations;
    }
    
    /* Get final heap state */
    stats->final_used_memory = heap->used_size;
    stats->final_free_memory = heap->free_size;
    
    /* Calculate fragmentation */
    if (heap->free_size > 0) {
        size_t largest_free = heap_get_largest_free_block(heap);
        stats->final_fragmentation_percent = 
            ((double)(heap->free_size - largest_free) / heap->free_size) * 100.0;
    }
}

/* Helper function to generate random size */
static size_t random_size(int min, int max) {
    return min + (rand() % (max - min + 1));
}

/* Helper function to pick random process to free */
static char* pick_random_process(HeapManager* heap) {
    /* Count allocated blocks */
    int count = 0;
    MemoryBlock* current = heap->head;
    while (current) {
        if (!current->is_free) {
            count++;
        }
        current = current->next;
    }
    
    if (count == 0) return NULL;
    
    /* Pick random one */
    int target = rand() % count;
    int index = 0;
    current = heap->head;
    while (current) {
        if (!current->is_free) {
            if (index == target) {
                static char pid[32];
                strncpy(pid, current->process_id, sizeof(pid) - 1);
                pid[sizeof(pid) - 1] = '\0';
                return pid;
            }
            index++;
        }
        current = current->next;
    }
    
    return NULL;
}

/* Run benchmark for single algorithm */
void benchmark_run_algorithm(AlgorithmType algo, BenchmarkScenario* scenario, BenchmarkStats* stats) {
    /* Algorithm name mapping */
    const char* algo_names[] = {"First Fit", "Next Fit", "Best Fit", "Worst Fit"};
    
    /* Initialize stats */
    benchmark_stats_init(stats, algo_names[algo]);
    
    /* Create heap */
    HeapManager* heap = heap_init(scenario->heap_size);
    if (!heap) {
        fprintf(stderr, "Failed to create heap for benchmark\n");
        return;
    }
    heap_set_algorithm(heap, algo);
    
    /* Seed random generator */
    srand(scenario->random_seed);
    
    /* Track allocated processes for selective freeing */
    int process_counter = 1;
    size_t peak_usage = 0;
    
    /* Run operations */
    for (int i = 0; i < scenario->operation_count; i++) {
        double prob = (double)rand() / RAND_MAX;
        
        if (prob < scenario->alloc_probability) {
            /* Allocation */
            size_t size = random_size(scenario->alloc_min_size, scenario->alloc_max_size);
            char pid[32];
            snprintf(pid, sizeof(pid), "P%d", process_counter++);
            
            Timer timer;
            timer_start(&timer);
            MemoryBlock* result = heap_allocate(heap, pid, size);
            double elapsed = timer_stop(&timer);
            
            benchmark_record_allocation(stats, result != NULL, elapsed);
            
            /* Track peak usage */
            if (heap->used_size > peak_usage) {
                peak_usage = heap->used_size;
            }
        } else {
            /* Deallocation */
            char* pid = pick_random_process(heap);
            if (pid) {
                Timer timer;
                timer_start(&timer);
                heap_free(heap, pid);
                double elapsed = timer_stop(&timer);
                
                benchmark_record_deallocation(stats, elapsed);
            }
        }
    }
    
    /* Finalize stats */
    stats->peak_memory_usage = peak_usage;
    benchmark_finalize(stats, heap);
    
    /* Cleanup */
    heap_destroy(heap);
}

/* Run benchmark for all algorithms */
void benchmark_compare_all(BenchmarkScenario* scenario, BenchmarkStats results[4]) {
    printf("Running benchmark: %s\n", scenario->name);
    printf("Description: %s\n", scenario->description);
    printf("Heap size: %zu bytes, Operations: %d\n\n", scenario->heap_size, scenario->operation_count);
    
    for (int algo = FIRST_FIT; algo <= WORST_FIT; algo++) {
        printf("Testing %s...\n", 
               algo == FIRST_FIT ? "First Fit" :
               algo == NEXT_FIT ? "Next Fit" :
               algo == BEST_FIT ? "Best Fit" : "Worst Fit");
        
        benchmark_run_algorithm(algo, scenario, &results[algo]);
    }
    
    printf("\nBenchmark completed!\n\n");
}

/* Print single benchmark result */
void benchmark_print_results(BenchmarkStats* stats) {
    printf("╔═══════════════════════════════════════════════════════════════════════╗\n");
    printf("║  BENCHMARK RESULTS: %-50s ║\n", stats->algorithm_name);
    printf("╠═══════════════════════════════════════════════════════════════════════╣\n");
    
    printf("║  Allocations:                                                          ║\n");
    printf("║    Total Attempts:        %-6d  Successful: %-6d  Failed: %-6d     ║\n",
           stats->total_allocations, stats->successful_allocations, stats->failed_allocations);
    printf("║    Success Rate:          %.1f%%                                      ║\n",
           (stats->total_allocations > 0 ? 
            (double)stats->successful_allocations / stats->total_allocations * 100 : 0));
    printf("║                                                                         ║\n");
    
    printf("║  Allocation Performance:                                                ║\n");
    printf("║    Average Time:          %.6f ms                                 ║\n", stats->avg_allocation_time_ms);
    printf("║    Min Time:              %.6f ms                                 ║\n", 
           stats->min_allocation_time_ms < 1e9 ? stats->min_allocation_time_ms : 0);
    printf("║    Max Time:              %.6f ms                                 ║\n", stats->max_allocation_time_ms);
    printf("║    Total Time:            %.3f ms                                  ║\n", stats->total_allocation_time_ms);
    printf("║                                                                         ║\n");
    
    printf("║  Deallocations:                                                         ║\n");
    printf("║    Total:                 %-6d                                         ║\n", stats->total_deallocations);
    printf("║    Average Time:          %.6f ms                                 ║\n", stats->avg_deallocation_time_ms);
    printf("║    Total Time:            %.3f ms                                  ║\n", stats->total_deallocation_time_ms);
    printf("║                                                                         ║\n");
    
    printf("║  Memory Statistics:                                                     ║\n");
    printf("║    Final Used:            %-10zu bytes                                ║\n", stats->final_used_memory);
    printf("║    Final Free:            %-10zu bytes                                ║\n", stats->final_free_memory);
    printf("║    Peak Usage:            %-10zu bytes                                ║\n", stats->peak_memory_usage);
    printf("║    Final Fragmentation:   %.2f%%                                      ║\n", stats->final_fragmentation_percent);
    
    printf("╚═══════════════════════════════════════════════════════════════════════╝\n\n");
}

/* Print comparison table */
void benchmark_print_comparison(BenchmarkStats results[4]) {
    printf("╔═══════════════════════════════════════════════════════════════════════════════════════════════════╗\n");
    printf("║                                    ALGORITHM COMPARISON                                            ║\n");
    printf("╠═════════════════╦════════════╦═════════════╦════════════╦═════════════╦══════════════╦═══════════╣\n");
    printf("║   Algorithm     ║  Success   ║   Avg Time  ║    Max     ║   Failed    ║  Peak Usage  ║   Frag    ║\n");
    printf("║                 ║    Rate    ║     (ms)    ║   (ms)     ║   Allocs    ║    (bytes)   ║    (%%)    ║\n");
    printf("╠═════════════════╬════════════╬═════════════╬════════════╬═════════════╬══════════════╬═══════════╣\n");
    
    for (int i = 0; i < 4; i++) {
        BenchmarkStats* s = &results[i];
        double success_rate = s->total_allocations > 0 ? 
            (double)s->successful_allocations / s->total_allocations * 100 : 0;
        
        printf("║ %-15s ║  %6.2f%%  ║  %9.6f  ║  %8.6f  ║    %6d   ║  %10zu  ║  %7.2f  ║\n",
               s->algorithm_name,
               success_rate,
               s->avg_allocation_time_ms,
               s->max_allocation_time_ms,
               s->failed_allocations,
               s->peak_memory_usage,
               s->final_fragmentation_percent);
    }
    
    printf("╚═════════════════╩════════════╩═════════════╩════════════╩═════════════╩══════════════╩═══════════╝\n\n");
    
    /* Find winners */
    int fastest = 0, most_reliable = 0, least_frag = 0;
    for (int i = 1; i < 4; i++) {
        if (results[i].avg_allocation_time_ms < results[fastest].avg_allocation_time_ms) {
            fastest = i;
        }
        if (results[i].failed_allocations < results[most_reliable].failed_allocations) {
            most_reliable = i;
        }
        if (results[i].final_fragmentation_percent < results[least_frag].final_fragmentation_percent) {
            least_frag = i;
        }
    }
    
    printf("╔═══════════════════════════════════════════════════════════════════════╗\n");
    printf("║                            SUMMARY                                     ║\n");
    printf("╠═══════════════════════════════════════════════════════════════════════╣\n");
    printf("║  🏆 Fastest Algorithm:         %-38s ║\n", results[fastest].algorithm_name);
    printf("║  🏆 Most Reliable:             %-38s ║\n", results[most_reliable].algorithm_name);
    printf("║  🏆 Least Fragmentation:       %-38s ║\n", results[least_frag].algorithm_name);
    printf("╚═══════════════════════════════════════════════════════════════════════╝\n");
}

/* Export to JSON */
int benchmark_export_json(const char* filename, BenchmarkStats results[4], BenchmarkScenario* scenario) {
    FILE* f = fopen(filename, "w");
    if (!f) {
        fprintf(stderr, "Failed to open %s for writing\n", filename);
        return -1;
    }
    
    fprintf(f, "{\n");
    fprintf(f, "  \"scenario\": {\n");
    fprintf(f, "    \"name\": \"%s\",\n", scenario->name);
    fprintf(f, "    \"description\": \"%s\",\n", scenario->description);
    fprintf(f, "    \"heap_size\": %zu,\n", scenario->heap_size);
    fprintf(f, "    \"operation_count\": %d,\n", scenario->operation_count);
    fprintf(f, "    \"alloc_min_size\": %d,\n", scenario->alloc_min_size);
    fprintf(f, "    \"alloc_max_size\": %d,\n", scenario->alloc_max_size);
    fprintf(f, "    \"alloc_probability\": %.2f\n", scenario->alloc_probability);
    fprintf(f, "  },\n");
    fprintf(f, "  \"results\": [\n");
    
    for (int i = 0; i < 4; i++) {
        BenchmarkStats* s = &results[i];
        fprintf(f, "    {\n");
        fprintf(f, "      \"algorithm\": \"%s\",\n", s->algorithm_name);
        fprintf(f, "      \"total_allocations\": %d,\n", s->total_allocations);
        fprintf(f, "      \"successful_allocations\": %d,\n", s->successful_allocations);
        fprintf(f, "      \"failed_allocations\": %d,\n", s->failed_allocations);
        fprintf(f, "      \"success_rate\": %.2f,\n", 
                s->total_allocations > 0 ? (double)s->successful_allocations / s->total_allocations * 100 : 0);
        fprintf(f, "      \"avg_allocation_time_ms\": %.6f,\n", s->avg_allocation_time_ms);
        fprintf(f, "      \"min_allocation_time_ms\": %.6f,\n", 
                s->min_allocation_time_ms < 1e9 ? s->min_allocation_time_ms : 0);
        fprintf(f, "      \"max_allocation_time_ms\": %.6f,\n", s->max_allocation_time_ms);
        fprintf(f, "      \"total_allocation_time_ms\": %.3f,\n", s->total_allocation_time_ms);
        fprintf(f, "      \"total_deallocations\": %d,\n", s->total_deallocations);
        fprintf(f, "      \"avg_deallocation_time_ms\": %.6f,\n", s->avg_deallocation_time_ms);
        fprintf(f, "      \"total_deallocation_time_ms\": %.3f,\n", s->total_deallocation_time_ms);
        fprintf(f, "      \"final_used_memory\": %zu,\n", s->final_used_memory);
        fprintf(f, "      \"final_free_memory\": %zu,\n", s->final_free_memory);
        fprintf(f, "      \"peak_memory_usage\": %zu,\n", s->peak_memory_usage);
        fprintf(f, "      \"final_fragmentation_percent\": %.2f\n", s->final_fragmentation_percent);
        fprintf(f, "    }%s\n", i < 3 ? "," : "");
    }
    
    fprintf(f, "  ]\n");
    fprintf(f, "}\n");
    
    fclose(f);
    printf("Benchmark results exported to: %s\n", filename);
    return 0;
}
