#include "mbcl.h"
#include "iterator.h"
#include <string.h>
#include <stdlib.h>

Iterator *iterator_new(size_t nElements)
{
    Iterator *wipIterator = malloc(sizeof(Iterator));
    memset(wipIterator, 0, sizeof(Iterator));

    wipIterator->nElements = nElements;
    wipIterator->data = malloc(nElements * sizeof(void *));
    memset(wipIterator->data, 0, nElements * sizeof(void *));

    return wipIterator;
}

void iterator_free(Iterator *iterator)
{
    if (iterator->data != NULL)
    {
        free(iterator->data);
    }
    free(iterator);
}

void *iterator_get(Iterator *iterator)
{
    MBCL_ASSERT(iterator_gettable(iterator), "Iterator invalid in iterator_get");
    return iterator->data[iterator->pos];
}

bool iterator_gettable(Iterator *iterator)
{
    return iterator->pos < iterator->nElements;
}

void iterator_next(Iterator *iterator)
{
    MBCL_ASSERT(iterator_gettable(iterator), "Iterator invalid in iterator_next");
    iterator->pos++;
}

void iterator_prev(Iterator *iterator)
{
    MBCL_ASSERT(iterator->pos > 0, "Iterator pos at start in iterator_prev");
    iterator->pos--;
}