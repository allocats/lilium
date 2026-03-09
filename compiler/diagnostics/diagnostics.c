#include "diagnostics.h"

#include "../cli/cli.h"
#include "../lilium/types.h"
#include "../utils/macros.h"

#include <stdio.h>

extern DiagnosticCtx diag_ctx;
extern LiliumContext lilium_ctx;

void init_diagnostic_context(ArenaAllocator* arena, DiagnosticCtx* ctx) {
    ctx -> diags = arena_alloc(arena, sizeof(Diagnostic) * 32);
    ctx -> diag_count = 0;
    ctx -> diag_capacity = 32;
    ctx -> error_count = 0;
}

void err_file_invalid(const char* path) {
    fprintf(
        stderr,
        "%serror:%s %sinvalid file '%s' expected 'path/to/file.lily'%s\n",
        ANSI_RED,
        ANSI_RESET,
        ANSI_BOLD,
        path,
        ANSI_RESET
    );
}

void err_file_not_found(const char* path) {
    fprintf(
        stderr,
        "%serror:%s %scannot find file '%s'%s\n",
        ANSI_RED,
        ANSI_RESET,
        ANSI_BOLD,
        path,
        ANSI_RESET
    );
}

void err_file_cant_open(const char* path) {
    fprintf(
        stderr,
        "%serror:%s %scannot open file '%s'%s\n",
        ANSI_RED,
        ANSI_RESET,
        ANSI_BOLD,
        path,
        ANSI_RESET
    );
}

const char* match_level_colour(DiagKind kind) {
    switch (kind) {
        case DIAG_NOTE: {
            return ANSI_BLUE;
        } break;

        case DIAG_WARN: {
            return ANSI_YELLOW;
        } break;

        case DIAG_ERR: {
            return ANSI_RED;
        } break;
    }
}

const char* match_level(DiagKind kind) {
    switch (kind) {
        case DIAG_NOTE: {
            return "note";
        } break;

        case DIAG_WARN: {
            return "warning";
        } break;

        case DIAG_ERR: {
            return "error";
        } break;
    }
}

const char* get_line_col_indent(u32 line) {
    static const char* indents[] = {
        "",
        " ",
        "  ",
        "   ",
        "    ",
        "     ",
        "      ",
        "       ",
        "        ",
        "         ",
        "          "
    };

    u32 digits = 1;

    while (line >= 10) {
        line /= 10;
        digits++;
    }

    if (UNLIKELY(digits >= sizeof(indents) / sizeof(indents[0]))) {
        return indents[sizeof(indents) / sizeof(indents[0]) - 1];
    }

    return indents[digits];
}

const char* get_source_line(const char* buffer, u32 line, u32* len) {
    u32 current_line = 1;

    const char* cursor = buffer;

    while (current_line < line) {
        if (*cursor == '\n') {
            current_line += 1;
        }

        cursor++;
    }

    const char* start = cursor;

    while (*cursor != '\n') {
        cursor++;
    }

    *len = cursor - start;

    return start;
}

void diagnostics_print(void) {
    u32 count = diag_ctx.diag_count > DIAG_SUPPRESS_THRESHOLD ? DIAG_SUPPRESS_THRESHOLD : diag_ctx.diag_count;

    for (u32 i = 0; i < count; i++) {
        Diagnostic diag = diag_ctx.diags[i];

        const char* level_colour = match_level_colour(diag.kind); 
        const char* level = match_level(diag.kind); 

        const char* path = lilium_ctx.file_entries.entries[diag.index].path;
        const char* buffer = lilium_ctx.file_entries.entries[diag.index].buffer;

        const char* line_indent = get_line_col_indent(diag.line);

        u32 source_line_length = 0;
        const char* source_line = get_source_line(buffer, diag.line, &source_line_length);

        // Header 
        fprintf(
            stderr,
            "%s%s:%s %s%s%s\n",
            level_colour,
            level,
            ANSI_RESET,
            ANSI_BOLD,
            diag.msg,
            ANSI_RESET
        );

        // Location
        fprintf(
            stderr,
            " %s%s-->%s %s:%u:%u\n",
            ANSI_MAGENTA,
            ANSI_BOLD,
            ANSI_RESET,
            path,
            diag.line,
            diag.col
        );

        // Source context
        fprintf(stderr, "%s %s|%s\n", line_indent, ANSI_BOLD, ANSI_RESET);
        fprintf(
            stderr,
            "%u %s|%s %.*s\n",
            diag.line,
            ANSI_BOLD,
            ANSI_RESET,
            source_line_length,
            source_line
        );
        fprintf(stderr, "%s %s| %s", line_indent, ANSI_BOLD, ANSI_RESET);

        u32 spaces = diag.col - 1;
        fprintf(stderr, "%*s", spaces, "");

        fprintf(stderr, "%s%s", ANSI_GREEN, ANSI_BOLD);
        u32 caret_len = diag.len;

        for (u32 i = 0; i < caret_len; i++) {
            fprintf(stderr, "^");
        }

        fprintf(stderr, "%s", ANSI_RESET);

        if (diag.help) {
            fprintf(
                stderr,
                "%s%s help: %s%s",
                ANSI_BOLD,
                ANSI_GREEN,
                diag.help,
                ANSI_RESET
            );
        }

        fprintf(stderr, "\n");
        fprintf(stderr, "%s %s|%s\n\n", line_indent, ANSI_BOLD, ANSI_RESET);
    }
}
