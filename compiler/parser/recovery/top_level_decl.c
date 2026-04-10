#include "../parser.h"

void recover_top_level_decl(Parser* p) {
    while (p -> cursor < p -> count) {
        TokenKind kind = parser_peek(p) -> kind;

        if (
            kind == TOK_MODULE    ||
            kind == TOK_IMPORT    ||
            kind == TOK_STRUCT    ||
            kind == TOK_ENUM      ||
            kind == TOK_UNION     ||
            kind == TOK_FN        ||
            kind == TOK_EOF
        ) {
            return;
        }

        parser_advance(p);
    }
}
