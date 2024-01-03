#include <stdio.h>
#include <string.h>

#include "memory.h"
#include "object.h"
#include "value.h"
#include "vm.h"

#define ALLOCATE_OBJ(type, objectType) (type*)allocateObject(sizeof(type), objectType)

// Initialize a new object on the heap.
// @param size: The size of the object to allocate.
// @param type: The type of the object to allocate.
// @return: A pointer to the newly allocated object.
static Obj* allocateObject(size_t size, ObjType type) {
    Obj* object  = (Obj*)reallocate(NULL, 0, size);
    object->type = type;
    return object;
}

// Initialize an ObjString with the passed char array.
// @param chars: The char array to construct the string from.
// @param length: The length of the char array.
// @return: A pointer to the newly allocated ObjString.
static ObjString* allocateString(char* chars, int length) {
    ObjString* string = ALLOCATE_OBJ(ObjString, OBJ_STRING);
    string->length    = length;
    string->chars     = chars;
    return string;
}

ObjString* copyString(const char* chars, int length) {
    char* heap_chars = ALLOCATE(char, length + 1);
    memcpy(heap_chars, chars, length);
    heap_chars[length] = '\0';
    return allocateString(heap_chars, length);
}

ObjString* takeString(char* chars, int length) {
    return allocateString(chars, length);
}

void printObject(Value value) {
    switch (OBJ_TYPE(value)) {
        case OBJ_STRING: printf("%s", AS_CSTRING(value)); break;
    }
}
