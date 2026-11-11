#ifndef NEXT_FIT_H
#define NEXT_FIT_H

#include <sys/types.h>
#include <stddef.h>

#include "../core/heap_manager.h"

/* Trả về block FREE phù hợp theo Next Fit, hoặc NULL nếu không có. */
MemoryBlock* next_fit_find(HeapManager* heap, size_t size);

#endif /* NEXT_FIT_H */
