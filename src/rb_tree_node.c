#include "rb_tree_node.h"
#include "mbcl.h"

#include <string.h>
#include <stdlib.h>

RBTreeNode *rb_tree_node_new(void *data)
{
    RBTreeNode *wipNode = malloc(sizeof(RBTreeNode));
    memset(wipNode, 0, sizeof(RBTreeNode));
    wipNode->data = data;

    return wipNode;
}

RBTreeNode *insert(RBTreeNode *root, void *data, MBCL_DATA_COMPARE_FUNCTION compareData)
{
    RBTreeNode *returned = NULL;
    if (root == NULL)
    {
        returned = rb_tree_node_new(data);
    }
    else
    {
        if (compareData(data, root->data) > 0)
        {
            root->left = insert(root->left, data, compareData);
        }
        else
        {
            root->right = insert(root->right, data, compareData);
        }
        returned = root;
    }

    return returned;
}