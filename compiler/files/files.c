#include "files.h"

#include "../arena/arena.h"
#include "../diagnostics/diagnostics.h"
#include "../hash/hash.h"

#ifdef DEBUG_MODE
#include <stdio.h>
#endif /* ifdef DEBUG_MODE */

#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

bool file_map(ArenaAllocator* arena, FileEntries* entries, char* path) {
    usize path_length = strnlen(path, PATH_MAX);
    u32 path_hash = hash_fnv1a(path, path_length);

    char* extension = strrchr(path, '.');

    if (!extension || path_length < 6 || strncmp(extension + 1, "lily", 4) != 0) {
        err_file_invalid(path);
        return false;
    }

    for (usize i = 0; i < entries -> count; i++) {
        FileEntry entry = entries -> entries[i];

        if (entry.hash == path_hash && strncmp(entry.path, path, path_length) == 0) {
            return true;
        }
    }

    if (entries -> count >= entries -> capacity) {
        usize size = entries -> capacity * sizeof(FileEntry);

        entries -> entries = arena_realloc(arena, entries -> entries, size, size * 2);
        entries -> capacity *= 2;
    }

    i32 fd = open(path, O_RDONLY);

    if (fd == -1) {
        if (errno == EBADF) {
            err_file_not_found(path);
        } else {
            err_file_cant_open(path);
        }

        return false;
    }

    struct stat st;
    if (fstat(fd, &st) != 0) {
        if (errno == EBADF) {
            err_file_not_found(path);
        } else {
            err_file_cant_open(path);
        }

        close(fd);
        return false;
    }

    usize buffer_length = st.st_size + 1;

    char* buffer = mmap(NULL, buffer_length, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0); 
    if (buffer == MAP_FAILED) {
        // Error
        close(fd);
        return false;
    }

    buffer[buffer_length - 1] = 0;

    FileEntry* slot = &entries -> entries[entries -> count++];

    slot -> path = path;
    slot -> hash = path_hash;
    slot -> buffer = buffer;
    slot -> length = buffer_length;

    #ifdef DEBUG_MODE
    printf("DEBUG: Mapped file: %s\n", path);
    #endif /* ifdef DEBUG_MODE */

    return true;
}

void file_entries_cleanup(FileEntries* entries) {
    for (usize i = 0; i < entries -> count; i++) {
        munmap(entries -> entries[i].buffer, entries -> entries[i].length);
        
        #ifdef DEBUG_MODE
        printf("DEBUG: Unmapped file: %s\n", entries -> entries[i].path);
        #endif /* ifdef DEBUG_MODE */
    }
}
