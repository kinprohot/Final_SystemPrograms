#include <sys/types.h>
#include <stddef.h>

#include "next_fit.h"

MemoryBlock* next_fit_find(HeapManager* heap, size_t size) {
    MemoryBlock* start = NULL;
    MemoryBlock* current = NULL;

    if (heap == NULL || heap->head == NULL || size == 0) {
        return NULL;
    }

    if (heap->last_allocated != NULL && heap->last_allocated->next != NULL) {
        start = heap->last_allocated->next;
    } else {
        start = heap->head;
    }

    current = start;
    while (current != NULL) {
        if (current->is_free && current->size >= size) {
            return current;
        }
        current = current->next;
    }

    current = heap->head;
    while (current != NULL && current != start) {
        if (current->is_free && current->size >= size) {
            return current;
        }
        current = current->next;
    }

    return NULL;
}
