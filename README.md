# Minimal Arena allocator

* Zero memory allocations
* Portable (Zero dependancy, minimal standard c library requirements. Only relies on `stddef.h` and optionally `stdio.h` and `stdarg.h`)
* C99 compliant
* CC0 license
* You can disable vsnprintf, stdio and stdarg dependancies by defining `ARENA_DISABLE_VSNPRINTF`. You can also point vsnprintf to a custom implementation.

## Example
```c
#include "../arena.h"
#include <stdio.h>
#include <string.h> // For strlen

int main(void)
{
    arena_t arena;
    void *arena_pool[1024];

    arena_init(arena_pool, 1024, &arena);

    void *p1 = arena_alloc(&arena, 32);
    void *p2 = arena_calloc(&arena, 1, 32);
    printf("Created two allocations %p and %p\n", p1, p2);

    size_t remaining = arena_available(&arena);
    printf("You have %zu bytes remaining\n", remaining);

    char *example = "You can also copy strings";
    char *new_string = arena_strdup(&arena, example);

    char *message = arena_asprintf(&arena, "You copied a string with %zu characters\n",
                                    strlen(new_string));
    printf("%s", message);

    // No complicated heap freeing. This clears everything
    arena_reset(&arena);
    return 0;
}
```

## Integration (CMake)
```
include(FetchContent)
FetchContent_Declare(
    arena
    GIT_REPOSITORY https://github.com/Ryzee119/arena.git
    GIT_TAG main
)
FetchContent_MakeAvailable(arena)
target_link_libraries(my_app PRIVATE arena)
```
