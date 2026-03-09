#include "lilium.h"

#include "../diagnostics/types.h"
#include "../files/files.h"

#include <stdio.h>

extern DiagnosticCtx diag_ctx;

void init_lilium_context(LiliumContext* lilium_ctx, ArenaAllocator* arena, i32 argc, char** argv) {
    lilium_ctx -> arena = arena;

    lilium_ctx -> tokens.items = arena_alloc(arena, sizeof(Token) * 256);
    lilium_ctx -> tokens.count = 0;
    lilium_ctx -> tokens.capacity = 256;

    lilium_ctx -> file_entries.entries = arena_alloc(arena, sizeof(FileEntry) * 32);
    lilium_ctx -> file_entries.count = 0;
    lilium_ctx -> file_entries.capacity = 32;
    lilium_ctx -> file_entries.tokenised = 0;

    for (i32 i = 1; i < argc; i++) {
        char* arg = argv[i];

        if (arg[0] == '-') {
            printf("TODO: Support arguments (%s)\n", arg);
        } else {
            bool result = file_map(arena, &lilium_ctx -> file_entries, arg);

            if (!result) {
                diag_ctx.error_count += 1;
            }
        }
    }
}
