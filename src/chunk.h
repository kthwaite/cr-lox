#ifndef clox_chunk_h
#define clox_chunk_h

#include "common.h"
#include "value.h"

// An instruction.
typedef enum {
    // Push a constant onto the stack.
    OP_CONSTANT,
    // Unary negation.
    OP_NEGATE,
    OP_NOT,
    // Binary operators.
    OP_ADD,
    OP_SUBTRACT,
    OP_MULTIPLY,
    OP_DIVIDE,
    //
    OP_TRUE,
    OP_FALSE,
    OP_NIL,
    OP_EQUAL,
    OP_GREATER,
    OP_LESS,
    //
    OP_RETURN,
} OpCode;

// A chunk of bytecode.
typedef struct {
    // The number of instructions.
    int count;
    // The number of bytes allocated for instructions.
    int capacity;
    // Array of constants.
    ValueArray constants;
    // Array of instructions.
    uint8_t* code;
    // Array of line numbers.
    int* lines;
} Chunk;

// Initialize a chunk.
void initChunk(Chunk* chunk);
// Write an instruction to a chunk.
void writeChunk(Chunk* chunk, uint8_t byte, int line);
// Add a constant to a chunk.
int addConstant(Chunk* chunk, Value value);
// Free a chunk.
void freeChunk(Chunk* chunk);

#endif
