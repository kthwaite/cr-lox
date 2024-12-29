#include "debug.h"
#include "chunk.h"
#include <stdio.h>

#define HANDLE_SIMPLE_INSTRUCTION(op, offset)                                  \
    case op: return simpleInstruction(#op, offset)

void disassembleChunk(Chunk* chunk, const char* name) {
    printf("== %s ==\n", name);
    for (int offset = 0; offset < chunk->count;) {
        offset = disassembleInstruction(chunk, offset);
    }
}

static int simpleInstruction(const char* name, int offset) {
    printf("%s\n", name);
    return offset + 1;
}

static int constantInstruction(const char* name, Chunk* chunk, int offset) {
    uint8_t constant = chunk->code[offset + 1];
    printf("%-16s %4d", name, constant);
    printValue(chunk->constants.values[constant]);
    printf("\n");
    return offset + 2;
}

int disassembleInstruction(Chunk* chunk, int offset) {
    printf("%04d ", offset);
    if (offset > 0 && chunk->lines[offset] == chunk->lines[offset - 1]) {
        printf(" | ");
    } else {
        printf("%4d ", chunk->lines[offset]);
    }
    uint8_t instruction = chunk->code[offset];
    switch (instruction) {
        HANDLE_SIMPLE_INSTRUCTION(OP_RETURN, offset);
        HANDLE_SIMPLE_INSTRUCTION(OP_NEGATE, offset);
        HANDLE_SIMPLE_INSTRUCTION(OP_ADD, offset);
        HANDLE_SIMPLE_INSTRUCTION(OP_SUBTRACT, offset);
        HANDLE_SIMPLE_INSTRUCTION(OP_MULTIPLY, offset);
        HANDLE_SIMPLE_INSTRUCTION(OP_DIVIDE, offset);
        HANDLE_SIMPLE_INSTRUCTION(OP_TRUE, offset);
        HANDLE_SIMPLE_INSTRUCTION(OP_FALSE, offset);
        HANDLE_SIMPLE_INSTRUCTION(OP_NIL, offset);
        HANDLE_SIMPLE_INSTRUCTION(OP_EQUAL, offset);
        HANDLE_SIMPLE_INSTRUCTION(OP_GREATER, offset);
        HANDLE_SIMPLE_INSTRUCTION(OP_LESS, offset);
        case OP_CONSTANT:
            return constantInstruction("OP_CONSTANT", chunk, offset);
        default:
            printf("Unknown instruction %d\n", instruction);
            return offset + 1;
    }
}
