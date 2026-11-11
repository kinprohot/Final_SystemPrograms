#ifndef EXPORT_H
#define EXPORT_H

#include "../core/heap_manager.h"

/* Export current heap state to JSON file */
int export_heap_to_json(HeapManager* heap, const char* filename);

/* Export memory blocks detail to JSON */
int export_blocks_to_json(HeapManager* heap, const char* filename);

/* Export statistics summary to JSON */
int export_stats_to_json(HeapManager* heap, const char* filename);

/* Export complete snapshot (state + blocks + stats) */
int export_snapshot_to_json(HeapManager* heap, const char* filename);

/* Export iOS-themed interactive HTML snapshot report */
int export_snapshot_to_html(HeapManager* heap, const char* filename);

#endif /* EXPORT_H */
