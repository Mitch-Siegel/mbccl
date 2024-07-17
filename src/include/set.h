#ifndef MBCL_SET_H
#define MBCL_SET_H

#include "rb_tree.h"

typedef RBTree Set;

#define set_new rb_tree_new

#define set_free rb_tree_free

#define set_insert(set, data)                \
    if (rb_tree_find((set), (data)) == NULL) \
    {                                        \
        rb_tree_insert((set), (data));       \
    }

#define set_find rb_tree_find

#define set_remove rb_tree_remove

#define set_begin rb_tree_begin

Set *set_union(Set *setA, Set *setB);

Set *set_union_destructive(Set *setA, Set *setB);

Set *set_copy(Set *set);

void set_clear(Set *set);

#endif
