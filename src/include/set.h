#ifndef MBCL_SET_H
#define MBCL_SET_H

#include "rb_tree.h"

typedef RBTree Set;

void set_verify(Set *set);

Set *set_new(MBCL_DATA_FREE_FUNCTION freeData, MBCL_DATA_COMPARE_FUNCTION compareData);

void set_free(Set *set);

void set_insert(Set *set, void *data);

bool set_try_insert(Set *set, void *data);

void *set_find(Set *set, void *data);

void set_remove(Set *set, void *data);

Iterator *set_begin(Set *set);

Set *set_union(Set *setA, Set *setB);

Set *set_union_destructive(Set *setA, Set *setB);

Set *set_copy(Set *set);

void set_clear(Set *set);

#endif
