// SPDX-License-Identifier: CC0-1.0
// Author: Ryan Wendland 2025

#include "arena.h"
#include <stddef.h>

#ifndef NULL
#define NULL ((void *)0)
#endif

#ifndef ARENA_ALIGNMENT_SIZE
#define ARENA_ALIGNMENT_SIZE (sizeof(void *) - 1)
#endif

#ifndef ARENA_DISABLE_VSNPRINTF
#include <stdarg.h>

#ifndef ARENA_VSNPRINTF_INCLUDE
#define ARENA_VSNPRINTF_INCLUDE <stdio.h>
#endif

#ifndef ARENA_VSNPRINTF
#define ARENA_VSNPRINTF vsnprintf
#endif

#include ARENA_VSNPRINTF_INCLUDE
#endif

arena_t *arena_init(void *pool, size_t pool_size, arena_t *arena)
{
    if (!pool || pool_size == 0 || !arena) {
        return NULL;
    }
    arena->size = pool_size;
    arena->used = 0;
    arena->pool = pool;
    return arena;
}

void arena_reset(arena_t *arena)
{
    arena->used = 0;
}

void *arena_alloc(arena_t *arena, size_t size)
{
    size = (size + ARENA_ALIGNMENT_SIZE) & ~((size_t)ARENA_ALIGNMENT_SIZE);

    if (arena->used + size > arena->size) {
        return NULL;
    }
    void *ptr = (char *)arena->pool + arena->used;
    arena->used += size;
    return ptr;
}

void *arena_calloc(arena_t *arena, size_t num, size_t size)
{
    const size_t sz = num * size;
    void *ptr = arena_alloc(arena, sz);
    if (!ptr) {
        return NULL;
    }

    for (size_t i = 0; i < sz; i++) {
        ((char *)ptr)[i] = 0;
    }

    return ptr;
}

size_t arena_available(arena_t *arena)
{
    return arena->size - arena->used;
}

char *arena_strdup(arena_t *arena, const char *str)
{
    if (!str) {
        return NULL;
    }

    size_t len = 0;
    const char *s = str;
    while (*s++) {
        len++;
    }
    len += 1;

    char *copy = (char *)arena_alloc(arena, len);
    if (copy) {
        for (size_t i = 0; i < len; i++) {
            copy[i] = str[i];
        }
    }
    return copy;
}

char *arena_strndup(arena_t *arena, const char *str, size_t n)
{
    size_t len = 0;
    const char *s = str;
    while (*s++ && len < n) {
        len++;
    }

    char *copy = (char *)arena_alloc(arena, len + 1);
    if (!copy) {
        return NULL;
    }

    for (size_t i = 0; i < len; i++) {
        copy[i] = str[i];
    }
    copy[len] = '\0';

    return copy;
}

#ifndef ARENA_DISABLE_VSNPRINTF
char *arena_vasprintf(arena_t *arena, const char *fmt, va_list args)
{
    va_list args_copy;
    va_copy(args_copy, args);

    int len = ARENA_VSNPRINTF(NULL, 0, fmt, args_copy);
    va_end(args_copy);

    if (len < 0) {
        return NULL;
    }

    len += 1;

    char *str = (char *)arena_alloc(arena, len);
    if (!str) {
        return NULL;
    }

    ARENA_VSNPRINTF(str, len, fmt, args);

    return str;
}

char *arena_asprintf(arena_t *arena, const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    char *str = arena_vasprintf(arena, fmt, args);
    va_end(args);

    return str;
}
#endif // ARENA_DISABLE_VSNPRINTF
