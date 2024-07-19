#include "array.h"

#include <stdlib.h>
#include <string.h>

void array_init(Array *array, MBCL_DATA_FREE_FUNCTION freeData, size_t size)
{
    memset(array, 0, sizeof(Array));
    array->data = malloc(size * sizeof(void *));
    memset(array->data, 0, size * sizeof(void *));
    array->size = size;
    array->freeData = freeData;
}

void array_deinit(Array *array)
{
    if (array->freeData)
    {
        while (array->size > 0)
        {
            array->size--;
            array->freeData(array->data[array->size]);
        }
    }

    if (array->data)
    {
        free(array->data);
    }
}

Array *array_new(MBCL_DATA_FREE_FUNCTION freeData, size_t size)
{
    Array *wipArray = malloc(sizeof(Array));
    array_init(wipArray, freeData, size);
    return wipArray;
}

void array_free(Array *array)
{
    array_deinit(array);
    free(array);
}

void *array_at(Array *array, size_t index)
{
    MBCL_ASSERT(index < array->size, "Array access out of bounds");

    return array->data[index];
}

void array_emplace(Array *array, size_t index, void *data)
{
    MBCL_ASSERT(index < array->size, "Array access out of bounds");
    array->data[index] = data;
}

void array_resize(Array *array, size_t newSize)
{
    array->data = realloc(array->data, newSize * sizeof(void *));
    array->size = newSize;
    MBCL_ASSERT(array->data != NULL, "realloc failed in array_resize");
}

Iterator *array_begin(Array *array)
{
    Iterator *wipIterator = iterator_new(array->size);
    memcpy(wipIterator->data, array->data, array->size * sizeof(void *));
    return wipIterator;
}

Iterator *array_end(Array *array)
{
    Iterator *wipIterator = iterator_new(array->size);
    memcpy(wipIterator->data, array->data, array->size * sizeof(void *));
    wipIterator->pos = array->size - 1;

    return wipIterator;
}
