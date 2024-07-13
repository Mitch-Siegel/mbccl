#include "stack.h"

#include <string.h>
#include <stdlib.h>

Stack *stack_new(MBCL_DATA_FREE_FUNCTION freeData)
{
    Stack *wipStack = malloc(sizeof(Stack));
    memset(wipStack, 0, sizeof(Stack));
    array_init(&wipStack->container, freeData, MBCL_STACK_DEFAULT_ALLOCATION);
    return wipStack;
}

void stack_free(Stack *stack)
{
    array_deinit(&stack->container);
    free(stack);
}

void stack_push(Stack *stack, void *data)
{
    if (stack->size == stack->container.size)
    {
        array_resize(&stack->container, stack->container.size * MBCL_STACK_SCALE_UP_FACTOR);
    }

    array_emplace(&stack->container, stack->size, data);
    stack->size++;
}

void *stack_pop(Stack *stack)
{
    void *poppedData = NULL;
    MBCL_ASSERT(stack->size > 0, "Pop from empty stack");
    stack->size -= 1;
    poppedData = array_at(&stack->container, stack->size);

    if(stack->size <= (stack->container.size * MBCL_STACK_SCALE_DOWN_THRESHOLD))
    {
        array_resize(&stack->container, stack->size);
    }

    return poppedData;
}

void *stack_peek(Stack *stack)
{
    void *peekedData = NULL;
    MBCL_ASSERT(stack->size > 0, "Peek from empty stack");
    peekedData = array_at(&stack->container, stack->size - 1);
    return peekedData;
}

Iterator *stack_bottom(Stack *stack)
{
    Iterator *stackIterator = iterator_new(stack->size);
    memcpy(&stackIterator->data, &stack->container.data, stack->size * sizeof(void *));
    return stackIterator;
}

Iterator *stack_top(Stack *stack)
{
    Iterator *stackIterator = iterator_new(stack->size);
    memcpy(&stackIterator->data, &stack->container.data, stack->size * sizeof(void *));
    stackIterator->pos = stack->size - 1;
    return stackIterator;
}
