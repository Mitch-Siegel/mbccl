#include "list.h"
#include <stdlib.h>
#include <string.h>

void list_init(List *list,
               MBCL_DATA_FREE_FUNCTION freeData,
               MBCL_DATA_COMPARE_FUNCTION compareData)
{
    memset(list, 0, sizeof(List));
    list->freeData = freeData;
    list->compareData = compareData;
}

List *list_new(MBCL_DATA_FREE_FUNCTION freeData,
               MBCL_DATA_COMPARE_FUNCTION compareData)
{
    List *wipList = malloc(sizeof(List));
    list_init(wipList, freeData, compareData);
    return wipList;
}

void list_deinit(List *list)
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
}

void list_free(List *list)
{
    list_deinit(list);
    free(list);
}

void list_append_node(List *list, ListNode *node)
{
    if (list->size == 0)
    {
        node->next = NULL;
        node->prev = NULL;
        list->head = node;
        list->tail = node;
    }
    else
    {
        list->tail->next = node;
        node->prev = list->tail;
        node->next = NULL;
        list->tail = node;
    }
    list->size++;
}

void list_append(List *list, void *data)
{
    ListNode *newNode = list_node_new(data);
    list_append_node(list, newNode);
}

void list_prepend_node(List *list, ListNode *node)
{
    if (list->size == 0)
    {
        node->next = NULL;
        node->prev = NULL;
        list->head = node;
        list->tail = node;
    }
    else
    {
        list->head->prev = node;
        node->next = list->head;
        list->head = node;
    }
    list->size++;
}

void list_prepend(List *list, void *data)
{
    ListNode *newNode = list_node_new(data);
    list_prepend_node(list, newNode);
}

void *list_front(List *list)
{
    MBCL_ASSERT(list->size > 0, "list_front from empty list");
    return list->head->data;
}

ListNode *list_pop_front_node(List *list)
{
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

    popped->prev = NULL;
    popped->next = NULL;

    return popped;
}

void *list_pop_front(List *list)
{
    MBCL_ASSERT(list->size > 0, "list_pop_front from empty list");

    ListNode *poppedNode = list_pop_front_node(list);
    void *poppedData = poppedNode->data;
    free(poppedNode);

    return poppedData;
}

void *list_back(List *list)
{
    MBCL_ASSERT(list->size > 0, "list_back from empty list");
    return list->tail->data;
}

static ListNode *list_pop_back_node(List *list)
{
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

    popped->prev = NULL;
    popped->next = NULL;

    return popped;
}

void *list_pop_back(List *list)
{
    MBCL_ASSERT(list->size > 0, "list_pop_back from empty list");

    ListNode *poppedNode = list_pop_back_node(list);
    void *poppedData = poppedNode->data;
    free(poppedNode);

    return poppedData;
}

void list_merge_sort(List *list)
{
    if (list->size > 2)
    {
        List firstList;
        list_init(&firstList, NULL, list->compareData);
        List secondList;
        list_init(&secondList, NULL, list->compareData);
        size_t half = list->size / 2;
        while (list->size > half)
        {
            list_append_node(&firstList, list_pop_back_node(list));
        }
        while (list->size > 0)
        {
            list_append_node(&secondList, list_pop_back_node(list));
        }

        list_merge_sort(&firstList);
        list_merge_sort(&secondList);

        while ((firstList.size > 0) && (secondList.size > 0))
        {
            void *data1 = list_front(&firstList);
            void *data2 = list_front(&secondList);
            if (list->compareData(data1, data2) > 0)
            {
                list_append_node(list, list_pop_front_node(&secondList));
            }
            else
            {
                list_append_node(list, list_pop_front_node(&firstList));
            }
        }

        while (firstList.size > 0)
        {
            list_append_node(list, list_pop_front_node(&firstList));
        }

        while (secondList.size > 0)
        {
            list_append_node(list, list_pop_front_node(&secondList));
        }
    }
    else
    {
        if (list->size == 2)
        {
            // compare the 2 elements, in-place swap if necessary
            if (list->compareData(list->head->data, list->tail->data) > 0)
            {
                void *temp = list->head->data;
                list->head->data = list->tail->data;
                list->tail->data = temp;
            }
        }
    }
}

void list_sort(List *list)
{
    list_merge_sort(list);
}

void *list_find(List *list, void *element)
{
    MBCL_ASSERT(list->compareData != NULL, "list_find with no comparedata function set");

    for (ListNode *listRunner = list->head; listRunner != NULL; listRunner = listRunner->next)
    {
        void *runnerData = listRunner->data;
        if (list->compareData(runnerData, element) == 0)
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
