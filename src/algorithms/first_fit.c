#include <sys/types.h>
#include <stddef.h>

#include "first_fit.h"

MemoryBlock* first_fit_find(HeapManager* heap, size_t size) {
    MemoryBlock* current = NULL;

    if (heap == NULL || size == 0) {
        return NULL;
    }

    current = heap->head;
    while (current != NULL) {
        if (current->is_free && current->size >= size) {
            return current;
        }
        current = current->next;
    }

    return NULL;
}
