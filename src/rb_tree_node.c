#include "rb_tree_node.h"
#include "mbcl.h"

#include <string.h>
#include <stdlib.h>

RBTreeNode *rb_tree_node_new(void *data)
{
    RBTreeNode *wipNode = malloc(sizeof(RBTreeNode));
    memset(wipNode, 0, sizeof(RBTreeNode));
    wipNode->data = data;
    wipNode->left = NULL;
    wipNode->right = NULL;

    return wipNode;
}

void rb_tree_node_free(RBTreeNode *node, MBCL_DATA_FREE_FUNCTION freeData)
{
    if(freeData)
    {
        freeData(node->data);
    }
    free(node);
}
