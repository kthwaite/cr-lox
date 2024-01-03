#ifndef CLOX_TABLE_H
#define CLOX_TABLE_H

#include "common.h"
#include "value.h"

// Maximum load factor for the hashtable.
#define TABLE_MAX_LOAD 0.75

// Hashtable entry.
typedef struct {
    ObjString* key;
    Value      value;

} Entry;

// Hashtable.
typedef struct {
    int    count;
    int    capacity;
    Entry* entries;
} Table;

// Initialize a table.
void initTable(Table* table);
// Free a table.
void freeTable(Table* table);
// Set a value in a table.
bool tableSet(Table* table, ObjString* key, Value value);
// Get a value from a table.
bool tableGet(Table* table, ObjString* key, Value* value);
// Delete a value from a table.
bool tableDelete(Table* table, ObjString* key);
// Copy values from one table to another.
void tableAddAll(Table* from, Table* to);
//
ObjString* tableFindString(Table* table, const char* chars, int length, uint32_t hash);

#endif
