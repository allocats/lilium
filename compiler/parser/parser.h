#pragma once

#include "types.h"

#include "../arena/arena.h"
#include "../ast/types.h"
#include "../token/types.h"

// Entry function
void parse_tokens(Ast* ast, Tokens* tokens);

// Top level declarations
void parse_import_decl(Parser* p, AstNode* node);

// Recovery
void recover_top_level_decl(Parser* p);

// Fails (Sets node to error and recovers)
void top_level_decl_fail(Parser* p, AstNode* node);

// Token navigation
Token* parser_peek(Parser* p);
Token* parser_advance(Parser* p);
Token* parser_peek_prev(Parser* p);
bool   parser_check(Parser* p, TokenKind kind);
