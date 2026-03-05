#include "hash.h"

u32 hash_fnv1a(char* string, usize length) {
    u32 hash = 2166136261u;

    for (u32 i = 0; i < length; i++) {
        hash ^= string[i];
        hash *= 16777619u;
    }

    return hash;
}
