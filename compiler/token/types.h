#pragma once

#include "../utils/types.h"

#define TOKENS(X)       \
    X(TOK_UNKNOWN)      \
    X(TOK_ERROR)        \
    X(TOK_EOF)          \
                        \
    X(TOK_I8)           \
    X(TOK_I16)          \
    X(TOK_I32)          \
    X(TOK_I64)          \
                        \
    X(TOK_U8)           \
    X(TOK_U16)          \
    X(TOK_U32)          \
    X(TOK_U64)          \
                        \
    X(TOK_F32)          \
    X(TOK_F64)          \
                        \
    X(TOK_CHAR)         \
    X(TOK_STR)          \
                        \
    X(TOK_BOOL)         \
    X(TOK_ISIZE)        \
    X(TOK_USIZE)        \
                        \
    X(TOK_IDENT)        \
    X(TOK_FLOAT_LIT)    \
    X(TOK_INTEGER_LIT)  \
    X(TOK_STRING_LIT)   \
    X(TOK_CHAR_LIT)     \
                        \
    X(TOK_FALSE)        \
    X(TOK_TRUE)         \
    X(TOK_NULL)         \
                        \
    X(TOK_IF)           \
    X(TOK_ELSE)         \
    X(TOK_MATCH)        \
                        \
    X(TOK_WHILE)        \
    X(TOK_FOR)          \
    X(TOK_LOOP)         \
                        \
    X(TOK_IMPORT)       \
    X(TOK_MODULE)       \
                        \
    X(TOK_ALIAS)        \
                        \
    X(TOK_FN)           \
    X(TOK_MACRO)        \
                        \
    X(TOK_ENUM)         \
    X(TOK_STRUCT)       \
    X(TOK_UNION)        \
                        \
    X(TOK_INLINE)       \
    X(TOK_EXTERNAL)     \
    X(TOK_STATIC)       \
                        \
    X(TOK_LET)          \
    X(TOK_CONST)        \
    X(TOK_DEFER)        \
    X(TOK_RETURN)       \
    X(TOK_BREAK)        \
    X(TOK_CONTINUE)     \
                        \
    X(TOK_HASH)         \
    X(TOK_AT)           \
                        \
    X(TOK_DOT)          \
    X(TOK_DOT_DOT)      \
    X(TOK_DOT_DOT_DOT)  \
    X(TOK_COMMA)        \
    X(TOK_COLON)        \
    X(TOK_SEMI)         \
    X(TOK_ARROW)        \
                        \
    X(TOK_LBRACE)       \
    X(TOK_RBRACE)       \
    X(TOK_LPAREN)       \
    X(TOK_RPAREN)       \
    X(TOK_LBRACKET)     \
    X(TOK_RBRACKET)     \
                        \
    X(TOK_EQ)           \
    X(TOK_EQ_EQ)        \
    X(TOK_BANG)         \
    X(TOK_BANG_EQ)      \
                        \
    X(TOK_MINUS)        \
    X(TOK_MINUS_EQ)     \
    X(TOK_PLUS)         \
    X(TOK_PLUS_EQ)      \
    X(TOK_STAR)         \
    X(TOK_STAR_EQ)      \
    X(TOK_SLASH)        \
    X(TOK_SLASH_EQ)     \
    X(TOK_PERCENT)      \
    X(TOK_PERCENT_EQ)   \
                        \
    X(TOK_AMP)          \
    X(TOK_AMP_AMP)      \
    X(TOK_AMP_EQ)       \
                        \
    X(TOK_PIPE)         \
    X(TOK_PIPE_PIPE)    \
    X(TOK_PIPE_EQ)      \
                        \
    X(TOK_TILDE)        \
    X(TOK_TILDE_EQ)     \
                        \
    X(TOK_CARET)        \
    X(TOK_CARET_EQ)     \
                        \
    X(TOK_LT)           \
    X(TOK_LT_EQ)        \
    X(TOK_SHL)          \
    X(TOK_SHL_EQ)       \
                        \
    X(TOK_GT)           \
    X(TOK_GT_EQ)        \
    X(TOK_SHR)          \
    X(TOK_SHR_EQ)       \

typedef enum {
    TOKENS(GENERATE_ENUM)
} TokenKind;

static const char* TOKEN_KIND_STRINGS[] = {
    TOKENS(GENERATE_STRING)
};

#undef TOKENS 

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
