#ifndef MBCL_SET_H
#define MBCL_SET_H

#include "rb_tree.h"

typedef RBTree Set;

#define set_new(freeData, compareData) rb_tree_new(freeData, compareData)

#define set_free(set) rb_tree_free((set))

#define set_insert(set, data)                \
    if (rb_tree_find((set), (data)) == NULL) \
    {                                        \
        rb_tree_insert((set), (data));       \
    }

#define set_find(set, data) rb_tree_find((set), (data))

#define set_remove(set, data) rb_tree_remove((set), (data))

#define set_begin(set) rb_tree_begin((set))

Set *set_union(Set *setA, Set *setB);

Set *set_union_destructive(Set *setA, Set *setB);

#endif
