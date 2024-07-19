#ifndef MBCL_DEQUE_H
#define MBCL_DEQUE_H

#include "mbcl.h"

typedef struct
{
    MBCL_CONTAINER_BASE;
    void **data;
    size_t startIdx;
    size_t cap;
} Deque;

#define DEQUE_SHIFT_AMOUNT 2

Deque *deque_new(MBCL_DATA_FREE_FUNCTION freeData);

void deque_free(Deque *deque);

void deque_push_front(Deque *deque, void *data);

void *deque_pop_front(Deque *deque);

void deque_push_back(Deque *deque, void *data);

void *deque_pop_back(Deque *deque);

void *deque_at(Deque *deque, size_t index);

Iterator *deque_front(Deque *deque);

Iterator *deque_rear(Deque *deque);

#endif
