#ifndef CLOX_OBJECT_H
#define CLOX_OBJECT_H

#include "common.h"
#include "value.h"

// Lox object type.
typedef enum {
    OBJ_STRING,
} ObjType;

// Lox object.
struct Obj {
    ObjType     type;
    struct Obj* next;
};

struct ObjString {
    Obj   obj;
    int   length;
    char* chars;
};

// Construct an ObjString, taking ownership of the passed C string.
ObjString* takeString(char* chars, int length);
// Construct an ObjString from a C string.
ObjString* copyString(const char* chars, int length);
// Print the contents of an Object-type Value.
void printObject(Value value);
// Check if a value is an object of a given type.
static inline bool isObjType(Value value, ObjType type) {
    return IS_OBJ(value) && AS_OBJ(value)->type == type;
}

// type checking
#define OBJ_TYPE(value) (AS_OBJ(value)->type)
// existential tests
#define IS_STRING(value) isObjType(value, OBJ_STRING)
// casts
#define AS_STRING(value) ((ObjString*)AS_OBJ(value))
#define AS_CSTRING(value) (((ObjString*)AS_OBJ(value))->chars)

#endif
