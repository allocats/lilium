#include "arena/arena.h"
#include "cli/cli.h"
#include "diagnostics/diagnostics.h"
#include "files/files.h"
#include "lilium/lilium.h"
#include "utils/timer.h"
#include "utils/types.h"

#include <stdio.h>

static ArenaAllocator global_arena = {0};

LiliumContext lilium_ctx = {0};
DiagnosticCtx diag_ctx = {0};

i32 main(i32 argc, char** argv) {
    init_ansi_codes();
     
    if (argc < 2) {
        fprintf(
            stderr,
            "%serror:%s %sno input files%s\n",
            ANSI_RED,
            ANSI_RESET,
            ANSI_BOLD,
            ANSI_RESET
        );

        return 1;
    }

    init_arena(&global_arena, 66536);
    init_diagnostic_context(&global_arena, &diag_ctx);
    init_lilium_context(&lilium_ctx, &global_arena, argc, argv);

    if (lilium_ctx.file_entries.count == 0) {
        fprintf(
            stderr,
            "%serror:%s %sno input files%s\n",
            ANSI_RED,
            ANSI_RESET,
            ANSI_BOLD,
            ANSI_RESET
        );

        goto compiler_exit;
    }

    if (diag_ctx.error_count > 0) goto compiler_exit;

    Timer timer = {0};

    timer_start(&timer);

    // lex()

    timer_end(&timer);

compiler_exit:; // Semicolon hack to allow for declarations, archaic language sigh
    i32 exit_code = 0;

    if (diag_ctx.error_count > 0) {
        exit_code = 1;

        diagnostics_print();

        fprintf(
            stderr,
            "\ncompilation %s%sfailed%s due to %s%s%u%s errors\n",
            ANSI_BOLD,
            ANSI_RED,
            ANSI_RESET,
            ANSI_BOLD,
            ANSI_RED,
            diag_ctx.error_count,
            ANSI_RESET
        );
    } else {
        fprintf(
            stderr,
            "\ncompiled %s%ssuccessfully%s in %.3fms\n",
            ANSI_BOLD,
            ANSI_MAGENTA,
            ANSI_RESET,
            time_elapsed_in_ms(timer)
        );
    }

    file_entries_cleanup(&lilium_ctx.file_entries);
    arena_free(&global_arena);

    return exit_code;
}
