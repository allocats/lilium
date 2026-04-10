#include "parser.h"

#include "../diagnostics/diagnostics.h"
#include "../utils/macros.h"

#include "../token/token.h"

#ifdef DEBUG_MODE
#include <assert.h>
#define ASSERT(x) assert(x)
#else
#define ASSERT(x) (void)(x)
#endif /* ifdef DEBUG_MODE */

void parse_tokens(Ast* ast, Tokens* tokens) {
    Parser p = {
        .file_index = 0,
        .cursor = 0,
        .count = tokens -> count,
        .tokens = tokens,
        .arena = ast -> arena
    };

    while (p.cursor < p.count) {
        if (UNLIKELY(ast -> count >= ast -> capacity)) {
            usize size = ast -> capacity * sizeof(AstNode);

            ast -> nodes = arena_resize(ast -> arena, ast -> nodes, size, size * 2);
            ast -> capacity *= 2;
        }

        AstNode* node = &ast -> nodes[ast -> count++];

        Token* token = parser_peek(&p);
        
        switch (token -> kind) {
            case TOK_MODULE: {

            } break;

            case TOK_IMPORT: {
                parser_advance(&p);
                parse_import_decl(&p, node); 
            } break;

            case TOK_EOF: {
                p.file_index += 1;
                p.cursor += 1;
            } break;

            default: {
                err_ast_add(
                    "unexpected top level declaration",
                    "this is not a valid top level declaration",
                    token,
                    LOC_WHOLE_TOK, 
                    p.file_index
                );

                ast -> count -= 1;
            } break;
        }
    }
}

inline Token* parser_peek(Parser* p) {
    ASSERT(p -> cursor < p -> count && "Parser_peek() OOB: Overflow");
    return &p -> tokens -> items[p -> cursor]; 
}

inline Token* parser_advance(Parser* p) {
    ASSERT(p -> cursor < p -> count && "Parser_advance() OOB: Overflow");
    return &p -> tokens -> items[p -> cursor++]; 
}

inline Token* parser_peek_prev(Parser* p) {
    ASSERT(p -> cursor > 0 && "Parser_peek_prev() OOB: Underflow");
    return &p -> tokens -> items[p -> cursor - 1];
}

inline bool parser_check(Parser* p, TokenKind kind) {
    Token* token = parser_peek(p);
    return token -> kind == kind;
}
