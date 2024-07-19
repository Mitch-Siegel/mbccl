#ifndef MBCL_ARRAY_H
#define MBCL_ARRAY_H

#include "mbcl.h"

typedef struct
{
    MBCL_CONTAINER_BASE;
    void **data;
} Array;

void array_init(Array *array, MBCL_DATA_FREE_FUNCTION freeData, size_t size);

void array_deinit(Array *array);

Array *array_new(MBCL_DATA_FREE_FUNCTION freeData, size_t size);

void array_free(Array *array);

void *array_at(Array *array, size_t index);

void array_emplace(Array *array, size_t index, void *data);

void array_resize(Array *array, size_t newSize);

Iterator *array_begin(Array *array);

Iterator *array_end(Array *array);

#endif
