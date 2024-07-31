#include "stack.h"

#include <string.h>
#include <stdlib.h>

Stack *stack_new(MBCL_DATA_FREE_FUNCTION freeData)
{
    Stack *wipStack = malloc(sizeof(Stack));
    memset(wipStack, 0, sizeof(Stack));
    wipStack->freeData = freeData;
    wipStack->data = malloc(MBCL_STACK_DEFAULT_ALLOCATION * sizeof(void *));
    wipStack->capacity = MBCL_STACK_DEFAULT_ALLOCATION;
    return wipStack;
}

void stack_free(Stack *stack)
{
    if(stack->freeData)
    {
        while(stack->size > 0)
        {
            stack->size -= 1;
            stack->freeData(stack->data[stack->size]);
        }
    }
    free(stack->data);
    free(stack);
}

void stack_push(Stack *stack, void *data)
{
    if (stack->size == stack->capacity)
    {
        stack->data = realloc(stack->data, stack->size * MBCL_STACK_SCALE_UP_FACTOR * sizeof(void *));
        stack->capacity *= MBCL_STACK_SCALE_UP_FACTOR;
    }

    stack->data[stack->size] = data;
    stack->size++;
}

void *stack_pop(Stack *stack)
{
    void *poppedData = NULL;
    MBCL_ASSERT(stack->size > 0, "Pop from empty stack");
    stack->size -= 1;
    poppedData = stack->data[stack->size];

    if ((stack->size > MBCL_STACK_DEFAULT_ALLOCATION) && (stack->size <= (stack->capacity * MBCL_STACK_SCALE_DOWN_THRESHOLD)))
    {
        stack->data = realloc(stack->data, stack->capacity * MBCL_STACK_SCALE_DOWN_THRESHOLD * sizeof(void *));
    }

    return poppedData;
}

void *stack_peek(Stack *stack)
{
    void *peekedData = NULL;
    MBCL_ASSERT(stack->size > 0, "Peek from empty stack");
    peekedData = stack->data[stack->size - 1];
    return peekedData;
}

Iterator *stack_bottom(Stack *stack)
{
    Iterator *stackIterator = iterator_new(stack->size);
    memcpy(stackIterator->data, stack->data, stack->size * sizeof(void *));
    return stackIterator;
}

Iterator *stack_top(Stack *stack)
{
    Iterator *stackIterator = iterator_new(stack->size);
    memcpy(stackIterator->data, stack->data, stack->size * sizeof(void *));
    stackIterator->pos = stack->size - 1;
    return stackIterator;
}
