#ifndef CLOX_CHUNK_H
#define CLOX_CHUNK_H

#include "common.h"
#include "value.h"

// A single instruction.
typedef enum {
    OP_CONSTANT,
    // TODO: OP_CONSTANT_LONG
    OP_NIL,
    OP_TRUE,
    OP_FALSE,

    OP_EQUAL,
    OP_GREATER,
    OP_LESS,

    OP_NOT,
    OP_NEGATE,

    OP_ADD,
    OP_SUBTRACT,
    OP_MULTIPLY,
    OP_DIVIDE,

    OP_RETURN,
} OpCode;

typedef struct {
    int count;
    int capacity;
    uint8_t* code;
    // TODO: could be uint8_t RLE with an `int max_lineno`
    int* lines;
    ValueArray constants;
} Chunk;

// Initialize a chunk.
void initChunk(Chunk* chunk);
// Free a chunk.
void freeChunk(Chunk* chunk);
// Write a byte into a chunk, reallocating if necessary.
void writeChunk(Chunk* chunk, uint8_t byte, int line);
// Add a constant to the constants array, returning the index.
int addConstant(Chunk* chunk, Value value);

#endif
