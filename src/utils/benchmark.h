#ifndef BENCHMARK_H
#define BENCHMARK_H

#include <stddef.h>
#include <time.h>
#include "../core/heap_manager.h"

/* Benchmark statistics for a single algorithm */
typedef struct {
    const char* algorithm_name;
    int total_allocations;
    int successful_allocations;
    int failed_allocations;
    double total_allocation_time_ms;
    double avg_allocation_time_ms;
    double max_allocation_time_ms;
    double min_allocation_time_ms;
    int total_deallocations;
    double total_deallocation_time_ms;
    double avg_deallocation_time_ms;
    double final_fragmentation_percent;
    size_t final_used_memory;
    size_t final_free_memory;
    size_t peak_memory_usage;
} BenchmarkStats;

/* Benchmark scenario configuration */
typedef struct {
    const char* name;
    const char* description;
    size_t heap_size;
    int operation_count;
    int alloc_min_size;
    int alloc_max_size;
    double alloc_probability;  /* 0.0-1.0, probability of alloc vs free */
    unsigned int random_seed;
} BenchmarkScenario;

/* Timer utilities */
typedef struct {
    struct timespec start;
    struct timespec end;
} Timer;

/* Initialize timer */
void timer_start(Timer* timer);

/* Stop timer and return elapsed time in milliseconds */
double timer_stop(Timer* timer);

/* Initialize benchmark stats */
void benchmark_stats_init(BenchmarkStats* stats, const char* algo_name);

/* Update stats with allocation operation */
void benchmark_record_allocation(BenchmarkStats* stats, int success, double time_ms);

/* Update stats with deallocation operation */
void benchmark_record_deallocation(BenchmarkStats* stats, double time_ms);

/* Finalize stats with heap state */
void benchmark_finalize(BenchmarkStats* stats, HeapManager* heap);

/* Run benchmark for single algorithm with given scenario */
void benchmark_run_algorithm(AlgorithmType algo, BenchmarkScenario* scenario, BenchmarkStats* stats);

/* Run benchmark for all algorithms and compare */
void benchmark_compare_all(BenchmarkScenario* scenario, BenchmarkStats results[4]);

/* Print benchmark results to console */
void benchmark_print_results(BenchmarkStats* stats);

/* Print comparison table for all algorithms */
void benchmark_print_comparison(BenchmarkStats results[4]);

/* Export benchmark results to JSON file */
int benchmark_export_json(const char* filename, BenchmarkStats results[4], BenchmarkScenario* scenario);

/* Predefined benchmark scenarios */
extern BenchmarkScenario SCENARIO_RANDOM;
extern BenchmarkScenario SCENARIO_WORST_CASE;
extern BenchmarkScenario SCENARIO_WEB_SERVER;

#endif /* BENCHMARK_H */
