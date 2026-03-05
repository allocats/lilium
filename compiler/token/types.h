#pragma once

#include "../utils/types.h"

#define X(TOK)      \
    TOK(TOK_IDENT)  \

typedef enum {
    X(GENERATE_ENUM)
} TokenKind;

static const char* TOKEN_KIND_STRINGS[] = {
    X(GENERATE_STRING)
};

#undef X

typedef struct {
    char* lexeme;
    u32 legnth;
    TokenKind kind;
} Token;

typedef struct {
    Token* tokens;
    usize count;
    usize capacity;
} Tokens;
