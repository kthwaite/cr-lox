#ifndef CLOX_DEBUG_H
#define CLOX_DEBUG_H

#include "chunk.h"

// Print the instructions in a chunk.
void disassembleChunk(Chunk* chunk, const char* name);
// Handle disassembly of the instruction at `offset`.
int disassembleInstruction(Chunk* chunk, int offset);
// Print the name of a single instruction, advancing the offset by one.
int simpleInstruction(const char* name, int offset);

#endif
