#ifndef CLOX_VALUE_H
#define CLOX_VALUE_H

#include "common.h"

typedef struct Obj       Obj;
typedef struct ObjString ObjString;

typedef enum {
    VAL_BOOL,
    VAL_NIL,
    VAL_NUMBER,
    VAL_OBJ,
} ValueType;

// Lox value.
typedef struct {
    ValueType type;
    union {
        bool   boolean;
        double number;
        Obj*   obj;
    } as;
} Value;

// existential tests
#define IS_BOOL(value) ((value).type == VAL_BOOL)
#define IS_NIL(value) ((value).type == VAL_NIL)
#define IS_OBJ(value) ((value).type == VAL_OBJ)
#define IS_NUMBER(value) ((value).type == VAL_NUMBER)
// casts
#define AS_BOOL(value) ((value).as.boolean)
#define AS_OBJ(value) ((value).as.obj)
#define AS_NUMBER(value) ((value).as.number)
// constructors
#define BOOL_VAL(value) ((Value){VAL_BOOL, {.boolean = value}})
#define NIL_VAL ((Value){VAL_NIL, {.number = 0}})
#define NUMBER_VAL(value) ((Value){VAL_NUMBER, {.number = value}})
#define OBJ_VAL(value) ((Value){VAL_OBJ, {.obj = value}})

void printValue(Value value);

// Dynamic array of values.
typedef struct {
    int    capacity;
    int    count;
    Value* values;
} ValueArray;

// Initialize a value array.
void initValueArray(ValueArray* array);
// Write a value into a value array, reallocating if necessary.
void writeValueArray(ValueArray* array, Value value);
// Free a value array.
void freeValueArray(ValueArray* array);
// Equality check for two Values.
bool valuesEqual(Value a, Value b);

#endif
