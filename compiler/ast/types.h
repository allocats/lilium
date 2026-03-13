#pragma once

#include "../utils/types.h"

#define IS_SYMBOL_RESOLVED  (1 << 0)
#define IS_TYPE_RESOLVED    (1 << 1)
#define IS_VALID_LVALUE     (1 << 2)
#define IS_CONSTANT         (1 << 3)
#define IS_COMP_TIME        (1 << 4) // TODO: Add this later down the line, just putting here so long :3
#define IS_INLINE           (1 << 5)
#define IS_EXTERNAL         (1 << 6)
#define IS_STATIC           (1 << 7)
#define IS_VARIADAC         (1 << 8)

typedef struct AstNode AstNode;

#define AST_NODES(X)    \
    X(AST_ERROR)        \
                        \
    X(AST_MODULE)       \
    X(AST_IMPORT)       \
                        \
    X(AST_BUILTIN_CALL) \
                        \
    X(AST_ENUM)         \
    X(AST_UNION)        \
    X(AST_STRUCT)       \
    X(AST_FUNCTION)     \
                        \
    X(AST_FIELD)        \
    X(AST_VARIANT)      \
    X(AST_PARAMETER)    \
                        \
    X(AST_CONSTANT)     \
    X(AST_VARIABLE)     \
                        \
    X(AST_CONTINUE)     \
    X(AST_BREAK)        \
                        \
    X(AST_BLOCK)        \
    X(AST_DEFER)        \
    X(AST_RETURN)       \
                        \
    X(AST_IF)           \
    X(AST_ELSE)         \
    X(AST_MATCH)        \
                        \
    X(AST_FOR)          \
    X(AST_LOOP)         \
    X(AST_WHILE)        \
                        \
    X(AST_BINARY)       \
    X(AST_UNARY)        \
    X(AST_ASSIGN)       \
    X(AST_CALL)         \
    X(AST_IDENT)        \
    X(AST_LITERAL)      \
    X(AST_INDEX)        \
    X(AST_MEMBER_ACCESS)\
    X(AST_STRUCT_INIT)  \


typedef enum [[gnu::packed]] {
    AST_NODES(GENERATE_ENUM)
} AstKind;

static const char* AST_KIND_STRINGS[] = {
    AST_NODES(GENERATE_STRING)
};

#undef AST_NODES

typedef struct {
    u32 start;
    u32 end;
} Span;

typedef struct {
    char* ptr; 
    usize len;
} AstImport;

// Namespace declaration for the file c:
// only one allowed per file
typedef struct {
    char* ptr; 
    usize len;
} AstModule;

typedef struct {
    // BuiltinKind kind;
    u32 kind;

    AstNode** args;
    u32 arg_count;
    u32 arg_capacity;
} AstBuiltinCall;

typedef struct {
    char* ptr;
    usize len;

    AstNode* variants;
    u32 arg_count;
    u32 arg_capacity;
} AstEnum;

typedef struct AstNode {
    AstKind kind;
    u16 flags;
    u32 file_index;

    Span span;

    union {
        AstModule module_decl;
        AstImport import_decl;

        AstBuiltinCall builtin_call;
    };
} AstNode;
