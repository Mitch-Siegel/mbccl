#include "set.h"

Set *set_union(Set *setA, Set *setB)
{
    MBCL_ASSERT(setA->compareData == setB->compareData, "set_union on sets with different compare functions");
    Set *unioned = set_new(NULL, setA->compareData);
    Iterator *unionIterator = NULL;
    for (unionIterator = set_begin(setA); iterator_valid(unionIterator); iterator_next(unionIterator))
    {
        set_insert(unioned, iterator_get(unionIterator));
    }
    iterator_free(unionIterator);
    for (unionIterator = set_begin(setB); iterator_valid(unionIterator); iterator_next(unionIterator))
    {
        set_insert(unioned, iterator_get(unionIterator));
    }
    iterator_free(unionIterator);

    return unioned;
}

Set *set_union_destructive(Set *setA, Set *setB)
{
    Set *unioned = set_union(setA, setB);
    set_free(setA);
    set_free(setB);
    return unioned;
}