#ifndef CLOX_MEMORY_H
#define CLOX_MEMORY_H

#include "common.h"

void* reallocate(void* pointer, size_t oldSize, size_t newSize);

// Grow the 'capacity' value by a factor of 2, or to 8 if 'capacity' is less than 8.
#define GROW_CAPACITY(capacity) ((capacity) < 8 ? 8 : (capacity) * 2)

// Grow the array at 'pointer' from 'old_count' to 'new_count' elements of type 'type'.
#define GROW_ARRAY(type, pointer, old_count, new_count)                                  \
    (type*)reallocate(pointer, sizeof(type) * (old_count), sizeof(type) * (new_count))

// Free the array at 'pointer'.
#define FREE_ARRAY(type, pointer, old_count)                                             \
    reallocate(pointer, sizeof(type) * (old_count), 0)

#endif
