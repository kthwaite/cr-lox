#ifndef CLOX_MEMORY_H
#define CLOX_MEMORY_H

#include "common.h"
#include "object.h"

// Reallocate or free the memory at `pointer`.
// @param pointer The pointer to reallocate.
// @param oldSize The old size of the memory at `pointer`.
// @param newSize The new size of the memory at `pointer`. If zero, `pointer` will be
//  freed instead.
// @return The new pointer to the reallocated memory, or NULL if `newSize` was 0.
void* reallocate(void* pointer, size_t oldSize, size_t newSize);
void  freeObjects();

//
#define FREE(type, pointer) reallocate(pointer, sizeof(type), 0)

// Grow the 'capacity' value by a factor of 2, or to 8 if 'capacity' is less than 8.
#define GROW_CAPACITY(capacity) ((capacity) < 8 ? 8 : (capacity) * 2)

// Grow the array at 'pointer' from 'old_count' to 'new_count' elements of type 'type'.
#define GROW_ARRAY(type, pointer, old_count, new_count)                                  \
    (type*)reallocate(pointer, sizeof(type) * (old_count), sizeof(type) * (new_count))

// Free the array at 'pointer'.
#define FREE_ARRAY(type, pointer, old_count)                                             \
    reallocate(pointer, sizeof(type) * (old_count), 0)

#define ALLOCATE(type, count) (type*)reallocate(NULL, 0, sizeof(type) * (count))

#endif
