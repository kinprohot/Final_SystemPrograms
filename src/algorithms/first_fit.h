#ifndef FIRST_FIT_H
#define FIRST_FIT_H

#include <sys/types.h>
#include <stddef.h>

#include "../core/heap_manager.h"

/* Trả về block FREE phù hợp theo First Fit, hoặc NULL nếu không có. */
MemoryBlock* first_fit_find(HeapManager* heap, size_t size);

#endif /* FIRST_FIT_H */
