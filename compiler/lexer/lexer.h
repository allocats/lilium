#pragma once

#include "types.h"

#include "../arena/arena.h"
#include "../token/types.h"

void lex_tokens(ArenaAllocator* tokens_arena, Tokens* tokens);
