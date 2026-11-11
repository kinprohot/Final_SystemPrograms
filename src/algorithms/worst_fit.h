#ifndef WORST_FIT_H
#define WORST_FIT_H

#include <sys/types.h>
#include <stddef.h>

#include "../core/heap_manager.h"

/* Trả về block FREE phù hợp theo Worst Fit, hoặc NULL nếu không có. */
MemoryBlock* worst_fit_find(HeapManager* heap, size_t size);

#endif /* WORST_FIT_H */
