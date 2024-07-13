#ifndef MBCL_LIST_NODE_H
#define MBCL_LIST_NODE_H

typedef struct ListNode
{
    void *data;
    struct ListNode *prev;
    struct ListNode *next;
} ListNode;

ListNode *list_node_new(void *data);

#endif
