#include "list.h"
#include <stdlib.h>
#include <string.h>

List *list_new(MBCL_DATA_FREE_FUNCTION freeData,
               MBCL_DATA_COMPARE_FUNCTION compareData)
{
    List *wipList = malloc(sizeof(List));
    memset(wipList, 0, sizeof(List));
    wipList->freeData = freeData;
    wipList->compareData = compareData;
    return wipList;
}

void list_free(List *list)
{
    ListNode *runner = list->head;
    while (runner != NULL)
    {
        if (list->freeData != NULL)
        {
            list->freeData(runner->data);
        }
        struct ListNode *old = runner;
        runner = runner->next;
        free(old);
    }
    free(list);
}

void list_append(List *list, void *data)
{
    ListNode *newNode = list_node_new(data);
    if (list->size == 0)
    {
        newNode->next = NULL;
        newNode->prev = NULL;
        list->head = newNode;
        list->tail = newNode;
    }
    else
    {
        list->tail->next = newNode;
        newNode->prev = list->tail;
        newNode->next = NULL;
        list->tail = newNode;
    }
    list->size++;
}

void list_prepend(List *list, void *data)
{
    ListNode *newNode = list_node_new(data);
    if (list->size == 0)
    {
        newNode->next = NULL;
        newNode->prev = NULL;
        list->head = newNode;
        list->tail = newNode;
    }
    else
    {
        list->head->prev = newNode;
        newNode->next = list->head;
        list->head = newNode;
    }
    list->size++;
}

void *list_pop_front(List *list)
{
    MBCL_ASSERT(list->size > 0, "list_pop_front from empty list");

    ListNode *popped = list->head;

    list->head = list->head->next;
    if (list->head != NULL)
    {
        list->head->prev = NULL;
    }
    else
    {
        list->tail = NULL;
    }
    list->size--;

    void *poppedData = popped->data;
    free(popped);

    return poppedData;
}

void *list_pop_back(List *list)
{
    MBCL_ASSERT(list->size > 0, "list_pop_back from empty list");

    ListNode *popped = list->tail;
    list->size--;

    if (list->size)
    {
        list->tail = list->tail->prev;
        list->tail->next = NULL;
    }
    else
    {
        list->tail = NULL;
        list->head = NULL;
    }

    void *poppedData = popped->data;
    free(popped);

    return poppedData;
}

void *list_find(List *list, void *element)
{
    MBCL_ASSERT(list->compareData != NULL, "list_find with no comparedata function set");

    for(ListNode *listRunner = list->head; listRunner != NULL; listRunner = listRunner->next)
    {
        void *runnerData = listRunner->data;
        if(list->compareData(runnerData, element) == 0)
        {
            return runnerData;
        }
    }

    return NULL;
}

Iterator *list_begin(List *list)
{
    Iterator *listIterator = iterator_new(list->size);
    size_t elementIndex = 0;
    for (ListNode *runner = list->head; runner != NULL; runner = runner->next)
    {
        listIterator->data[elementIndex++] = runner->data;
    }

    return listIterator;
}

Iterator *list_end(List *list)
{
    Iterator *listIterator = iterator_new(list->size);
    size_t elementIndex = 0;
    for (ListNode *runner = list->head; runner != NULL; runner = runner->next)
    {
        listIterator->data[elementIndex++] = runner->data;
    }

    listIterator->pos = listIterator->nElements - 1;
    return listIterator;
}
