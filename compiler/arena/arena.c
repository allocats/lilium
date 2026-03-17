#include "arena.h"

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h> 
#include <string.h>

#define UNLIKELY(x) __builtin_expect(x, 0)
#define LIKELY(x) __builtin_expect(x, 1)

extern void* arena_realloc_avx2(ArenaAllocator* arena, void* ptr, const size_t old_size, const size_t new_size);
extern void* arena_memcpy_avx2(void* dest, const void* src, size_t len);
extern void* arena_memset_avx2(void* ptr, int const value, size_t len);

extern void* arena_realloc_sse2(ArenaAllocator* arena, void* ptr, const size_t old_size, const size_t new_size);
extern void* arena_memcpy_sse2(void* dest, const void* src, size_t len);
extern void* arena_memset_sse2(void* ptr, int const value, size_t len);

extern void* arena_realloc_generic(ArenaAllocator* arena, void* ptr, const size_t old_size, const size_t new_size);
extern void* arena_memcpy_generic(void* dest, const void* src, size_t len);
extern void* arena_memset_generic(void* ptr, int const value, size_t len);

static void* (*arena_realloc_impl)(ArenaAllocator* arena, void* ptr, const size_t old_size, const size_t new_size);
static void* (*arena_memcpy_impl)(void* dest, const void* src, size_t len);
static void* (*arena_memset_impl)(void* ptr, const int value, size_t len);

__attribute__((constructor)) static void arena_dispatch(void) {
    __builtin_cpu_init();

    if (__builtin_cpu_supports("avx2")) {
        arena_realloc_impl = arena_realloc_avx2;
        arena_memcpy_impl = arena_memcpy_avx2;
        arena_memset_impl = arena_memset_avx2;
    } else if (__builtin_cpu_supports("sse2")) {
        arena_realloc_impl = arena_realloc_sse2;
        arena_memcpy_impl = arena_memcpy_sse2;
        arena_memset_impl = arena_memset_sse2;
    } else {
        arena_realloc_impl = arena_realloc_generic;
        arena_memcpy_impl = arena_memcpy_generic;
        arena_memset_impl = arena_memset_generic;
    }
}

void* arena_realloc(ArenaAllocator* arena, void* ptr, const size_t old_size, const size_t new_size) {
    return arena_realloc_impl(arena, ptr, old_size, new_size);
}

void* arena_memcpy(void* dest, const void* src, size_t len) {
    return arena_memcpy_impl(dest, src, len);
}

void* arena_memset(void* ptr, const int value, size_t len) {
    return arena_memset_impl(ptr, value, len);
}

inline size_t align_size(const size_t size) {
    return (size + 31) & ~(31);
}

void init_arena(ArenaAllocator* arena, const size_t default_capacity) {
    assert(arena);
    arena -> start = NULL;
    arena -> current = arena -> start;
    arena -> end = NULL;
    arena -> default_capacity = default_capacity == 0 ? ARENA_DEFAULT_CAPACITY : align_size(default_capacity);
}

static ArenaBlock* new_block(size_t default_capacity, size_t size) {
    size_t capacity = default_capacity;

    while (UNLIKELY(size > capacity * sizeof(uintptr_t))) {
        capacity *= 2;
    }

    const size_t bytes = capacity * sizeof(uintptr_t);
    const size_t total_size = sizeof(ArenaBlock) + bytes;
    const size_t aligned_size = align_size(total_size);

    ArenaBlock* block = (ArenaBlock*) aligned_alloc(32, aligned_size);
    assert(block && "Buy more ram silly :3");

    memset(block, 0, aligned_size);

    block -> next = NULL;
    block -> usage =  0;
    block -> capacity = bytes;

    return block;
}

static inline void free_block(ArenaBlock* block) {
    free(block);
}

void* arena_alloc(ArenaAllocator* arena, const size_t size) {
    const size_t aligned = align_size(size);
    ArenaBlock* block = arena -> current;

    if (UNLIKELY(!block)) {
        block = new_block(arena -> default_capacity, aligned);

        arena -> start = block;
        arena -> current = block;
        arena -> end = block;
    }

    if (UNLIKELY(block -> usage + aligned > block -> capacity)) {
        ArenaBlock* next = new_block(arena -> default_capacity, aligned);

        block -> next = next;
        arena -> end = next;
        arena -> current = next;
        block = next;
    }

    void* result = (char*) block -> data + block -> usage;
    block -> usage += aligned;
    return result;
}

void* arena_resize(ArenaAllocator* arena, void* ptr, const size_t old_size, const size_t new_size) {
    const size_t aligned_old = align_size(old_size);
    const size_t aligned_new = align_size(new_size);

    ArenaBlock* block = arena -> current;

    char* expected = (char*) block -> data + block -> usage - aligned_old;

    if (LIKELY(ptr == expected)) {
        const size_t extra = aligned_new - aligned_old;

        if (LIKELY(block -> usage + extra <= block -> capacity)) {
            arena_memset_impl((char*) ptr + aligned_old, 0, extra);
            block -> usage += extra;
            return ptr;
        }

        block -> usage -= aligned_old;
    }

    void* new_ptr = arena_alloc(arena, aligned_new);
    arena_memcpy_impl(new_ptr, ptr, old_size);
    return new_ptr;
}

char* arena_strdup(ArenaAllocator* arena, const char* str) {
    const size_t len = strlen(str);
    char* duplicate = (char*) arena_alloc(arena, len + 1);

    arena_memcpy(duplicate, str, len + 1);
    duplicate[len] = '\0';

    return duplicate;
}

inline void arena_reset(ArenaAllocator* arena) {
    for (ArenaBlock* block = arena -> start; block != NULL; block = block -> next) {
        block -> usage = 0;
    }

    arena -> end = arena -> start;
}

void free_arena(ArenaAllocator* arena) {
    ArenaBlock* block = arena -> start;

    while (block != NULL) {
        ArenaBlock* previous = block;
        block = block -> next;
        free_block(previous);
    }

    arena -> start = NULL;
    arena -> end = NULL;
}

void print_arena_stats(const ArenaAllocator* arena, const char* label) {
    size_t usage = total_usage(arena);
    size_t capacity = total_capacity(arena);

    const char* name = label != NULL ? label : "Arena";

    printf("\n========= %s Stats =========\n\n", name);
    printf("    Capacity: %zu bytes\n", capacity);
    printf("    Usage: %zu bytes\n", usage);
    printf("    Percentage: %.4f%%\n\n", (double)usage / (double)capacity * 100);
    printf("========= %s Stats =========\n\n", name);
}

size_t total_capacity(const ArenaAllocator* arena) {
    const ArenaBlock* current = arena -> start;
    size_t total = 0;

    while (current != NULL) {
        total += current -> capacity;
        current = current -> next;
    }
    
    return total;
}

size_t total_usage(const ArenaAllocator* arena) {
    const ArenaBlock* current = arena -> start;
    size_t total = 0;

    while (current != NULL) {
        total += current -> usage;
        current = current -> next;
    }
    
    return total;
}
