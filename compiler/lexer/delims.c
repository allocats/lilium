#include "delims.h"

#include <stdio.h>

void match_delim(DelimStack* stack, Token* token, u32 index) {
    Token* popped = delim_stack_pop(stack);

    if (popped == nullptr) {
        // err_delim_unopened(token, index);
        return;
    }

    switch (token -> kind) {
        case TOK_RPAREN: {
            if (popped -> kind != TOK_LPAREN) {
                // err_delim_mismatch(token, index);
            }
        } break;

        case TOK_RBRACE: {
            if (popped -> kind != TOK_LBRACE) {
                // err_delim_mismatch(token, index);
            }
        } break;

        case TOK_RBRACKET: {
            if (popped -> kind != TOK_LBRACKET) {
                // err_delim_mismatch(token, index);
            }
        } break;

        default: {
            fprintf(
                stderr,
                "UNREACHABLE [%s:%u]: Hit default case in match_delim(). Please report\n",
                __FILE__,
                __LINE__
            );
            exit(1);
        } break;
    }
}

void delim_stack_push(DelimStack* stack, Token* token, u32 index)  {
    if (stack -> top + 1 >= MAX_DELIM_STACK) {
        // err_delim_stack_max(token, index);
        return;
    }

    stack -> delims[++stack -> top] = token;
}

Token* delim_stack_pop(DelimStack* stack)  {
    if (stack -> top < 0) return null;
    return stack -> delims[stack -> top--];
}
