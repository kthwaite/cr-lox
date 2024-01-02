#ifndef CLOX_CHUNK_H
#define CLOX_CHUNK_H

#include "common.h"

// A single instruction.
typedef enum {
    OP_RETURN,
} OpCode;

typedef struct {
    int count;
    int capacity;
    uint8_t* code;
} Chunk;

// Initialize a chunk.
void initChunk(Chunk* chunk);
// Free a chunk.
void freeChunk(Chunk* chunk);
// Write a byte into a chunk, reallocating if necessary.
void writeChunk(Chunk* chunk, uint8_t byte);


#endif
