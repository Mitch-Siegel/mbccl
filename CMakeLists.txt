cmake_minimum_required(VERSION 3.22)
project(MBCL VERSION 0.1 LANGUAGES C)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

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

add_compile_options(-Werror -Wall -Wpedantic -Wextra)

set(CMAKE_CXX_CLANG_TIDY
  clang-tidy-17;
  -format-style='file';
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
