#pragma once

#include "../arena/arena.h"
#include "../utils/types.h"
#include "../token/types.h"

typedef struct {
    // Current file index
    u32 file_index;

    // Current index into token array
    usize cursor;

    // Number of tokens
    usize count;

    // Pointer to tokens array
    Tokens* tokens;

    // Pointer to ast arena 
    ArenaAllocator* arena;
} Parser;
