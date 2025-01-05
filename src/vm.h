#ifndef clox_vm_h
#define clox_vm_h

#include "chunk.h"
#include "table.h"
#include "value.h"

#define STACK_MAX 256

typedef struct {
    Chunk* chunk;
    uint8_t* ip;
    Value stack[STACK_MAX];
    Value* stackTop;
    Table strings;
    Obj* objects;
} VM;

typedef enum {
    INTERPRET_OK,
    INTERPRET_COMPILE_ERROR,
    INTERPRET_RUNTIME_ERROR,
} InterpretResult;

extern VM vm;

// Initialize the VM.
void initVM();
// Interpret a chunk.
InterpretResult interpret(const char* source);
// Push a value onto the stack.
void push(Value value);
// Pop a value from the stack.
Value pop();
// Deinitialize the VM and free associated memory.
void freeVM();

#endif
