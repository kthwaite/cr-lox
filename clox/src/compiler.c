#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "chunk.h"
#include "common.h"
#include "compiler.h"
#include "object.h"
#include "scanner.h"

#ifdef DEBUG_PRINT_CODE
#include "debug.h"
#endif

typedef struct {
    Token current;
    Token previous;
    bool  hadError;
    bool  panicMode;
} Parser;
typedef enum {
    PREC_NONE,
    PREC_ASSIGNMENT, // =
    PREC_OR,         // or
    PREC_AND,        // and
    PREC_EQUALITY,   // == !=
    PREC_COMPARISON, // < > <= >=
    PREC_TERM,       // + -
    PREC_FACTOR,     // * /
    PREC_UNARY,      // ! -
    PREC_CALL,       // . ()
    PREC_PRIMARY
} Precedence;
Parser parser;
typedef void (*ParseFn)();
typedef struct {
    ParseFn    prefix;
    ParseFn    infix;
    Precedence precedence;
} ParseRule;
Chunk* compilingChunk;

static void       expression();
static ParseRule* getRule(TokenType type);
static void       parsePrecedence(Precedence prec);

static Chunk* currentChunk() {
    return compilingChunk;
}

static void expression() {
    parsePrecedence(PREC_ASSIGNMENT);
}

// Emit error message at the current token.
static void errorAt(Token* token, const char* msg) {
    // Skip errors in panic mode.
    if (parser.panicMode) {
        return;
    }
    parser.panicMode = true;
    fprintf(stderr, "[line %d] error", token->line);
    switch (token->type) {
        case TOKEN_EOF: fprintf(stderr, " at eof"); break;
        case TOKEN_ERROR: break;
        default: fprintf(stderr, " at '%.*s'", token->length, token->start); break;
    }
    fprintf(stderr, ": %s\n", msg);
    parser.hadError = true;
}

static void error(const char* msg) {
    errorAt(&parser.previous, msg);
}

static void errorAtCurrent(const char* msg) {
    errorAt(&parser.current, msg);
}

static void advance() {
    parser.previous = parser.current;
    for (;;) {
        parser.current = scanToken();
        if (parser.current.type != TOKEN_ERROR) {
            break;
        }
        errorAtCurrent(parser.current.start);
    }
}

// Consume a token of `type`, erroring if another token is encountered.
static void consume(TokenType type, const char* msg) {
    if (parser.current.type == type) {
        advance();
        return;
    }
    errorAtCurrent(msg);
}

// Append a bytecode op to the current chunk.
static void emitByte(uint8_t byte) {
    writeChunk(currentChunk(), byte, parser.previous.line);
}

// Append two bytecode ops to the current chunk.
static void emitBytes(uint8_t byte0, uint8_t byte1) {
    emitByte(byte0);
    emitByte(byte1);
}

// Append an OP_RETURN to the current chunk.
static void emitReturn() {
    emitByte(OP_RETURN);
}

//
static void endCompiler() {
    emitReturn();
#ifdef DEBUG_PRINT_CODE
    if (!parser.hadError) {
        disassembleChunk(currentChunk(), "code");
    }
#endif
}

//
static void binary() {
    TokenType  op_type = parser.previous.type;
    ParseRule* rule    = getRule(op_type);
    parsePrecedence((Precedence)(rule->precedence + 1));

    switch (op_type) {
        // TODO: add combined instructions
        case TOKEN_BANG_EQUAL: emitBytes(OP_EQUAL, OP_NOT); break;
        case TOKEN_EQUAL_EQUAL: emitByte(OP_EQUAL); break;
        case TOKEN_GREATER: emitByte(OP_GREATER); break;
        case TOKEN_GREATER_EQUAL: emitBytes(OP_LESS, OP_NOT); break;
        case TOKEN_LESS: emitByte(OP_LESS); break;
        case TOKEN_LESS_EQUAL: emitBytes(OP_GREATER, OP_NOT); break;

        case TOKEN_PLUS: emitByte(OP_ADD); break;
        case TOKEN_MINUS: emitByte(OP_SUBTRACT); break;
        case TOKEN_STAR: emitByte(OP_MULTIPLY); break;
        case TOKEN_SLASH: emitByte(OP_DIVIDE); break;
        default: return; // unreachable
    }
}

//
static void literal() {
    switch (parser.previous.type) {
        case TOKEN_FALSE: emitByte(OP_FALSE); break;
        case TOKEN_NIL: emitByte(OP_NIL); break;
        case TOKEN_TRUE: emitByte(OP_TRUE); break;
        default: return; // unreachable
    }
}

//
static void grouping() {
    expression();
    consume(TOKEN_RIGHT_PAREN, "Expect ')' after expression");
}

// Add a constant to the current chunk and return its index.
static uint8_t makeConstant(Value value) {
    int constant = addConstant(currentChunk(), value);
    if (constant > UINT8_MAX) {
        error("Too many constants in one chunk");
        return 0;
    }
    return (uint8_t)constant;
}

// Append an OP_CONSTANT to the current chunk along with its value.
static void emitConstant(Value value) {
    emitBytes(OP_CONSTANT, makeConstant(value));
}

// Parse a number and emit it as a constant.
static void number() {
    double value = strtod(parser.previous.start, NULL);

    emitConstant(NUMBER_VAL(value));
}

//
static void string() {
    emitConstant(
        OBJ_VAL(copyString(parser.previous.start + 1, parser.previous.length - 2)));
}

// Parse unary operators.
static void unary() {
    TokenType op_type = parser.previous.type;
    expression();
    switch (op_type) {
        case TOKEN_BANG: emitByte(OP_NOT); break;
        case TOKEN_MINUS: emitByte(OP_NEGATE); break;
        default: break;
    }
}

//
static void parsePrecedence(Precedence prec) {
    advance();
    ParseFn prefix_rule = getRule(parser.previous.type)->prefix;
    if (prefix_rule == NULL) {
        error("Expected expression");
        return;
    }
    prefix_rule();
    while (prec <= getRule(parser.current.type)->precedence) {
        advance();
        ParseFn infix_rule = getRule(parser.previous.type)->infix;
        infix_rule();
    }
}

ParseRule rules[] = {
    [TOKEN_LEFT_PAREN]    = {grouping, NULL,   PREC_NONE      },
    [TOKEN_RIGHT_PAREN]   = {NULL,     NULL,   PREC_NONE      },
    [TOKEN_LEFT_BRACE]    = {NULL,     NULL,   PREC_NONE      },
    [TOKEN_RIGHT_BRACE]   = {NULL,     NULL,   PREC_NONE      },
    [TOKEN_COMMA]         = {NULL,     NULL,   PREC_NONE      },
    [TOKEN_DOT]           = {NULL,     NULL,   PREC_NONE      },
    [TOKEN_MINUS]         = {unary,    binary, PREC_TERM      },
    [TOKEN_PLUS]          = {NULL,     binary, PREC_TERM      },
    [TOKEN_SEMICOLON]     = {NULL,     NULL,   PREC_NONE      },
    [TOKEN_SLASH]         = {NULL,     binary, PREC_FACTOR    },
    [TOKEN_STAR]          = {NULL,     binary, PREC_FACTOR    },
    [TOKEN_BANG]          = {unary,    NULL,   PREC_NONE      },
    [TOKEN_BANG_EQUAL]    = {NULL,     binary, PREC_EQUALITY  },
    [TOKEN_EQUAL]         = {NULL,     NULL,   PREC_NONE      },
    [TOKEN_EQUAL_EQUAL]   = {NULL,     binary, PREC_EQUALITY  },
    [TOKEN_GREATER]       = {NULL,     binary, PREC_COMPARISON},
    [TOKEN_GREATER_EQUAL] = {NULL,     binary, PREC_COMPARISON},
    [TOKEN_LESS]          = {NULL,     binary, PREC_COMPARISON},
    [TOKEN_LESS_EQUAL]    = {NULL,     binary, PREC_COMPARISON},
    [TOKEN_IDENTIFIER]    = {NULL,     NULL,   PREC_NONE      },
    [TOKEN_STRING]        = {string,   NULL,   PREC_NONE      },
    [TOKEN_NUMBER]        = {number,   NULL,   PREC_NONE      },
    [TOKEN_AND]           = {NULL,     NULL,   PREC_NONE      },
    [TOKEN_CLASS]         = {NULL,     NULL,   PREC_NONE      },
    [TOKEN_ELSE]          = {NULL,     NULL,   PREC_NONE      },
    [TOKEN_FALSE]         = {literal,  NULL,   PREC_NONE      },
    [TOKEN_FOR]           = {NULL,     NULL,   PREC_NONE      },
    [TOKEN_FUN]           = {NULL,     NULL,   PREC_NONE      },
    [TOKEN_IF]            = {NULL,     NULL,   PREC_NONE      },
    [TOKEN_NIL]           = {literal,  NULL,   PREC_NONE      },
    [TOKEN_OR]            = {NULL,     NULL,   PREC_NONE      },
    [TOKEN_PRINT]         = {NULL,     NULL,   PREC_NONE      },
    [TOKEN_RETURN]        = {NULL,     NULL,   PREC_NONE      },
    [TOKEN_SUPER]         = {NULL,     NULL,   PREC_NONE      },
    [TOKEN_THIS]          = {NULL,     NULL,   PREC_NONE      },
    [TOKEN_TRUE]          = {literal,  NULL,   PREC_NONE      },
    [TOKEN_VAR]           = {NULL,     NULL,   PREC_NONE      },
    [TOKEN_WHILE]         = {NULL,     NULL,   PREC_NONE      },
    [TOKEN_ERROR]         = {NULL,     NULL,   PREC_NONE      },
    [TOKEN_EOF]           = {NULL,     NULL,   PREC_NONE      },
};

// Get the rule for a given token type.
static ParseRule* getRule(TokenType type) {
    return &rules[type];
}

bool compile(const char* source, Chunk* chunk) {
    initScanner(source);
    compilingChunk = chunk;
    advance();
    expression();
    consume(TOKEN_EOF, "Expected end of expression");
    endCompiler();
    return !parser.hadError;
}
