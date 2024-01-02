#ifndef CLOX_VALUE_H
#define CLOX_VALUE_H

#include "common.h"

// Lox numeric value.
typedef double Value;

void printValue(Value value);

// Dynamic array of values.
typedef struct {
    int capacity;
    int count;
    Value* values;
} ValueArray;

// Initialize a value array.
void initValueArray(ValueArray* array);
// Write a value into a value array, reallocating if necessary.
void writeValueArray(ValueArray* array, Value value);
// Free a value array.
void freeValueArray(ValueArray* array);

#endif
