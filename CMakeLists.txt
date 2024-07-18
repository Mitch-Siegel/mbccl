cmake_minimum_required(VERSION 3.22)
project(MBCL VERSION 0.1 LANGUAGES C)

include(GNUInstallDirs)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_compile_options(-Werror -Wall -Wpedantic -Wextra)
add_compile_options(-g)

# Include directories
include_directories(src/include)

# Add the library
add_library(mbcl SHARED
    src/iterator.c
    src/array.c
    src/stack.c
    src/list_node.c
    src/list.c
    src/rb_tree_node.c
    src/rb_tree.c
    src/set.c
    src/deque.c
    src/hash_table.c
)


set(CMAKE_CXX_CLANG_TIDY
  clang-tidy-17;
  -format-style='file';
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

enable_testing()

add_subdirectory(tests)

# Install library
install(TARGETS mbcl
    EXPORT mbclTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ
)

# Ensure sane permissions on library directory after installation
install(CODE "execute_process(COMMAND chmod go+x ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR})")

# Install headers
install(DIRECTORY src/include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/mbcl
    FILE_PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
    DIRECTORY_PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
)

# Install version file
install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/mbclConfigVersion.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/mbcl
    PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ
)

# Install export file
install(EXPORT mbclTargets
    FILE mbclTargets.cmake
    NAMESPACE MBCL::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/mbcl
    PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ
)
