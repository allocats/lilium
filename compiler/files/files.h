#pragma once

#include "types.h"

#include "../arena/arena.h"

bool file_map(ArenaAllocator* arena, FileEntries* entries, char* path);
void file_entries_cleanup(FileEntries* entries);
