#include "set.h"

#include "array.h"
void set_verify(Set *set)
{

    Iterator *setI = set_begin(set);
    for (size_t i = 0; i < set->size; i++)
    {
        for (size_t j = i + 1; j < set->size; j++)
        {
            MBCL_ASSERT(set->compareData(setI->data[i], setI->data[j]), "duplicate elements in set!");
        }
    }
    iterator_free(setI);
}

Set *set_new(MBCL_DATA_FREE_FUNCTION freeData, MBCL_DATA_COMPARE_FUNCTION compareData)
{
    return rb_tree_new(freeData, compareData);
}

void set_free(Set *set)
{
    rb_tree_free(set);
}

void set_insert(Set *set, void *data)
{
    // MBCL_ASSERT(rb_tree_find((set), (data)) == NULL, "Duplicate set insertion");
    rb_tree_insert((set), (data));
    set_verify(set);
}

bool set_try_insert(Set *set, void *data)
{
    if (set_find(set, data) == NULL)
    {
        rb_tree_insert(set, data);
        return true;
    }
    return false;
}

void *set_find(Set *set, void *data)
{
    return rb_tree_find(set, data);
}

void set_remove(Set *set, void *data)
{
    rb_tree_remove(set, data);
}

Iterator* set_begin(Set *set)
{
    return rb_tree_begin(set);
}

Set *set_union(Set *setA, Set *setB)
{
    MBCL_ASSERT(setA->compareData == setB->compareData, "set_union on sets with different compare functions");
    Set *unioned = set_new(NULL, setA->compareData);
    Iterator *unionIterator = NULL;

    for (unionIterator = set_begin(setA); iterator_valid(unionIterator); iterator_next(unionIterator))
    {
        void *unionedData = iterator_get(unionIterator);
        if (!set_try_insert(unioned, unionedData) && (setA->freeData != NULL))
        {
            setA->freeData(unionedData);
        }
    }
    iterator_free(unionIterator);
    for (unionIterator = set_begin(setB); iterator_valid(unionIterator); iterator_next(unionIterator))
    {
        void *unionedData = iterator_get(unionIterator);
        if (!set_try_insert(unioned, unionedData) && (setB->freeData != NULL))
        {
            setB->freeData(unionedData);
        }
    }
    iterator_free(unionIterator);

    return unioned;
}

Set *set_union_destructive(Set *setA, Set *setB)
{
    Set *unioned = set_union(setA, setB);
    setA->freeData = NULL;
    set_free(setA);
    setB->freeData = NULL;
    set_free(setB);
    return unioned;
}
#include <stdio.h>
Set *set_copy(Set *set)
{
    Set *copied = set_new(NULL, set->compareData);
    Iterator *copyIterator = set_begin(set);
    while (iterator_valid(copyIterator))
    {
        set_insert(copied, iterator_get(copyIterator));
        iterator_next(copyIterator);
    }
    iterator_free(copyIterator);
    return copied;
}

void set_clear(Set *set)
{
    while (set->size > 0)
    {
        set_remove(set, set->root->data);
    }
}
