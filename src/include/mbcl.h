#include <assert.h>
#include <aio.h>

#include "iterator.h"

#define MBCL_ASSERT(condition, message) assert((condition) && (message))

typedef void (*MBCL_DATA_FREE_FUNCTION)(void *data);
typedef ssize_t (*MBCL_DATA_COMPARE_FUNCTION)(void *dataA, void *dataB);
typedef Iterator (*MBCL_ITERATOR_CTOR)(void *container);
typedef void (*MBCL_DATA_FIND_FUNCTION)(void *container, void *data);

#define MBCL_CONTAINER_BASE                     \
    struct                                      \
    {                                           \
        size_t size;                            \
        MBCL_DATA_FREE_FUNCTION freeData;       \
        MBCL_DATA_COMPARE_FUNCTION compareData; \
    }
