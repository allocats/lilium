#include "token.h"
#include "types.h"

#include <stdio.h>

void print_token(Token* token) {
    printf(
        "Kind = %s\nLexeme = \'%.*s\'\nLength = %u\n",
        TOKEN_KIND_STRINGS[token -> kind],
        token -> length,
        token -> lexeme,
        token -> length
    );
}

void print_tokens(FILE* fd, Tokens tokens) {
    fprintf(fd, "\n");

    for (usize i = 0; i < tokens.count; i++) {
        Token token = tokens.items[i];

        fprintf(
            fd,
            "Token %zu :: {\n  Kind: %s\n  Lexeme: \'%.*s\'\n  Length: %u\n}\n\n",
            i,
            TOKEN_KIND_STRINGS[token.kind],
            token.length,
            token.lexeme,
            token.length
        );
    }

    fprintf(
        fd,
        "\nTokens :: [%zu/%zu] %.3f%%\n\n",
        tokens.count,
        tokens.capacity,
        (f32) ((f32) tokens.count / tokens.capacity * 100)
    );
}
