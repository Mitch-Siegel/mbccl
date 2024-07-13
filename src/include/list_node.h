
typedef struct ListNode
{
    void *data;
    struct ListNode *prev;
    struct ListNode *next;
} ListNode;

ListNode *list_node_new(void *data);