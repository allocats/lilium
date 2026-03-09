#pragma once

#include "types.h"

#include "../arena/arena.h"

void init_diagnostic_context(ArenaAllocator* arena, DiagnosticCtx* ctx);

void diagnostics_print(void);

void err_file_invalid(const char* path);
void err_file_not_found(const char* path);
void err_file_cant_open(const char* path);

void err_delim_stack_max();
void err_delim_stack_mismatch();
void err_delim_stack_unopened();
