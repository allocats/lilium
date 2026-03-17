#pragma once

#include "types.h"

#include "../arena/arena.h"

void init_lilium_context(LiliumContext* lilium_ctx, ArenaAllocator* global_arena, ArenaAllocator* tokens_arena, i32 argc, char** argv);
