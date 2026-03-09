#include "token.h"

#include <stdio.h>

void print_tokens(FILE* fd, Tokens tokens) {
    for (usize i = 0; i < tokens.count; i++) {
        Token token = tokens.items[i];

        fprintf(
            fd,
            "Token %zu :: {\n  Lexeme: %.*s\n  Length: %u\n}\n\n",
            i,
            token.legnth,
            token.lexeme,
            token.legnth
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
