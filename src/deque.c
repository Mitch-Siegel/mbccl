#include "deque.h"

#include <string.h>
#include <stdlib.h>

Deque *deque_new(MBCL_DATA_FREE_FUNCTION freeData)
{
    Deque *wipDeque = malloc(sizeof(Deque));
    wipDeque->freeData = freeData;
    wipDeque->data = malloc((DEQUE_SHIFT_AMOUNT * 2) * sizeof(void *));
    wipDeque->cap = (DEQUE_SHIFT_AMOUNT * 2);
    wipDeque->size = 0;
    wipDeque->startIdx = DEQUE_SHIFT_AMOUNT;

    return wipDeque;
}

void deque_free(Deque *deque)
{
    free(deque->data);
    free(deque);
}
#include <stdio.h>
void deque_push_front(Deque *deque, void *data)
{
    if (deque->startIdx == 0)
    {
        deque->data = realloc(deque->data, (deque->startIdx + deque->size + DEQUE_SHIFT_AMOUNT) * sizeof(void *));
        deque->cap += DEQUE_SHIFT_AMOUNT;
        memmove(&deque->data[DEQUE_SHIFT_AMOUNT], &deque->data[deque->startIdx], deque->size * sizeof(void *));
        deque->startIdx = DEQUE_SHIFT_AMOUNT;
        printf("shift from %zu to %zu\n", 0UL, deque->startIdx);
    }
    deque->startIdx--;
    printf("put at %zu\n", deque->startIdx);
    deque->data[deque->startIdx] = data;
    deque->size++;
}

void *deque_pop_front(Deque *deque)
{
    MBCL_ASSERT(deque->size > 0, "pop_front from empty deque");
    void *poppedData = deque->data[deque->startIdx];
    deque->startIdx++;
    deque->size--;
    return poppedData;
}

void deque_push_back(Deque *deque, void *data)
{
    if ((deque->startIdx + deque->size) <= deque->cap)
    {
        deque->data = realloc(deque->data, (deque->startIdx + deque->size + DEQUE_SHIFT_AMOUNT) * sizeof(void *));
        deque->cap += DEQUE_SHIFT_AMOUNT;
    }
    deque->data[deque->startIdx + deque->size] = data;
    deque->size++;
}

void *deque_pop_back(Deque *deque)
{
    MBCL_ASSERT(deque->size > 0, "pop_rear from empty deque");
    printf("pop from %zu\n", (deque->startIdx + deque->size) - 1);
    deque->size--;
    void *poppedData = deque->data[deque->startIdx + deque->size];
    return poppedData;
}

void *deque_at(Deque *deque, size_t index)
{
    MBCL_ASSERT(index < deque->size, "Deque access out of bounds");
    return deque->data[deque->startIdx + index];
}

Iterator *deque_front(Deque *deque)
{
    Iterator *wipIterator = iterator_new(deque->size);
    memcpy(wipIterator->data, deque->data + deque->startIdx, deque->size * sizeof(void *));
    return wipIterator;
}

Iterator *deque_rear(Deque *deque)
{
    Iterator *wipIterator = iterator_new(deque->size);
    memcpy(wipIterator->data, deque->data + deque->startIdx, deque->size * sizeof(void *));
    wipIterator->pos = deque->size - 1;
    return wipIterator;
}
