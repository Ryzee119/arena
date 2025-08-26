// SPDX-License-Identifier: CC0-1.0
// Author: Ryan Wendland 2025

#include "../arena.h"

#include <assert.h>
#include <stdio.h>
#include <stdint.h> // uintptr_t
#include <string.h> // strcmp

#define ARENA_SIZE 1024

int main(void)
{
    unsigned char pool[ARENA_SIZE];
    arena_t arena;

    // --- Initialization tests ---
    assert(arena_init(pool, ARENA_SIZE, &arena) == &arena);
    assert(arena.size <= ARENA_SIZE);
    assert(arena.used == 0);
    assert(arena.pool != NULL);
    printf("arena_init passed\n");

    // --- Simple allocation ---
    void *p1 = arena_alloc(&arena, 16);
    assert(p1 != NULL);
    assert(arena.used > 0);
    assert(((uintptr_t)p1 % sizeof(void *)) == 0); // alignment check
    printf("arena_alloc passed\n");

    // --- calloc ---
    int *arr = arena_calloc(&arena, 4, sizeof(int));
    assert(arr != NULL);
    for (int i = 0; i < 4; i++) {
        assert(arr[i] == 0);
    }
    printf("arena_calloc passed\n");

    // --- available ---
    size_t before = arena_available(&arena);
    void *p2 = arena_alloc(&arena, 32);
    assert(p2 != NULL);
    size_t after = arena_available(&arena);
    assert(after < before);
    assert(before - after == 32);
    printf("arena_available passed\n");

    // --- reset ---
    arena_reset(&arena);
    assert(arena.used == 0);
    assert(arena_available(&arena) == arena.size);
    printf("arena_reset passed\n");

    // --- strdup ---
    const char *hello = "hello";
    char *dup = arena_strdup(&arena, hello);
    assert(dup != NULL);
    assert(strcmp(dup, hello) == 0);
    printf("arena_strdup passed\n");

    // --- strndup ---
    const char *longstr = "abcdef";
    char *dup2 = arena_strndup(&arena, longstr, 3);
    assert(dup2 != NULL);
    assert(strcmp(dup2, "abc") == 0);
    printf("arena_strndup passed\n");

    // --- asprintf ---
    // --- vasprintf --- (tested internally)
    char *fmt1 = arena_asprintf(&arena, "num=%d", 42);
    assert(fmt1 != NULL);
    assert(strcmp(fmt1, "num=42") == 0);
    printf("arena_asprintf passed\n");

    // --- Out-of-memory test ---
    arena_reset(&arena);
    void *too_big = arena_alloc(&arena, ARENA_SIZE + 1);
    assert(too_big == NULL);
    printf("Out-of-memory test passed\n");

    printf("All arena tests passed!\n");
    return 0;
}
