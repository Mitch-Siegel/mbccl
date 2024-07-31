#ifndef MBCL_STACK_H
#define MBCL_STACK_H

#include "mbcl.h"
#include "iterator.h"
#include <stddef.h>
#include "array.h"

typedef struct
{
    MBCL_CONTAINER_BASE;
    void **data;
    size_t capacity;
} Stack;

#define MBCL_STACK_DEFAULT_ALLOCATION 10
#define MBCL_STACK_SCALE_UP_FACTOR 1.2
#define MBCL_STACK_SCALE_DOWN_THRESHOLD 0.6

Stack *stack_new(MBCL_DATA_FREE_FUNCTION freeData);

void stack_free(Stack *stack);

void stack_push(Stack *stack, void *data);

void *stack_pop(Stack *stack);

void *stack_peek(Stack *stack);

Iterator *stack_bottom(Stack *stack);

Iterator *stack_top(Stack *stack);

#endif
