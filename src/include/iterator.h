#ifndef MBCL_ITERATOR_H
#define MBCL_ITERATOR_H

#include <stddef.h>
#include <stdbool.h>

typedef struct
{
    size_t pos;
    size_t nElements;
    void **data;
} Iterator;

Iterator *iterator_new(size_t nElements);

void iterator_free(Iterator *iterator);

void *iterator_get(Iterator *iterator);

#define iterator_get_as(iterator, type) ((type)iterator_get((iterator)))

void iterator_next(Iterator *iterator);

void iterator_prev(Iterator *iterator);

bool iterator_valid(Iterator *iterator);

#endif
