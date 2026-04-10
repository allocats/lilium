#include "../parser.h"
#include "../../diagnostics/diagnostics.h"

void parse_module_decl(Parser* p, AstNode* node);

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
