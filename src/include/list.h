#ifndef MBCL_LIST_H
#define MBCL_LIST_H

#include "list_node.h"
#include "iterator.h"
#include "mbcl.h"

typedef struct
{
    size_t size;
    ListNode *head;
    ListNode *tail;
    MBCL_DATA_FREE_FUNCTION freeData;
    MBCL_DATA_COMPARE_FUNCTION compareData;
} List;

List *list_new(MBCL_DATA_FREE_FUNCTION freeData,
               MBCL_DATA_COMPARE_FUNCTION compareData);

void list_free(List *list);

void list_append(List *list, void *data);

void list_prepend(List *list, void *data);

void *list_pop_front(List *list);

void *list_pop_back(List *list);

void *list_find(List *list, void *element);

Iterator *list_begin(List *list);

Iterator *list_end(List *list);

#endif
