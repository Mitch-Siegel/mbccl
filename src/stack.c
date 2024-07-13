#include "stack.h"

#include <string.h>
#include <stdlib.h>

Stack *stack_new(MBCL_DATA_FREE_FUNCTION freeData,
                 MBCL_DATA_COMPARE_FUNCTION compareData)
{
    Stack *wipStack = malloc(sizeof(Stack));
    memset(wipStack, 0, sizeof(Stack));
    wipStack->data = malloc(MBCL_STACK_DEFAULT_ALLOCATION * sizeof(void *));
    wipStack->size = 0;
    wipStack->capacity = MBCL_STACK_DEFAULT_ALLOCATION;
    wipStack->freeData = freeData;
    wipStack->compareData = compareData;
    return wipStack;
}

void stack_free(Stack *stack)
{
    while (stack->size > 0)
    {
        stack->freeData(stack_pop(stack));
    }
    free(stack->data);
    free(stack);
}

void stack_push(Stack *stack, void *data)
{
    if (stack->size >= stack->capacity)
    {
        void **newData = malloc((stack->capacity * MBCL_STACK_SCALE_FACTOR) * sizeof(void *));
        memcpy(newData, stack->data, stack->capacity * sizeof(void *));
        free(stack->data);
        stack->data = newData;
        stack->capacity = (stack->capacity * MBCL_STACK_SCALE_FACTOR);
    }

    stack->data[stack->size++] = data;
}

void *stack_pop(Stack *stack)
{
    void *poppedData = NULL;
    MBCL_ASSERT(stack->size > 0, "Pop from empty stack");
    poppedData = stack->data[--stack->size];
    return poppedData;
}

void *stack_peek(Stack *stack)
{
    void *peekedData = NULL;
    MBCL_ASSERT(stack->size > 0, "Peek from empty stack");
    peekedData = stack->data[stack->size - 1];
    return peekedData;
}

void *stack_find(Stack *stack, void *element)
{
    MBCL_ASSERT(stack->compareData != NULL, "list_find with no comparedata function set");

    for (size_t elementIndex = 0; elementIndex < stack->size; elementIndex++)
    {
        if (stack->compareData(stack->data[elementIndex], element) == 0)
        {
            return stack->data[elementIndex];
        }
    }

    return NULL;
}

Iterator *stack_begin(Stack *stack)
{
    Iterator *stackIterator = iterator_new(stack->size);
    memcpy(&stackIterator->data, &stack->data, stack->size * sizeof(void *));
    return stackIterator;
}

Iterator *stack_end(Stack *stack)
{
    Iterator *stackIterator = iterator_new(stack->size);
    memcpy(&stackIterator->data, &stack->data, stack->size * sizeof(void *));
    stackIterator->pos = stack->size - 1;
    return stackIterator;
}
