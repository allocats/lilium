#include "lexer.h"

#include "delims.h"

#include "../lilium/types.h"
#include "../utils/macros.h"

#include <string.h>

#define IS_DIGIT(c) (char_map[(unsigned char)(c)] & 1)
#define IS_ALPHA(c) (char_map[(unsigned char)(c)] & 2)
#define IS_OPERATOR(c) (char_map[(unsigned char)(c)] & 4)
#define IS_DELIMITER(c) (char_map[(unsigned char)(c)] & 8)
#define IS_WHITESPACE(c) (char_map[(unsigned char)(c)] & 16)
#define IS_CHAR_DELIM(c) (char_map[(unsigned char)(c)] & 32)
#define IS_STRING_DELIM(c) (char_map[(unsigned char)(c)] & 64)
#define IS_ALPHA_NUMERIC(c) (IS_ALPHA(c) && IS_DIGIT(c))

#define SKIP_WHITESPACE(cursor) \
    while (IS_WHITESPACE(*cursor)) { cursor++; }

#define matches(str, start, length) \
    ((length) == sizeof(str) - 1 && strncmp((str), (start), (length)) == 0)

extern LiliumContext lilium_ctx;

char* lex_word(Tokens* tokens, char* cursor);
char* lex_number(Tokens* tokens, char* cursor);
char* lex_delimeter(char* cursor);
char* lex_operator(char* cursor);
char* lex_char_lit(char* cursor);
char* lex_string_lit(char* cursor);
char* lex_invalid(char* cursor);

void lex_tokens(Tokens* tokens) {
    for (u32 i = lilium_ctx.file_entries.tokenised; i < lilium_ctx.file_entries.count; i++) {
        FileEntry entry = lilium_ctx.file_entries.entries[i];

        char* cursor = entry.buffer;
        char* end = entry.buffer + entry.length; 

        while (cursor < end) {
            if (UNLIKELY(tokens -> count >= tokens -> capacity)) {
                usize size = tokens -> capacity * sizeof(Token);

                tokens -> items = arena_realloc(lilium_ctx.arena, tokens -> items, size, size * 2); 
                tokens -> capacity *= 2;
            }

            SKIP_WHITESPACE(cursor);

            char c = *cursor;

            if (IS_ALPHA(c)) {
                cursor = lex_word(tokens, cursor);
            } else if (IS_DELIMITER(c)) {
                cursor = lex_number(tokens, cursor);
            } else if (IS_DIGIT(c)) {
            } else if (IS_OPERATOR(c)) {
            } else if (IS_CHAR_DELIM(c)) {
            } else if (IS_STRING_DELIM(c)) {
            } else {
            }
        }
    }

    lilium_ctx.file_entries.tokenised = lilium_ctx.file_entries.count;
}

char* lex_word(Tokens* tokens, char* cursor) {
    Token* token = &tokens -> items[tokens -> count += 1];

    char* start = cursor;

    while (IS_ALPHA_NUMERIC(*cursor)) {
        cursor += 1;
    }

    u32 length = cursor - start;

    token -> lexeme = start;
    token -> length = length;

    switch (*start) {
        case 'b': {
            if (matches("break", start, length)) {
                token -> kind = TOK_BREAK;
                break;
            }

            token -> kind = TOK_IDENT;
        } break;

        case 'c': {
            if (matches("const", start, length)) {
                token -> kind = TOK_CONST;
                break;
            }

            if (matches("continue", start, length)) {
                token -> kind = TOK_CONTINUE;
                break;
            }

            token -> kind = TOK_IDENT;
        } break;

        case 'd': {
            if (matches("defer", start, length)) {
                token -> kind = TOK_DEFER;
                break;
            }

            token -> kind = TOK_IDENT;
        } break;

        case 'e': {
            if (matches("else", start, length)) {
                token -> kind = TOK_ELSE;
                break;
            }

            if (matches("enum", start, length)) {
                token -> kind = TOK_ENUM;
                break;
            }

            if (matches("external", start, length)) {
                token -> kind = TOK_EXTERNAL;
                break;
            }

            token -> kind = TOK_IDENT;
        } break;

        case 'f': {
            if (matches("fn", start, length)) {
                token -> kind = TOK_FN;
                break;
            }

            if (matches("for", start, length)) {
                token -> kind = TOK_FOR;
                break;
            }

            if (matches("false", start, length)) {
                token -> kind = TOK_FALSE;
                break;
            }

            token -> kind = TOK_IDENT;
        } break;

        case 'i': {
            if (matches("if", start, length)) {
                token -> kind = TOK_IF;
                break;
            }

            if (matches("import", start, length)) {
                token -> kind = TOK_IMPORT;
                break;
            }

            if (matches("inline", start, length)) {
                token -> kind = TOK_INLINE;
                break;
            }

            token -> kind = TOK_IDENT;
        } break;

        case 'l': {
            if (matches("let", start, length)) {
                token -> kind = TOK_LET;
                break;
            }

            if (matches("loop", start, length)) {
                token -> kind = TOK_LOOP;
                break;
            }

            token -> kind = TOK_IDENT;
        } break;

        case 'm': {
            if (matches("match", start, length)) {
                token -> kind = TOK_MATCH;
                break;
            }

            if (matches("macro", start, length)) {
                token -> kind = TOK_MACRO;
                break;
            }

            if (matches("module", start, length)) {
                token -> kind = TOK_MODULE;
                break;
            }

            token -> kind = TOK_IDENT;
        } break;

        case 'n': {
            if (matches("null", start, length)) {
                token -> kind = TOK_NULL;
                break;
            }

            token -> kind = TOK_IDENT;
        } break;

        case 'r': {
            if (matches("return", start, length)) {
                token -> kind = TOK_RETURN;
                break;
            }

            token -> kind = TOK_IDENT;
        } break;

        case 's': {
            if (matches("struct", start, length)) {
                token -> kind = TOK_STRUCT;
                break;
            }

            if (matches("static", start, length)) {
                token -> kind = TOK_STATIC;
                break;
            }

            token -> kind = TOK_IDENT;
        } break;

        case 't': {
            if (matches("true", start, length)) {
                token -> kind = TOK_TRUE;
                break;
            }

            token -> kind = TOK_IDENT;
        } break;

        case 'u': {
            if (matches("union", start, length)) {
                token -> kind = TOK_UNION;
                break;
            }

            token -> kind = TOK_IDENT;
        } break;

        case 'w': {
            if (matches("while", start, length)) {
                token -> kind = TOK_WHILE;
                break;
            }

            token -> kind = TOK_IDENT;
        } break;

        default: {
            token -> kind = TOK_IDENT;
        } break;
    }

    return cursor;
}

char* lex_number(Tokens* tokens, char* cursor) {
    Token* token = &tokens -> items[tokens -> count += 1];

    char* start = cursor;

    bool is_floating_point = false;

    while (IS_DIGIT(*cursor)) {
        cursor += 1;
    }

    if (*cursor == '.') {
        is_floating_point = true;

        cursor += 1;

        while (IS_DIGIT(*cursor)) {
            cursor += 1;
        }
    }

    u32 length = cursor - start;

    token -> kind   = is_floating_point ? TOK_FLOAT_LIT : TOK_INTEGER_LIT;
    token -> lexeme = start;
    token -> length = length;

    return cursor;
}

char* lex_delimeter(char* cursor) {

}

char* lex_operator(char* cursor) {

}

char* lex_char_lit(char* cursor) {

}

char* lex_string_lit(char* cursor) {

}

char* lex_invalid(char* cursor) {

}
