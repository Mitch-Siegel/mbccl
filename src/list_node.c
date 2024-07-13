#include "list_node.h"
#include <stdlib.h>
#include <string.h>

ListNode *list_node_new(void *data)
{
    ListNode *wipNode = malloc(sizeof(ListNode));
    memset(wipNode, 0, sizeof(ListNode));
    wipNode->data = data;

    return wipNode;
}
