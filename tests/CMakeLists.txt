
include_directories(include)

# Add test executable
add_executable(test-rb-tree test_rb_tree.c)
target_link_libraries(test-rb-tree mbcl)

# Add the test to CTest
add_test(NAME rb_tree_test_ascending_insertion COMMAND test-rb-tree 0)
add_test(NAME rb_tree_test_descending_insertion COMMAND test-rb-tree 1)
add_test(NAME rb_tree_test_worst_case_insertion COMMAND test-rb-tree 2)
