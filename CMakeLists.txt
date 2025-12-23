cmake_minimum_required(VERSION 3.15)

project(dynarr
    VERSION 0.1.0
    LANGUAGES C
)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

add_library(dynarr
    src/dynarr.c
)

target_include_directories(dynarr
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)

target_compile_options(dynarr
    PRIVATE
        -Wall
        -Wextra
        -Wpedantic
)

add_executable(dumb_test
    tests/dumb.c
)

target_link_libraries(dumb_test
    PRIVATE dynarr
)
