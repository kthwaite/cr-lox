#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"
#include "vm.h"

static void repl() {
    char line[1024];
    for (;;) {
        printf("> ");
        if (!fgets(line, sizeof(line), stdin)) {
            printf("\n");
            break;
        }
        interpret(line);
    }
}

static char* readFile(const char* path) {
    FILE* file = fopen(path, "rb");
    if (file == NULL) {
        fprintf(stderr, "Could not open file \"%s\".\n", path);
        exit(74);
    }

    fseek(file, 0L, SEEK_END);
    size_t file_len = ftell(file);
    rewind(file);

    char* buffer = (char*)calloc(file_len + 1, sizeof(char));
    if (buffer == NULL) {
        fprintf(stderr, "Not enough memory to read \"%s\".\n", path);
        exit(74);
    }
    size_t bytes_read = fread(buffer, sizeof(char), file_len, file);
    if (bytes_read < file_len) {
        fprintf(stderr, "Could not read file \"%s\".\n", path);
        exit(74);
    }
    buffer[bytes_read] = '\0';

    fclose(file);
    return buffer;
}

static void runFile(const char* path) {
    char* source = readFile(path);
    InterpretResult result = interpret(source);
    free(source);
    switch (result) {
        case INTERPRET_COMPILE_ERROR: exit(65);
        case INTERPRET_RUNTIME_ERROR: exit(70);
        default: break;
    }
}

int main(int argc, const char** argv) {
    initVM();
    switch (argc) {
        case 1: repl(); break;
        case 2: runFile(argv[1]); break;
        default: fprintf(stderr, "Usage: clox: [path]\n"); exit(64);
    }
    freeVM();
    return 0;
}
