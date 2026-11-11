#include <sys/types.h>
#include <stddef.h>

#include "best_fit.h"

MemoryBlock* best_fit_find(HeapManager* heap, size_t size) {
    MemoryBlock* current = NULL;
    MemoryBlock* best = NULL;
    size_t best_size = 0;

    if (heap == NULL || size == 0) {
        return NULL;
    }

    current = heap->head;
    while (current != NULL) {
        if (current->is_free && current->size >= size) {
            if (best == NULL || current->size < best_size) {
                best = current;
                best_size = current->size;
            }
        }
        current = current->next;
    }

    return best;
}
