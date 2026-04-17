#include "../parser.h"
#include "../../diagnostics/diagnostics.h"
#include "../../utils/macros.h"

void parse_module_decl(Parser* p, AstNode* node) {
    node -> kind = AST_MODULE;

    node -> module_decl.ptrs = arena_alloc(p -> arena, sizeof(char*) * 4);
    node -> module_decl.lens = arena_alloc(p -> arena, sizeof(u32) * 4);
    node -> module_decl.count = 0;
    node -> module_decl.capacity = 4;

    while (p -> cursor < p -> count) {
        Token* token = parser_peek(p);

        if (token -> kind != TOK_IDENT) {
            err_ast_add(
                "expected identifier for module declaration",
                "add a valid identifier here",
                token,
                LOC_WHOLE_TOK,
                p -> file_index
            );

            top_level_decl_fail(p, node);
            return;
        }

        if (UNLIKELY(node -> module_decl.count >= node -> module_decl.capacity)) {
            usize size = sizeof(char*) * node -> module_decl.capacity;

            node -> module_decl.ptrs = arena_realloc(
                p -> arena,
                node -> module_decl.ptrs,
                size,
                size * 2
            );

            size = sizeof(u32) * node -> module_decl.capacity;

            node -> module_decl.lens = arena_realloc(
                p -> arena,
                node -> module_decl.lens,
                size,
                size * 2
            );

            node -> module_decl.capacity *= 2;
        }

        node -> module_decl.ptrs[node -> module_decl.count] = token -> lexeme;
        node -> module_decl.lens[node -> module_decl.count] = token -> length;
        node -> module_decl.count += 1; 

        parser_advance(p);

        token = parser_peek(p);

        if (token -> kind == TOK_SEMI) {
            parser_advance(p);
            return;
        }

        if (token -> kind != TOK_DOT) {
            err_ast_add(
                "expected '.'",
                "add a '.' here",
                parser_peek_prev(p),
                LOC_END_OF_TOK,
                p -> file_index
            );

            top_level_decl_fail(p, node);
            return;
        }

        parser_advance(p);
    } 

    err_ast_add(
        "unexpected end of file in module declaration",
        "add a ';' to terminate the module declaration",
        parser_peek_prev(p),
        LOC_END_OF_TOK,
        p->file_index
    );
    top_level_decl_fail(p, node);
}

void parse_import_decl(Parser* p, AstNode* node) {
    node -> kind = AST_IMPORT;

    Token* token = parser_peek(p);

    if (token -> kind != TOK_STRING_LIT) {
        err_ast_add(
            "expected string literal for import",
            "add a valid string literal here",
            parser_peek(p),
            LOC_WHOLE_TOK,
            p -> file_index
        );

        top_level_decl_fail(p, node);
        return;
    }

    node -> import_decl.ptr = token -> lexeme;
    node -> import_decl.len = token -> length;

    parser_advance(p);

    if (!parser_check(p, TOK_SEMI)) {
        err_ast_add(
            "expeced ';'",
            "add a ';' here",
            parser_peek_prev(p),
            LOC_END_OF_TOK,
            p -> file_index
        );

        top_level_decl_fail(p, node);
        return;
    }

    parser_advance(p);
}
