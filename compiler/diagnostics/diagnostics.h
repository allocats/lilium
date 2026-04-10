#pragma once

#include "types.h"

#include "../arena/arena.h"
#include "../token/types.h"

void init_diagnostic_context(ArenaAllocator* arena, DiagnosticCtx* ctx);

void diagnostics_print(void);

void err_file_invalid(const char* path);
void err_file_not_found(const char* path);
void err_file_cant_open(const char* path);

void err_lex_char_literal_empty(Token* token, u32 file_index);
void err_lex_char_literal_unterminated(Token* token, u32 file_index);
void err_lex_string_literal_unterminated(Token* token, u32 file_index);

void err_delim_stack_max(Token* token, u32 file_index);
void err_delim_stack_mismatch(Token* token, u32 file_index);
void err_delim_stack_unopened(Token* token, u32 file_index);
void err_delim_stack_unclosed(Token* token, u32 file_index);

void err_ast_add(char* msg, char* help, Token* token, u32 location, u32 file_index);
