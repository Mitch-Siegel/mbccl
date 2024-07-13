#include "mbcl.h"
#include "iterator.h"
#include <stddef.h>

typedef struct
{
    MBCL_CONTAINER_BASE;
    size_t capacity;
    void **data;
} Stack;

#define MBCL_STACK_DEFAULT_ALLOCATION 10
#define MBCL_STACK_SCALE_FACTOR 1.2

Stack *stack_new(MBCL_DATA_FREE_FUNCTION freeData,
                 MBCL_DATA_COMPARE_FUNCTION compareData);

void stack_free(Stack *stack);

void stack_push(Stack *stack, void *data);

void *stack_pop(Stack *stack);

void *stack_peek(Stack *stack);

void *stack_find(Stack *stack, void *element);

Iterator *stack_begin(Stack *stack);

Iterator *stack_end(Stack *stack);
