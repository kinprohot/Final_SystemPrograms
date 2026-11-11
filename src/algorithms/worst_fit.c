#include <sys/types.h>
#include <stddef.h>

#include "worst_fit.h"

MemoryBlock* worst_fit_find(HeapManager* heap, size_t size) {
    MemoryBlock* current = NULL;
    MemoryBlock* worst = NULL;
    size_t worst_size = 0;

    if (heap == NULL || size == 0) {
        return NULL;
    }

    current = heap->head;
    while (current != NULL) {
        if (current->is_free && current->size >= size) {
            if (worst == NULL || current->size > worst_size) {
                worst = current;
                worst_size = current->size;
            }
        }
        current = current->next;
    }

    return worst;
}
