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
    if (freeData)
    {
        freeData(node->data);
    }
    free(node);
}

RBTreeNode *rb_tree_node_min(RBTreeNode *node)
{
    while (node->left != NULL)
    {
        node = node->left;
    }

    return node;
}

void rb_tree_node_transplant(RBTreeNode **root, RBTreeNode *toReplace, RBTreeNode *replacement)
{
    if (toReplace->parent == NULL)
    {
        *root = replacement;
    }
    else if (toReplace == toReplace->parent->left)
    {
        toReplace->parent->left = replacement;
    }
    else
    {
        toReplace->parent->right = replacement;
    }
    if (replacement != NULL)
    {
        replacement->parent = toReplace->parent;
    }
}
