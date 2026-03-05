#pragma once

#include "../utils/types.h"

typedef struct {
    u8 _padding[4];
    u32 hash;

    char *buffer;
    usize length;

    char* path;
} FileEntry;

typedef struct {
    FileEntry* entries;
    usize count;
    usize capacity;
} FileEntries;
