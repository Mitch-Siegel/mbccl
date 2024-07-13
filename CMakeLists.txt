cmake_minimum_required(VERSION 3.22)
project(MBCL VERSION 0.1 LANGUAGES C)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_compile_options(-Werror -Wall -Wpedantic -Wextra)
add_compile_options(-g)

# Include directories
include_directories(src/include)

# Add the library
add_library(mbcl
    src/iterator.c
    src/stack.c
    src/list_node.c
    src/list.c
    src/rb_tree_node.c
    src/rb_tree.c
)


set(CMAKE_CXX_CLANG_TIDY
  clang-tidy-17;
  -format-style='file';
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

enable_testing()

add_subdirectory(tests)
