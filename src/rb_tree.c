#include "rb_tree.h"
#include <stdlib.h>
#include <string.h>

void rb_tree_rotate_left(RBTreeNode **root, RBTreeNode *pivot)
{
    MBCL_ASSERT(pivot != NULL, "null pivot in rb_tree_rotate_left");

    RBTreeNode *rightChild = pivot->right;
    pivot->right = rightChild->left;

    if (rightChild->left != NULL)
    {
        rightChild->left->parent = pivot;
    }

    rightChild->parent = pivot->parent;
    if (pivot->parent == NULL)
    {
        *root = rightChild;
    }
    else if (pivot == pivot->parent->left)
    {
        pivot->parent->left = rightChild;
    }
    else
    {
        pivot->parent->right = rightChild;
    }

    rightChild->left = pivot;
    pivot->parent = rightChild;
}

void rb_tree_rotate_right(RBTreeNode **root, RBTreeNode *pivot)
{
    MBCL_ASSERT(pivot != NULL, "null pivot in rb_tree_rotate_right");

    RBTreeNode *leftChild = pivot->left;
    pivot->left = leftChild->right;

    if (leftChild->right != NULL)
    {
        leftChild->right->parent = pivot;
    }

    leftChild->parent = pivot->parent;
    if (pivot->parent == NULL)
    {
        *root = leftChild;
    }
    else if (pivot == pivot->parent->left)
    {
        pivot->parent->left = leftChild;
    }
    else
    {
        pivot->parent->right = leftChild;
    }

    leftChild->right = pivot;
    pivot->parent = leftChild;
}

void rb_tree_insert_fixup(RBTree *tree, RBTreeNode *inserted)
{
    while ((inserted->parent != NULL) && (inserted->parent->color == C_RED))
    {
        if (inserted->parent == inserted->parent->parent->left)
        {
            RBTreeNode *uncle = inserted->parent->parent->right;
            if ((uncle != NULL) && (uncle->color == C_RED))
            {
                inserted->parent->color = C_BLACK;
                uncle->color = C_BLACK;
                inserted->parent->parent->color = C_RED;
                inserted = inserted->parent->parent;
            }
            else
            {
                if (inserted == inserted->parent->right)
                {
                    inserted = inserted->parent;
                    rb_tree_rotate_left(&tree->root, inserted);
                }
                inserted->parent->color = C_BLACK;
                inserted->parent->parent->color = C_RED;
                rb_tree_rotate_right(&tree->root, inserted->parent->parent);
            }
        }
        else
        {
            RBTreeNode *uncle = inserted->parent->parent->left;
            if ((uncle != NULL) && (uncle->color == C_RED))
            {
                inserted->parent->color = C_BLACK;
                uncle->color = C_BLACK;
                inserted->parent->parent->color = C_RED;
                inserted = inserted->parent->parent;
            }
            else
            {
                if (inserted == inserted->parent->left)
                {
                    inserted = inserted->parent;
                    rb_tree_rotate_right(&tree->root, inserted);
                }
                inserted->parent->color = C_BLACK;
                inserted->parent->parent->color = C_RED;
                rb_tree_rotate_left(&tree->root, inserted->parent->parent);
            }
        }
    }
    tree->root->color = C_BLACK;
}

RBTree *rb_tree_new(MBCL_DATA_FREE_FUNCTION freeData, MBCL_DATA_COMPARE_FUNCTION compareData)
{
    RBTree *wipTree = malloc(sizeof(RBTree));
    memset(wipTree, 0, sizeof(RBTree));

    wipTree->root = NULL;
    wipTree->freeData = freeData;
    wipTree->compareData = compareData;

    return wipTree;
}

void rb_tree_free_nodes(RBTreeNode *root, MBCL_DATA_FREE_FUNCTION freeData)
{
    if (root->left != NULL)
    {
        rb_tree_free_nodes(root->left, freeData);
    }

    if (root->right != NULL)
    {
        rb_tree_free_nodes(root->right, freeData);
    }

    rb_tree_node_free(root, freeData);
}

void rb_tree_free(RBTree *tree)
{
    rb_tree_free_nodes(tree->root, tree->freeData);
    free(tree);
}

void rb_tree_insert(RBTree *tree, void *data)
{
    RBTreeNode *newNode = rb_tree_node_new(data);
    RBTreeNode *nodeParent = NULL;
    RBTreeNode *currentNode = tree->root;

    while (currentNode != NULL)
    {
        nodeParent = currentNode;
        ssize_t cmpVal = tree->compareData(currentNode->data, newNode->data);
        MBCL_ASSERT(cmpVal != 0, "Duplicate insertion to rb tree");
        if (cmpVal > 0)
        {
            currentNode = currentNode->left;
        }
        else
        {
            currentNode = currentNode->right;
        }
    }

    newNode->parent = nodeParent;

    if (nodeParent == NULL)
    {
        tree->root = newNode;
    }
    else if (tree->compareData(nodeParent->data, newNode->data) > 0)
    {
        nodeParent->left = newNode;
    }
    else
    {
        nodeParent->right = newNode;
    }

    newNode->left = NULL;
    newNode->right = NULL;
    newNode->color = C_RED;

    rb_tree_insert_fixup(tree, newNode);

    tree->size++;
}

void *rb_tree_find(RBTree *tree, void *data)
{
    RBTreeNode *currentNode = tree->root;
    while (currentNode != NULL)
    {
        ssize_t cmpVal = tree->compareData(currentNode->data, data);
        if (cmpVal == 0)
        {
            return currentNode->data;
        }
        else if (cmpVal > 0)
        {
            currentNode = currentNode->left;
        }
        else
        {
            currentNode = currentNode->right;
        }
    }

    return NULL;
}

#include <stdio.h>
void rb_tree_inorder_traverse_to_iterator(RBTreeNode *node, Iterator *treeIterator, size_t *elementIndex)
{
    if (node->left != NULL)
    {
        rb_tree_inorder_traverse_to_iterator(node->left, treeIterator, elementIndex);
    }

    treeIterator->data[*elementIndex] = node->data;
    *elementIndex += 1;

    if (node->right != NULL)
    {
        rb_tree_inorder_traverse_to_iterator(node->right, treeIterator, elementIndex);
    }
}

Iterator *rb_tree_begin(RBTree *tree)
{
    Iterator *treeIterator = iterator_new(tree->size);
    size_t elementIndex = 0;
    rb_tree_inorder_traverse_to_iterator(tree->root, treeIterator, &elementIndex);

    return treeIterator;
}

void rb_tree_print_rec(RBTreeNode *node, size_t iteration)
{
    printf("%zu[label=%d]\n", (size_t)node + iteration, *(int *)node->data);
    if (node->left != NULL)
    {
        printf("%zu->%zu\n", (size_t)node + iteration, (size_t)node->left + iteration);
        rb_tree_print_rec(node->left, iteration);
    }

    if (node->right != NULL)
    {
        printf("%zu->%zu\n", (size_t)node + iteration, (size_t)node->right + iteration);
        rb_tree_print_rec(node->right, iteration);
    }
}

void rb_tree_print(RBTree *tree)
{
    static size_t iteration = 0;

    printf("subgraph %zu {\n", iteration);
    rb_tree_print_rec(tree->root, iteration);
    printf("}\n");

    iteration++;
}
