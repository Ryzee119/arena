cmake_minimum_required(VERSION 3.5)
project(arena LANGUAGES C)

add_library(arena
    arena.c
    arena.h
)

target_include_directories(arena PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
