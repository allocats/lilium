#pragma once

#include "../utils/types.h"

#define X(TOK)          \
    TOK(TOK_UNKNOWN)    \
    TOK(TOK_ERROR)      \
    TOK(TOK_EOF)        \
                        \
    TOK(TOK_IDENT)      \
    TOK(TOK_FLOAT_LIT)  \
    TOK(TOK_INTEGER_LIT)\
    TOK(TOK_STRING_LIT) \
    TOK(TOK_CHAR_LIT)   \
                        \
    TOK(TOK_FALSE)      \
    TOK(TOK_TRUE)       \
    TOK(TOK_NULL)       \
                        \
    TOK(TOK_IF)         \
    TOK(TOK_ELSE)       \
    TOK(TOK_MATCH)      \
                        \
    TOK(TOK_WHILE)      \
    TOK(TOK_FOR)        \
    TOK(TOK_LOOP)       \
                        \
    TOK(TOK_IMPORT)     \
    TOK(TOK_MODULE)     \
                        \
    TOK(TOK_ALIAS)      \
                        \
    TOK(TOK_FN)         \
    TOK(TOK_MACRO)      \
                        \
    TOK(TOK_ENUM)       \
    TOK(TOK_STRUCT)     \
    TOK(TOK_UNION)      \
                        \
    TOK(TOK_INLINE)     \
    TOK(TOK_EXTERNAL)   \
    TOK(TOK_STATIC)     \
                        \
    TOK(TOK_LET)        \
    TOK(TOK_CONST)      \
    TOK(TOK_DEFER)      \
    TOK(TOK_RETURN)     \
    TOK(TOK_BREAK)      \
    TOK(TOK_CONTINUE)   \
                        \
    TOK(TOK_HASH)       \
    TOK(TOK_AT)         \
                        \
    TOK(TOK_DOT)        \
    TOK(TOK_DOT_DOT)    \
    TOK(TOK_DOT_DOT_DOT)\
    TOK(TOK_COMMA)      \
    TOK(TOK_COLON)      \
    TOK(TOK_SEMI)       \
    TOK(TOK_ARROW)      \
                        \
    TOK(TOK_LBRACE)     \
    TOK(TOK_RBRACE)     \
    TOK(TOK_LPAREN)     \
    TOK(TOK_RPAREN)     \
    TOK(TOK_LBRACKET)   \
    TOK(TOK_RBRACKET)   \
                        \
    TOK(TOK_EQ)         \
    TOK(TOK_EQ_EQ)      \
    TOK(TOK_BANG)       \
    TOK(TOK_BANG_EQ)    \
                        \
    TOK(TOK_MINUS)      \
    TOK(TOK_MINUS_EQ)   \
    TOK(TOK_PLUS)       \
    TOK(TOK_PLUS_EQ)    \
    TOK(TOK_STAR)       \
    TOK(TOK_STAR_EQ)    \
    TOK(TOK_SLASH)      \
    TOK(TOK_SLASH_EQ)   \
    TOK(TOK_PERCENT)    \
    TOK(TOK_PERCENT_EQ) \
                        \
    TOK(TOK_AMP)        \
    TOK(TOK_AMP_AMP)    \
    TOK(TOK_AMP_EQ)     \
                        \
    TOK(TOK_PIPE)       \
    TOK(TOK_PIPE_PIPE)  \
    TOK(TOK_PIPE_EQ)    \
                        \
    TOK(TOK_TILDE)      \
    TOK(TOK_TILDE_EQ)   \
                        \
    TOK(TOK_CARET)      \
    TOK(TOK_CARET_EQ)   \
                        \
    TOK(TOK_LT)         \
    TOK(TOK_LT_EQ)      \
    TOK(TOK_SHL)        \
    TOK(TOK_SHL_EQ)     \
                        \
    TOK(TOK_GT)         \
    TOK(TOK_GT_EQ)      \
    TOK(TOK_SHR)        \
    TOK(TOK_SHR_EQ)     \

typedef enum {
    X(GENERATE_ENUM)
} TokenKind;

static const char* TOKEN_KIND_STRINGS[] = {
    X(GENERATE_STRING)
};

#undef X

typedef struct {
    char* lexeme;
    u32 length;
    TokenKind kind;
} Token;

typedef struct {
    Token* items;
    usize count;
    usize capacity;
} Tokens;
