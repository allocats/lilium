#pragma once

#include "../token/types.h"
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

typedef struct {
    u32 start;
    u32 end;
} Span;

/*
*
*   Not supported yet: macro, match
*
*/
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

/*
*
*  Base Types for AstType
*
*/
#define AST_BASE_TYPES(X)   \
    X(BASE_I8)              \
    X(BASE_I16)             \
    X(BASE_I32)             \
    X(BASE_I64)             \
                            \
    X(BASE_U8)              \
    X(BASE_U16)             \
    X(BASE_U32)             \
    X(BASE_U64)             \
                            \
    X(BASE_F32)             \
    X(BASE_F64)             \
                            \
    X(BASE_ISIZE)           \
    X(BASE_USIZE)           \
                            \
    X(BASE_BOOL)            \
    X(BASE_STR)             \
                            \
    X(BASE_NAMED)           \

typedef enum [[gnu::packed]] {
    AST_BASE_TYPES(GENERATE_ENUM)
} BaseType;

static const char* AST_BASE_TYPES_STRINGS[] = {
    AST_BASE_TYPES(GENERATE_STRING)
};

#undef AST_BASE_TYPES

/*
*
*  Type suffix for AstType
*
*/
#define TYPE_SUFFIX_KINDS(X)\
    X(SUFFIX_POINTER)       \
    X(SUFFIX_SLICE)         \
    X(SUFFIX_ARRAY)         \

typedef enum {
    TYPE_SUFFIX_KINDS(GENERATE_ENUM)
} SuffixKind;

static const char* TYPE_SUFFIX_KIND_STRINGS[] = {
    TYPE_SUFFIX_KINDS(GENERATE_STRING)
};

#undef TYPE_SUFFIX_KINDS

typedef struct [[gnu::packed]] {
    SuffixKind kind;
    u16        size;
} TypeSuffix;

typedef struct {
    BaseType kind;
    bool is_const;

    u16   len;
    char* ptr;

    TypeSuffix suffix[8];
} AstType;

typedef struct {
    AstNode** stmts;
    u32 stmt_count;
    u32 stmt_capacity;
} AstBlock;

typedef struct {
    char* ptr; 
    usize len;
} AstImport;

// Namespace declaration for the file, has segments
// only one allowed per file, example:
//
// module std.io;
typedef struct {
    char** ptrs; 
    usize* lens;
    u32 count;
    u32 capacity;
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

    AstNode* value;
} AstVariant;

typedef struct {
    char* ptr;
    usize len;

    // For now only support integer types
    // i32, u8, i64
    TokenKind underlying_type;

    AstVariant* variants;
    u32 variant_count;
    u32 variant_capacity;
} AstEnumDecl;

typedef struct {
    char* ptr;
    usize len;

    AstType type;

    AstNode* default_value;
} AstField;

typedef struct {
    char* ptr;
    usize len;

    AstField* fields;
    u32 field_count;
    u32 field_capacity;
} AstStructDecl;

typedef struct {
    char* ptr;
    usize len;

    AstField* fields;
    u32 field_count;
    u32 field_capacity;
} AstUnionDecl;

typedef struct {
    char* ptr;
    usize len;

    AstType type;
} AstParameter;

typedef struct {
    char* ptr;
    u32 len;

    u16 param_count;
    u16 param_capacity;
    AstParameter* params;

    AstBlock block;

    AstType return_type;
} AstFunctionDecl;

typedef struct {
    AstNode* condition;
    AstBlock block;
} AstIfBranch;

typedef struct {
    AstIfBranch* branches;
    u32 branch_count;
    u32 branch_capacity;

    // null if no else statement
    // points to node of AST_BLOCK kind
    AstNode* else_block;
} AstIf;

typedef struct {
    AstBlock block;
} AstInfLoop;

typedef struct {
    AstNode* condition;

    AstBlock block;
} AstWhileLoop;

typedef struct {
    // i: i32 = 0
    AstNode* init;

    // i <= 10
    AstNode* cond;

    // i += 1
    AstNode* step;

    AstBlock block;
} AstForLoop;

typedef struct {
    char* ptr;
    usize len;

    // Optional
    AstNode* value;

    AstType type;
} AstLet;

typedef struct {
    char* ptr;
    usize len;

    // NOT Optional
    AstNode* value;

    AstType type;
} AstConst;

typedef struct {
    AstNode* expression;
} AstDefer;

typedef struct {
    AstNode* expression;
} AstReturn;

typedef struct {
    char* ptr;
    usize len;
} AstIdent;

typedef struct {
    AstNode* ident;

    AstNode** args;
    u32 arg_count;
    u32 arg_capacity;
} AstCall;

typedef struct {
    AstNode* ident;
    AstNode* value;

    TokenKind op;
} AstAssign;

typedef struct {
    AstNode* left;
    AstNode* right;

    TokenKind op;
} AstBinary;

typedef struct {
    AstNode* operand;

    TokenKind op;
} AstUnary;

typedef struct {
    AstNode* ident;
    AstNode* index;
} AstIndex;

typedef struct {
    AstNode* ident;
    AstIdent field;
    bool pointer_access;
} AstMemberAccess;

typedef struct AstNode {
    AstKind kind;
    u8 _padding;
    u16 flags;

    u32 file_index;
    u32 module_index;

    Span span;

    union {
        AstModule module_decl;
        AstImport import_decl;

        AstBuiltinCall builtin_call;

        AstEnumDecl     enum_decl;
        AstStructDecl   struct_decl;
        AstUnionDecl    union_decl;
        AstFunctionDecl function_decl;

        AstIf if_stmt;

        AstInfLoop   inf_loop;
        AstForLoop   for_loop;
        AstWhileLoop while_loop;

        AstLet   let_stmt;
        AstConst const_stmt;

        AstDefer  defer_stmt;
        AstReturn return_stmt;

        AstCall         call;
        AstIdent        ident;
        AstAssign       assignment;
        AstBinary       binary_op;
        AstUnary        unary_op;
        AstIndex        index;
        AstMemberAccess member_access;
    };
} AstNode;

typedef struct {
    ArenaAllocator* arena;
    AstNode* nodes;
    u32 count;
    u32 capacity;
} Ast;
