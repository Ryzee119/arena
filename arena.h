// SPDX-License-Identifier: CC0-1.0
// Author: Ryan Wendland 2025

#ifndef ARENA_H
#define ARENA_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

typedef struct
{
    size_t size;
    size_t used;
    void *pool;
} arena_t;

/**
 * @brief Initialize an arena allocator with a memory pool.
 *
 * @param pool       Pointer to the raw memory buffer backing the arena.
 * @param pool_size  Size of the memory buffer in bytes.
 * @param arena      Pointer to an arena_t struct to initialize.
 *
 * @return Pointer to the initialized arena on success, or NULL if arguments are invalid.
 *
 * @note The caller must supply a valid memory pool and arena_t instance.
 */
arena_t *arena_init(void *pool, size_t pool_size, arena_t *arena);

/**
 * @brief Reset the arena, discarding all allocations.
 *
 * @param arena Pointer to the arena.
 *
 * @note This does not free memory; it simply rewinds the allocation pointer
 *       so that the pool can be reused.
 */
void arena_reset(arena_t *arena);

/**
 * @brief Query how many bytes remain available in the arena.
 *
 * @param arena Pointer to the arena.
 * @return Number of bytes still available for allocation.
 */
size_t arena_available(arena_t *arena);

/**
 * @brief Allocate a block of memory from the arena.
 *
 * @param arena Pointer to the arena.
 * @param size  Number of bytes to allocate.
 *
 * @return Pointer to allocated memory, or NULL if insufficient space.
 *
 * @note Memory is not freed individually; it is reclaimed only on reset.
 */
void *arena_alloc(arena_t *arena, size_t size);

/**
 * @brief Allocate and zero-initialize memory from the arena.
 *
 * @param arena Pointer to the arena.
 * @param num   Number of elements to allocate.
 * @param size  Size of each element in bytes.
 *
 * @return Pointer to zeroed memory block, or NULL if insufficient space.
 */
void *arena_calloc(arena_t *arena, size_t num, size_t size);

/**
 * @brief Duplicate a string into the arena.
 *
 * @param arena Pointer to the arena.
 * @param str   NUL-terminated string to duplicate.
 *
 * @return Pointer to duplicated string, or NULL if allocation fails
 *         or if @p str is NULL.
 */
char *arena_strdup(arena_t *arena, const char *str);

/**
 * @brief Duplicate at most @p n characters of a string into the arena.
 *
 * @param arena Pointer to the arena.
 * @param str   NUL-terminated string to duplicate.
 * @param n     Maximum number of characters to copy (excluding NUL).
 *
 * @return Pointer to duplicated string, always NUL-terminated,
 *         or NULL if allocation fails or if @p str is NULL.
 */
char *arena_strndup(arena_t *arena, const char *str, size_t n);

#ifndef ARENA_DISABLE_VSNPRINTF
#include <stdarg.h>

/**
 * @brief Allocate a formatted string in the arena (like asprintf).
 *
 * @param arena Pointer to the arena.
 * @param fmt   printf-style format string.
 * @param ...   Additional arguments for formatting.
 *
 * @return Pointer to formatted string, or NULL if allocation fails
 *         or formatting error occurs.
 */
char *arena_asprintf(arena_t *arena, const char *fmt, ...);

/**
 * @brief Allocate a formatted string in the arena using a va_list.
 *
 * @param arena Pointer to the arena.
 * @param fmt   printf-style format string.
 * @param args  Variable argument list.
 *
 * @return Pointer to formatted string, or NULL if allocation fails
 *         or formatting error occurs.
 *
 * @note This is the va_list variant of arena_asprintf.
 */
char *arena_vasprintf(arena_t *arena, const char *fmt, va_list args);
#endif

#ifdef __cplusplus
}
#endif

#endif // ARENA_H
