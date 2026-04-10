#include "lexer.h"

#include "delims.h"

#include "../diagnostics/diagnostics.h"
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
#define IS_ALPHA_NUMERIC(c) (IS_ALPHA(c) || IS_DIGIT(c))

#define SKIP_WHITESPACE(cursor) \
    while (IS_WHITESPACE(*cursor)) { cursor++; }

#define matches(str, start, length) \
    ((length) == sizeof(str) - 1 && strncmp((str), (start), (length)) == 0)

extern LiliumContext lilium_ctx;

char* lex_word(Tokens* tokens, char* cursor);
char* lex_number(Tokens* tokens, char* cursor);
char* lex_delimeter(Tokens* tokens, DelimStack* stack, char* cursor, u32 file_index);
char* lex_operator(Tokens* tokens, char* cursor);
char* lex_char_lit(Tokens* tokens, char* cursor, u32 file_index);
char* lex_string_lit(Tokens* tokens, char* cursor, u32 file_index);
char* lex_invalid(Tokens* tokens, char* cursor);

void lex_tokens(Tokens* tokens) {
    DelimStack delim_stack = {
        .delims = {0},
        .top = -1
    };

    for (u32 i = lilium_ctx.file_entries.tokenised; i < lilium_ctx.file_entries.count; i++) {
        FileEntry entry = lilium_ctx.file_entries.entries[i];

        char* cursor = entry.buffer;
        char* end = entry.buffer + entry.length; 

        while (cursor < end) {
            if (UNLIKELY(tokens -> count >= tokens -> capacity)) {
                usize size = tokens -> capacity * sizeof(Token);

                tokens -> items = arena_resize(tokens -> arena, tokens -> items, size, size * 2); 
                tokens -> capacity *= 2;
            }

            SKIP_WHITESPACE(cursor);

            char c = *cursor;

            if (IS_ALPHA(c)) {
                cursor = lex_word(tokens, cursor);
            } else if (IS_DIGIT(c)) {
                cursor = lex_number(tokens, cursor);
            } else if (IS_DELIMITER(c)) {
                cursor = lex_delimeter(tokens, &delim_stack, cursor, i);
            } else if (IS_OPERATOR(c)) {
                cursor = lex_operator(tokens, cursor);
            } else if (IS_CHAR_DELIM(c)) {
                cursor = lex_char_lit(tokens, cursor, i);
            } else if (IS_STRING_DELIM(c)) {
                cursor = lex_string_lit(tokens, cursor, i);
            } else {
                cursor = lex_invalid(tokens, cursor);
            }
        }

        if (delim_stack.top != -1) {
            isize count = delim_stack.top;

            for (isize n = 0; n < count + 1; n++) {
                Token* token = delim_stack.delims[n];
                err_delim_stack_unclosed(token, i);
            }
        }
    }

    lilium_ctx.file_entries.tokenised = lilium_ctx.file_entries.count;

}

char* lex_word(Tokens* tokens, char* cursor) {
    Token* token = &tokens -> items[tokens -> count++];

    char* start = cursor;

    while (IS_ALPHA_NUMERIC(*cursor)) {
        cursor++;
    }

    u32 length = cursor - start;

    token -> lexeme = start;
    token -> length = length;

    switch (*start) {
        case 'b': {
            if (matches("bool", start, length)) {
                token -> kind = TOK_BOOL;
                break;
            }

            if (matches("break", start, length)) {
                token -> kind = TOK_BREAK;
                break;
            }

            token -> kind = TOK_IDENT;
        } break;

        case 'c': {
            if (matches("char", start, length)) {
                token -> kind = TOK_CHAR;
                break;
            }

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

            if (matches("f32", start, length)) {
                token -> kind = TOK_F32;
                break;
            }

            if (matches("f64", start, length)) {
                token -> kind = TOK_F64;
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

            if (matches("i8", start, length)) {
                token -> kind = TOK_I8;
                break;
            }

            if (matches("i16", start, length)) {
                token -> kind = TOK_I16;
                break;
            }

            if (matches("i32", start, length)) {
                token -> kind = TOK_I32;
                break;
            }

            if (matches("i64", start, length)) {
                token -> kind = TOK_I64;
                break;
            }

            if (matches("isize", start, length)) {
                token -> kind = TOK_ISIZE;
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
            if (matches("str", start, length)) {
                token -> kind = TOK_STR;
                break;
            }

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
            if (matches("u8", start, length)) {
                token -> kind = TOK_U8;
                break;
            }

            if (matches("u16", start, length)) {
                token -> kind = TOK_U16;
                break;
            }

            if (matches("u32", start, length)) {
                token -> kind = TOK_U32;
                break;
            }

            if (matches("u64", start, length)) {
                token -> kind = TOK_U64;
                break;
            }

            if (matches("usize", start, length)) {
                token -> kind = TOK_USIZE;
                break;
            }

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
    Token* token = &tokens -> items[tokens -> count++];

    char* start = cursor;

    bool is_floating_point = false;

    while (IS_DIGIT(*cursor)) {
        cursor++;
    }

    if (*cursor == '.') {
        is_floating_point = true;

        cursor++;

        while (IS_DIGIT(*cursor)) {
            cursor++;
        }
    }

    u32 length = cursor - start;

    token -> kind   = is_floating_point ? TOK_FLOAT_LIT : TOK_INTEGER_LIT;
    token -> lexeme = start;
    token -> length = length;

    return cursor;
}

char* lex_delimeter(Tokens* tokens, DelimStack* stack, char* cursor, u32 file_index) {
    Token* token = &tokens -> items[tokens -> count++];

    char* start = cursor++;
    
    token -> lexeme = start;
    token -> length = cursor - start;

    switch (*start) {
        case ',': {
            token -> kind = TOK_COMMA;
        } break;

        case ';': {
            token -> kind = TOK_SEMI;
        } break;

        case ':': {
            token -> kind = TOK_COLON;
        } break;

        case '(': {
            token -> kind = TOK_LPAREN;
            delim_stack_push(stack, token, file_index);
        } break;

        case ')': {
            token -> kind = TOK_RPAREN;
            match_delim(stack, token, file_index);
        } break;

        case '[': {
            token -> kind = TOK_LBRACKET;
            delim_stack_push(stack, token, file_index);
        } break;

        case ']': {
            token -> kind = TOK_RBRACKET;
            match_delim(stack, token, file_index);
        } break;

        case '{': {
            token -> kind = TOK_LBRACE;
            delim_stack_push(stack, token, file_index);
        } break;

        case '}': {
            token -> kind = TOK_RBRACE;
            match_delim(stack, token, file_index);
        } break;

        case '\0': {
            token -> kind = TOK_EOF;
        } break;
    }

    return cursor;
}

char* lex_operator(Tokens* tokens, char* cursor) {
    Token* token = &tokens -> items[tokens -> count++];

    char* start = cursor++;

    switch (*start) {
        case '=': {
            if (*cursor == '=') {
                token -> kind = TOK_EQ_EQ;
                cursor++;
                break;
            }

            token -> kind = TOK_EQ;
        } break;

        case '!': {
            if (*cursor == '=') {
                token -> kind = TOK_BANG_EQ;
                cursor++;
                break;
            }

            token -> kind = TOK_BANG;
        } break;

        case '+': {
            if (*cursor == '=') {
                token -> kind = TOK_PLUS_EQ;
                cursor++;
                break;
            }

            token -> kind = TOK_PLUS;
        } break;

        case '-': {
            if (*cursor == '=') {
                token -> kind = TOK_MINUS_EQ;
                cursor++;
                break;
            }

            if (*cursor == '>') {
                token -> kind = TOK_ARROW;
                cursor++;
                break;
            }

            token -> kind = TOK_MINUS;
        } break;

        case '*': {
            if (*cursor == '=') {
                token -> kind = TOK_STAR_EQ;
                cursor++;
                break;
            }

            token -> kind = TOK_STAR;
        } break;

        case '/': {
            if (*cursor == '=') {
                token -> kind = TOK_SLASH_EQ;
                cursor++;
                break;
            }

            if (*cursor == '/') {
                tokens -> count--;

                while (*cursor != 0 && *cursor != '\n') {
                    cursor++;
                }

                if (*cursor != 0) {
                    cursor++;
                }

                break;
            }

            token -> kind = TOK_SLASH;
        } break;

        case '%': {
            if (*cursor == '=') {
                token -> kind = TOK_PERCENT_EQ;
                cursor++;
                break;
            }

            token -> kind = TOK_PERCENT;
        } break;

        case '~': {
            if (*cursor == '=') {
                token -> kind = TOK_TILDE_EQ;
                cursor++;
                break;
            }

            token -> kind = TOK_TILDE;
        } break;

        case '^': {
            if (*cursor == '=') {
                token -> kind = TOK_CARET_EQ;
                cursor++;
                break;
            }

            token -> kind = TOK_CARET;
        } break;

        case '&': {
            if (*cursor == '=') {
                token -> kind = TOK_AMP_EQ;
                cursor++;
                break;
            }

            if (*cursor == '&') {
                token -> kind = TOK_AMP_AMP;
                cursor++;
                break;
            }

            token -> kind = TOK_AMP;
        } break;

        case '|': {
            if (*cursor == '=') {
                token -> kind = TOK_PIPE_EQ;
                cursor++;
                break;
            }

            if (*cursor == '|') {
                token -> kind = TOK_PIPE_PIPE;
                cursor++;
                break;
            }

            token -> kind = TOK_PIPE;
        } break;

        case '>': {
            if (*cursor == '=') {
                token -> kind = TOK_GT_EQ;
                cursor++;
                break;
            }

            if (*cursor == '>') {
                token -> kind = TOK_SHR;
                cursor++;

                if (*cursor == '=') {
                    token -> kind = TOK_SHR_EQ;
                    cursor++;
                }

                break;
            }

            token -> kind = TOK_GT;
        } break;

        case '<': {
            if (*cursor == '=') {
                token -> kind = TOK_LT_EQ;
                cursor++;
                break;
            }

            if (*cursor == '>') {
                token -> kind = TOK_SHL;
                cursor++;

                if (*cursor == '=') {
                    token -> kind = TOK_SHL_EQ;
                    cursor++;
                }

                break;
            }

            token -> kind = TOK_LT;
        } break;

        case '.': {
            if (*cursor == '.') {
                token -> kind = TOK_DOT_DOT;
                cursor++;

                if (*cursor == '.') {
                    token -> kind = TOK_DOT_DOT;
                    cursor++;
                }

                break;
            }

            token -> kind = TOK_DOT;
        } break;

        case '@': {
            token -> kind = TOK_AT;
        } break;
    }

    token -> lexeme = start;
    token -> length = cursor - start;

    return cursor;
}

char* lex_char_lit(Tokens* tokens, char* cursor, u32 file_index) {
    Token* token = &tokens -> items[tokens -> count++];

    token -> kind = TOK_CHAR_LIT;
    token -> lexeme = cursor; 

    char* start = cursor++;

    if (*cursor == '\'') {
        token -> kind = TOK_ERROR;
        token -> length = cursor - start;

        err_lex_char_literal_empty(token, file_index);

        return cursor + 1;
    }

    if (*cursor == '\\') {
        cursor++;
    }

    cursor++;

    if (*cursor != '\'') {
        token -> kind = TOK_ERROR;
        token -> length = cursor - start;

        err_lex_char_literal_unterminated(token, file_index);

        return cursor + 1;
    }

    token -> lexeme = start;
    token -> length = ++cursor - start; 

    return cursor;
}

char* lex_string_lit(Tokens* tokens, char* cursor, u32 file_index) {
    Token* token = &tokens -> items[tokens -> count++];

    char* start = cursor;

    token -> kind = TOK_STRING_LIT;
    token -> lexeme = start; 

    cursor++;

    while (*cursor != '\"' && *cursor != 0) {
        cursor++;

        if (*cursor == '\\') {
            cursor += 2;
        }
    }

    if (*cursor != '\"') {
        err_lex_string_literal_unterminated(token, file_index);
        token -> kind = TOK_ERROR;
    }

    cursor++;

    token -> length = cursor - start;

    return cursor;
}

char* lex_invalid(Tokens* tokens, char* cursor) {
    Token* token = &tokens -> items[tokens -> count++]; 

    char* start = cursor;

    while (
        !IS_ALPHA(*cursor)            &&
        !IS_DIGIT(*cursor)            &&
        !IS_OPERATOR(*cursor)         &&
        !IS_CHAR_DELIM(*cursor)       &&
        !IS_STRING_DELIM(*cursor)     &&
        !IS_DELIMITER(*cursor)        &&
        !IS_WHITESPACE(*cursor)
    ) {
        cursor++;
    }

    token -> kind = TOK_ERROR;
    token -> lexeme = start;
    token -> length = cursor - start;

    // err_unknown_token(token, index);

    return cursor;
}
