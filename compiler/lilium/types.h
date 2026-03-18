#pragma once

#include "../arena/arena.h"
#include "../ast/types.h"
#include "../files/types.h"
#include "../token/types.h"

typedef struct {
    ArenaAllocator* arena;
    FileEntries file_entries;
    Tokens tokens;
    Ast ast;
} LiliumContext;
