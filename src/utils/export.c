#include "export.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

/* Helper: Get algorithm name as string */
static const char* get_algo_name(AlgorithmType algo) {
    switch (algo) {
        case FIRST_FIT: return "First Fit";
        case NEXT_FIT: return "Next Fit";
        case BEST_FIT: return "Best Fit";
        case WORST_FIT: return "Worst Fit";
        default: return "Unknown";
    }
}

/* Helper: Get current timestamp */
static void get_timestamp(char* buffer, size_t size) {
    time_t now = time(NULL);
    struct tm* t = localtime(&now);
    strftime(buffer, size, "%Y-%m-%d %H:%M:%S", t);
}

/* Export heap state to JSON */
int export_heap_to_json(HeapManager* heap, const char* filename) {
    if (!heap || !filename) return -1;
    
    FILE* f = fopen(filename, "w");
    if (!f) {
        fprintf(stderr, "Failed to open %s for writing\n", filename);
        return -1;
    }
    
    char timestamp[64];
    get_timestamp(timestamp, sizeof(timestamp));
    
    fprintf(f, "{\n");
    fprintf(f, "  \"timestamp\": \"%s\",\n", timestamp);
    fprintf(f, "  \"algorithm\": \"%s\",\n", get_algo_name(heap->current_algorithm));
    fprintf(f, "  \"total_size\": %zu,\n", heap->total_size);
    fprintf(f, "  \"used_size\": %zu,\n", heap->used_size);
    fprintf(f, "  \"free_size\": %zu,\n", heap->free_size);
    fprintf(f, "  \"utilization_percent\": %.2f,\n", 
           (double)heap->used_size / heap->total_size * 100);
    
    /* Calculate fragmentation */
    size_t largest_free = heap_get_largest_free_block(heap);
    double fragmentation = 0;
    if (heap->free_size > 0) {
        fragmentation = ((double)(heap->free_size - largest_free) / heap->free_size) * 100.0;
    }
    fprintf(f, "  \"fragmentation_percent\": %.2f,\n", fragmentation);
    fprintf(f, "  \"largest_free_block\": %zu,\n", largest_free);
    
    /* Count blocks */
    int total_blocks = 0, free_blocks = 0, allocated_blocks = 0;
    MemoryBlock* current = heap->head;
    while (current) {
        total_blocks++;
        if (current->is_free) {
            free_blocks++;
        } else {
            allocated_blocks++;
        }
        current = current->next;
    }
    
    fprintf(f, "  \"total_blocks\": %d,\n", total_blocks);
    fprintf(f, "  \"free_blocks\": %d,\n", free_blocks);
    fprintf(f, "  \"allocated_blocks\": %d,\n", allocated_blocks);
    fprintf(f, "  \"allocation_count\": %d,\n", heap->allocation_count);
    fprintf(f, "  \"deallocation_count\": %d,\n", heap->deallocation_count);
    fprintf(f, "  \"failed_allocation_count\": %d\n", heap->failed_allocation_count);
    fprintf(f, "}\n");
    
    fclose(f);
    printf("Heap state exported to: %s\n", filename);
    return 0;
}

/* Export memory blocks detail to JSON */
int export_blocks_to_json(HeapManager* heap, const char* filename) {
    if (!heap || !filename) return -1;
    
    FILE* f = fopen(filename, "w");
    if (!f) {
        fprintf(stderr, "Failed to open %s for writing\n", filename);
        return -1;
    }
    
    char timestamp[64];
    get_timestamp(timestamp, sizeof(timestamp));
    
    fprintf(f, "{\n");
    fprintf(f, "  \"timestamp\": \"%s\",\n", timestamp);
    fprintf(f, "  \"algorithm\": \"%s\",\n", get_algo_name(heap->current_algorithm));
    fprintf(f, "  \"blocks\": [\n");
    
    MemoryBlock* current = heap->head;
    int first = 1;
    while (current) {
        if (!first) fprintf(f, ",\n");
        first = 0;
        
        fprintf(f, "    {\n");
        fprintf(f, "      \"start_address\": %zu,\n", current->start_address);
        fprintf(f, "      \"size\": %zu,\n", current->size);
        fprintf(f, "      \"end_address\": %zu,\n", current->start_address + current->size);
        fprintf(f, "      \"is_free\": %s", current->is_free ? "true" : "false");
        
        if (!current->is_free) {
            fprintf(f, ",\n");
            fprintf(f, "      \"process_id\": \"%s\"\n", current->process_id);
        } else {
            fprintf(f, "\n");
        }
        
        fprintf(f, "    }");
        current = current->next;
    }
    
    fprintf(f, "\n  ]\n");
    fprintf(f, "}\n");
    
    fclose(f);
    printf("Memory blocks exported to: %s\n", filename);
    return 0;
}

/* Export statistics summary to JSON */
int export_stats_to_json(HeapManager* heap, const char* filename) {
    if (!heap || !filename) return -1;
    
    FILE* f = fopen(filename, "w");
    if (!f) {
        fprintf(stderr, "Failed to open %s for writing\n", filename);
        return -1;
    }
    
    char timestamp[64];
    get_timestamp(timestamp, sizeof(timestamp));
    
    /* Calculate statistics */
    size_t largest_free = heap_get_largest_free_block(heap);
    double fragmentation = 0;
    if (heap->free_size > 0) {
        fragmentation = ((double)(heap->free_size - largest_free) / heap->free_size) * 100.0;
    }
    
    double utilization = (double)heap->used_size / heap->total_size * 100;
    double success_rate = 0;
    int total_attempts = heap->allocation_count + heap->failed_allocation_count;
    if (total_attempts > 0) {
        success_rate = (double)heap->allocation_count / total_attempts * 100;
    }
    
    fprintf(f, "{\n");
    fprintf(f, "  \"timestamp\": \"%s\",\n", timestamp);
    fprintf(f, "  \"algorithm\": \"%s\",\n", get_algo_name(heap->current_algorithm));
    fprintf(f, "  \"memory\": {\n");
    fprintf(f, "    \"total\": %zu,\n", heap->total_size);
    fprintf(f, "    \"used\": %zu,\n", heap->used_size);
    fprintf(f, "    \"free\": %zu,\n", heap->free_size);
    fprintf(f, "    \"utilization_percent\": %.2f\n", utilization);
    fprintf(f, "  },\n");
    fprintf(f, "  \"fragmentation\": {\n");
    fprintf(f, "    \"external_percent\": %.2f,\n", fragmentation);
    fprintf(f, "    \"largest_free_block\": %zu,\n", largest_free);
    fprintf(f, "    \"severity\": \"%s\"\n",
           fragmentation < 10 ? "Low" :
           fragmentation < 30 ? "Moderate" : "High");
    fprintf(f, "  },\n");
    fprintf(f, "  \"operations\": {\n");
    fprintf(f, "    \"allocations\": %d,\n", heap->allocation_count);
    fprintf(f, "    \"deallocations\": %d,\n", heap->deallocation_count);
    fprintf(f, "    \"failed_allocations\": %d,\n", heap->failed_allocation_count);
    fprintf(f, "    \"total_attempts\": %d,\n", total_attempts);
    fprintf(f, "    \"success_rate_percent\": %.2f\n", success_rate);
    fprintf(f, "  }\n");
    fprintf(f, "}\n");
    
    fclose(f);
    printf("Statistics exported to: %s\n", filename);
    return 0;
}

/* Export complete snapshot */
int export_snapshot_to_json(HeapManager* heap, const char* filename) {
    if (!heap || !filename) return -1;
    
    FILE* f = fopen(filename, "w");
    if (!f) {
        fprintf(stderr, "Failed to open %s for writing\n", filename);
        return -1;
    }
    
    char timestamp[64];
    get_timestamp(timestamp, sizeof(timestamp));
    
    /* Calculate statistics */
    size_t largest_free = heap_get_largest_free_block(heap);
    double fragmentation = 0;
    if (heap->free_size > 0) {
        fragmentation = ((double)(heap->free_size - largest_free) / heap->free_size) * 100.0;
    }
    
    fprintf(f, "{\n");
    fprintf(f, "  \"snapshot\": {\n");
    fprintf(f, "    \"timestamp\": \"%s\",\n", timestamp);
    fprintf(f, "    \"algorithm\": \"%s\"\n", get_algo_name(heap->current_algorithm));
    fprintf(f, "  },\n");
    
    /* State */
    fprintf(f, "  \"state\": {\n");
    fprintf(f, "    \"total_size\": %zu,\n", heap->total_size);
    fprintf(f, "    \"used_size\": %zu,\n", heap->used_size);
    fprintf(f, "    \"free_size\": %zu,\n", heap->free_size);
    fprintf(f, "    \"utilization_percent\": %.2f,\n",
           (double)heap->used_size / heap->total_size * 100);
    fprintf(f, "    \"fragmentation_percent\": %.2f,\n", fragmentation);
    fprintf(f, "    \"largest_free_block\": %zu\n", largest_free);
    fprintf(f, "  },\n");
    
    /* Operations */
    fprintf(f, "  \"operations\": {\n");
    fprintf(f, "    \"allocations\": %d,\n", heap->allocation_count);
    fprintf(f, "    \"deallocations\": %d,\n", heap->deallocation_count);
    fprintf(f, "    \"failed_allocations\": %d\n", heap->failed_allocation_count);
    fprintf(f, "  },\n");
    
    /* Blocks */
    fprintf(f, "  \"blocks\": [\n");
    MemoryBlock* current = heap->head;
    int first = 1;
    while (current) {
        if (!first) fprintf(f, ",\n");
        first = 0;
        
        fprintf(f, "    {\n");
        fprintf(f, "      \"start\": %zu,\n", current->start_address);
        fprintf(f, "      \"size\": %zu,\n", current->size);
        fprintf(f, "      \"end\": %zu,\n", current->start_address + current->size);
        fprintf(f, "      \"free\": %s", current->is_free ? "true" : "false");
        
        if (!current->is_free) {
            fprintf(f, ",\n");
            fprintf(f, "      \"process\": \"%s\"\n", current->process_id);
        } else {
            fprintf(f, "\n");
        }
        
        fprintf(f, "    }");
        current = current->next;
    }
    fprintf(f, "\n  ]\n");
    fprintf(f, "}\n");
    
    fclose(f);
    printf("Complete snapshot exported to: %s\n", filename);
    return 0;
}

/* Export iOS-themed interactive HTML snapshot report */
int export_snapshot_to_html(HeapManager* heap, const char* filename) {
    if (!heap || !filename) return -1;
    
    FILE* f = fopen(filename, "w");
    if (!f) {
        fprintf(stderr, "Failed to open %s for writing\n", filename);
        return -1;
    }
    
    char timestamp[64];
    get_timestamp(timestamp, sizeof(timestamp));
    
    size_t largest_free = heap_get_largest_free_block(heap);
    double fragmentation = 0;
    if (heap->free_size > 0) {
        fragmentation = ((double)(heap->free_size - largest_free) / heap->free_size) * 100.0;
    }
    double utilization = (double)heap->used_size / heap->total_size * 100;
    int total_attempts = heap->allocation_count + heap->failed_allocation_count;
    double success_rate = (total_attempts > 0) ? ((double)heap->allocation_count / total_attempts * 100) : 100.0;

    fprintf(f, "<!DOCTYPE html>\n<html lang=\"en\">\n<head>\n");
    fprintf(f, "  <meta charset=\"UTF-8\">\n  <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n");
    fprintf(f, "  <title>MemoryOS Report - %s</title>\n", timestamp);
    fprintf(f, "  <style>\n");
    fprintf(f, "    :root { --bg: #F2F2F7; --card: #FFFFFF; --text: #000000; --sub: #8E8E93; --blue: #007AFF; --green: #34C759; --orange: #FF9500; --red: #FF3B30; --sep: rgba(60,60,67,0.12); }\n");
    fprintf(f, "    @media (prefers-color-scheme: dark) { :root { --bg: #000000; --card: #1C1C1E; --text: #FFFFFF; --sub: #8E8E93; --sep: rgba(84,84,88,0.36); } }\n");
    fprintf(f, "    body { font-family: -apple-system, BlinkMacSystemFont, 'SF Pro Display', system-ui, sans-serif; background: var(--bg); color: var(--text); padding: 24px 16px; margin: 0; display: flex; flex-direction: column; align-items: center; }\n");
    fprintf(f, "    .container { width: 100%%; max-width: 680px; display: flex; flex-direction: column; gap: 16px; }\n");
    fprintf(f, "    .header { text-align: center; margin-bottom: 8px; }\n");
    fprintf(f, "    .header h1 { margin: 0; font-size: 26px; font-weight: 700; letter-spacing: -0.4px; }\n");
    fprintf(f, "    .header p { margin: 4px 0 0; font-size: 13px; color: var(--sub); }\n");
    fprintf(f, "    .badge { display: inline-block; padding: 3px 10px; border-radius: 99px; background: rgba(0,122,255,0.12); color: var(--blue); font-size: 12px; font-weight: 600; margin-top: 6px; }\n");
    fprintf(f, "    .card { background: var(--card); border-radius: 16px; padding: 16px; border: 1px solid var(--sep); box-shadow: 0 4px 16px rgba(0,0,0,0.04); }\n");
    fprintf(f, "    .storage-bar { width: 100%%; height: 26px; border-radius: 8px; background: rgba(142,142,147,0.2); overflow: hidden; display: flex; margin: 12px 0; }\n");
    fprintf(f, "    .storage-seg { height: 100%%; }\n");
    fprintf(f, "    .grid { display: grid; grid-template-columns: 1fr 1fr; gap: 10px; }\n");
    fprintf(f, "    .metric { background: var(--card); border-radius: 12px; padding: 12px; border: 1px solid var(--sep); }\n");
    fprintf(f, "    .metric .lbl { font-size: 11px; color: var(--sub); text-transform: uppercase; font-weight: 600; }\n");
    fprintf(f, "    .metric .val { font-size: 22px; font-weight: 700; font-family: ui-monospace, monospace; margin-top: 2px; }\n");
    fprintf(f, "    .block-row { display: flex; justify-content: space-between; padding: 8px 0; border-bottom: 0.5px solid var(--sep); font-size: 13px; font-family: ui-monospace, monospace; }\n");
    fprintf(f, "  </style>\n</head>\n<body>\n");
    fprintf(f, "  <div class=\"container\">\n");
    fprintf(f, "    <div class=\"header\">\n");
    fprintf(f, "      <h1>Memory Allocation Snapshot</h1>\n");
    fprintf(f, "      <span class=\"badge\">Apple iOS HIG Theme &bull; %s</span>\n", get_algo_name(heap->current_algorithm));
    fprintf(f, "      <p>Report generated at %s</p>\n", timestamp);
    fprintf(f, "    </div>\n");

    /* Storage Bar Card */
    fprintf(f, "    <div class=\"card\">\n");
    fprintf(f, "      <div style=\"display: flex; justify-content: space-between; font-weight: 600;\">\n");
    fprintf(f, "        <span>iPhone Storage Bar</span>\n");
    fprintf(f, "        <span style=\"color: var(--blue);\">%.1f%% Used</span>\n", utilization);
    fprintf(f, "      </div>\n");
    fprintf(f, "      <div class=\"storage-bar\">\n");

    static const char* palette[] = {"#007AFF", "#5856D6", "#AF52DE", "#FF2D55", "#FF9500", "#30B0C7", "#34C759"};
    int color_idx = 0;
    MemoryBlock* cur = heap->head;
    while (cur) {
        double pct = (double)cur->size / heap->total_size * 100.0;
        if (cur->is_free) {
            fprintf(f, "        <div class=\"storage-seg\" style=\"width: %.2f%%; background: repeating-linear-gradient(45deg, rgba(142,142,147,0.2), rgba(142,142,147,0.2) 4px, rgba(142,142,147,0.3) 4px, rgba(142,142,147,0.3) 8px);\" title=\"FREE %zu KB\"></div>\n", pct, cur->size);
        } else {
            const char* col = palette[color_idx % 7];
            color_idx++;
            fprintf(f, "        <div class=\"storage-seg\" style=\"width: %.2f%%; background: %s;\" title=\"%s: %zu KB\"></div>\n", pct, col, cur->process_id, cur->size);
        }
        cur = cur->next;
    }
    fprintf(f, "      </div>\n");
    fprintf(f, "      <div style=\"display: flex; justify-content: space-between; font-size: 12px; color: var(--sub);\">\n");
    fprintf(f, "        <span>Used: %zu KB</span><span>Free: %zu KB</span><span>Total: %zu KB</span>\n", heap->used_size, heap->free_size, heap->total_size);
    fprintf(f, "      </div>\n");
    fprintf(f, "    </div>\n");

    /* Metrics Grid */
    fprintf(f, "    <div class=\"grid\">\n");
    fprintf(f, "      <div class=\"metric\"><div class=\"lbl\">External Fragmentation</div><div class=\"val\" style=\"color: %s;\">%.1f%%</div></div>\n",
            fragmentation < 15 ? "var(--green)" : (fragmentation < 35 ? "var(--orange)" : "var(--red)"), fragmentation);
    fprintf(f, "      <div class=\"metric\"><div class=\"lbl\">Allocation Success Rate</div><div class=\"val\" style=\"color: var(--green);\">%.1f%%</div></div>\n", success_rate);
    fprintf(f, "      <div class=\"metric\"><div class=\"lbl\">Largest Free Block</div><div class=\"val\">%zu KB</div></div>\n", largest_free);
    fprintf(f, "      <div class=\"metric\"><div class=\"lbl\">Allocations / Fails</div><div class=\"val\">%d / %d</div></div>\n", heap->allocation_count, heap->failed_allocation_count);
    fprintf(f, "    </div>\n");

    /* Block Details */
    fprintf(f, "    <div class=\"card\">\n");
    fprintf(f, "      <div style=\"font-size: 14px; font-weight: 600; margin-bottom: 8px;\">Memory Blocks List (Doubly-Linked List)</div>\n");
    cur = heap->head;
    while (cur) {
        fprintf(f, "      <div class=\"block-row\">\n");
        if (cur->is_free) {
            fprintf(f, "        <span style=\"color: var(--sub);\">[FREE] %zu - %zu</span><span>%zu KB</span>\n", cur->start_address, cur->start_address + cur->size, cur->size);
        } else {
            fprintf(f, "        <span style=\"color: var(--blue); font-weight: 600;\">[%s] %zu - %zu</span><span>%zu KB</span>\n", cur->process_id, cur->start_address, cur->start_address + cur->size, cur->size);
        }
        fprintf(f, "      </div>\n");
        cur = cur->next;
    }
    fprintf(f, "    </div>\n");
    fprintf(f, "  </div>\n</body>\n</html>\n");

    fclose(f);
    printf("iOS-themed HTML report exported to: %s\n", filename);
    return 0;
}
