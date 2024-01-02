#ifndef CLOX_VM_H
#define CLOX_VM_H

#include "chunk.h"
#include "value.h"

#define STACK_MAX 256

typedef struct {
    Chunk* chunk;
    uint8_t* ip;
    Value stack[STACK_MAX];
    Value* stackTop;
} VM;

typedef enum {
    INTERPRET_OK,
    INTERPRET_COMPILE_ERROR,
    INTERPRET_RUNTIME_ERROR,
} InterpretResult;

// Initialize the VM.
void initVM();
// Interpret a chunk using the VM.
InterpretResult interpret(const char* source);
// Push a value onto the stack.
void push(Value value);
// Pop a value off the stack.
Value pop();
// Free the VM.
void freeVM();

#endif
