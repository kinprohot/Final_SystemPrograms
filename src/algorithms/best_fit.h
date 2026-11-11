#ifndef BEST_FIT_H
#define BEST_FIT_H

#include <sys/types.h>
#include <stddef.h>

#include "../core/heap_manager.h"

/* Trả về block FREE phù hợp theo Best Fit, hoặc NULL nếu không có. */
MemoryBlock* best_fit_find(HeapManager* heap, size_t size);

#endif /* BEST_FIT_H */
