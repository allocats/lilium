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
    u32 count;
    u32 capacity;

    // essentially a checkpoint marker
    u32 tokenised;
} FileEntries;
