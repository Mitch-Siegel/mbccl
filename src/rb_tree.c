#include "rb_tree.h"
#include <stdlib.h>
#include <string.h>

static void rb_tree_rotate_left(RBTreeNode **root, RBTreeNode *pivot)
{
    MBCL_ASSERT(pivot != NULL, "null pivot in rb_tree_rotate_left");

    if (pivot->right == NULL)
    {
        // nothing to do
        return;
    }

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

static void rb_tree_rotate_right(RBTreeNode **root, RBTreeNode *pivot)
{
    MBCL_ASSERT(pivot != NULL, "null pivot in rb_tree_rotate_right");

    if (pivot->left == NULL)
    {
        // nothing to do
        return;
    }

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

static void rb_tree_insert_fixup(RBTree *tree, RBTreeNode *inserted)
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

void rb_tree_init(RBTree *tree, MBCL_DATA_FREE_FUNCTION freeData, MBCL_DATA_COMPARE_FUNCTION compareData)
{
    tree->root = NULL;
    tree->freeData = freeData;
    tree->compareData = compareData;
}

RBTree *rb_tree_new(MBCL_DATA_FREE_FUNCTION freeData, MBCL_DATA_COMPARE_FUNCTION compareData)
{
    RBTree *wipTree = malloc(sizeof(RBTree));
    memset(wipTree, 0, sizeof(RBTree));

    rb_tree_init(wipTree, freeData, compareData);

    return wipTree;
}

static void rb_tree_free_nodes(RBTreeNode *root, MBCL_DATA_FREE_FUNCTION freeData)
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

void rb_tree_deinit(RBTree *tree)
{
    if (tree->root != NULL)
    {
        rb_tree_free_nodes(tree->root, tree->freeData);
    }
}

void rb_tree_free(RBTree *tree)
{
    rb_tree_deinit(tree);
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
#include <stdio.h>
static RBTreeNode *rb_tree_find_node(RBTree *tree, void *data)
{
    // printf("find node with data %p\n", data);
    RBTreeNode *currentNode = tree->root;
    while (currentNode != NULL)
    {
        ssize_t cmpVal = tree->compareData(currentNode->data, data);
        // printf("current has data %p - compared to %zd\n", currentNode->data, cmpVal);
        if (cmpVal == 0)
        {
            // printf("return that\n");
            return currentNode;
        }
        else if (cmpVal > 0)
        {
            // printf("go left\n");
            currentNode = currentNode->left;
        }
        else
        {
            // printf("go right\n");
            currentNode = currentNode->right;
        }
    }

    return NULL;
}

void *rb_tree_find(RBTree *tree, void *data)
{
    RBTreeNode *foundNode = rb_tree_find_node(tree, data);

    void *retData = NULL;
    if (foundNode != NULL)
    {
        retData = foundNode->data;
    }
    return retData;
}

static void rb_tree_remove_fixup(RBTreeNode **root, RBTreeNode *currentNode)
{
    RBTreeNode *sibling;
    while ((currentNode != NULL) &&
           (currentNode != *root) &&
           (currentNode->color == C_BLACK))
    {
        if ((currentNode->parent != NULL) && (currentNode == currentNode->parent->left))
        {
            sibling = currentNode->parent->right;
            if ((sibling != NULL) && (sibling->color == C_RED))
            {
                sibling->color = C_BLACK;
                currentNode->parent->color = C_RED;
                rb_tree_rotate_left(root, currentNode->parent);
                sibling = currentNode->parent->right;
            }
            if ((sibling != NULL) &&
                (((sibling->left == NULL) || (sibling->left->color == C_BLACK)) &&
                 ((sibling->right == NULL) || (sibling->right->color == C_BLACK))))
            {
                sibling->color = C_RED;
                currentNode = currentNode->parent;
            }
            else
            {
                if ((sibling != NULL) &&
                    (sibling->right != NULL) &&
                    (sibling->right->color == C_BLACK))
                {
                    if (sibling->left != NULL)
                    {
                        sibling->left->color = C_BLACK;
                    }
                    sibling->color = C_RED;
                    rb_tree_rotate_right(root, sibling);
                    sibling = currentNode->parent->right;
                }
                if (sibling != NULL)
                {
                    sibling->color = currentNode->parent->color;
                }
                if (currentNode->parent != NULL)
                {
                    currentNode->parent->color = C_BLACK;
                }
                if (sibling != NULL && sibling->right != NULL)
                {
                    sibling->right->color = C_BLACK;
                }
                rb_tree_rotate_left(root, currentNode->parent);
                currentNode = *root;
            }
        }
        else if (currentNode->parent != NULL)
        {
            sibling = currentNode->parent->left;
            if ((sibling != NULL) && (sibling->color == C_RED))
            {
                sibling->color = C_BLACK;
                currentNode->parent->color = C_RED;
                rb_tree_rotate_right(root, currentNode->parent);
                sibling = currentNode->parent->left;
            }
            if (sibling != NULL &&
                (((sibling->right == NULL) || (sibling->right->color == C_BLACK)) &&
                 ((sibling->left == NULL) || (sibling->left->color == C_BLACK))))
            {
                sibling->color = C_RED;
                currentNode = currentNode->parent;
            }
            else
            {
                if ((sibling != NULL) &&
                    (sibling->left != NULL) &&
                    (sibling->left->color == C_BLACK))
                {
                    if (sibling->right != NULL)
                    {
                        sibling->right->color = C_BLACK;
                    }
                    sibling->color = C_RED;
                    rb_tree_rotate_left(root, sibling);
                    sibling = currentNode->parent->left;
                }
                if (sibling != NULL)
                {
                    sibling->color = currentNode->parent->color;
                }
                if (currentNode->parent != NULL)
                {
                    currentNode->parent->color = C_BLACK;
                }
                if ((sibling != NULL) && (sibling->left != NULL))
                {
                    sibling->left->color = C_BLACK;
                }
                rb_tree_rotate_right(root, currentNode->parent);
                currentNode = *root;
            }
        }
    }

    if (currentNode != NULL)
    {
        currentNode->color = C_BLACK;
    }
}

// Function to delete a node from the red-black tree
void rb_tree_remove(RBTree *tree, void *data)
{
    RBTreeNode *toRemove = rb_tree_find_node(tree, data);

    MBCL_ASSERT(toRemove != NULL, "rb_tree_remove to remove data not in tree");

    RBTreeNode *replacement = NULL;
    RBTreeNode *current = toRemove;
    RB_TREE_NODE_COLOR originalColor = current->color;

    if (toRemove->left == NULL)
    {
        replacement = toRemove->right;
        rb_tree_node_transplant(&tree->root, toRemove, toRemove->right);
    }
    else if (toRemove->right == NULL)
    {
        replacement = toRemove->left;
        rb_tree_node_transplant(&tree->root, toRemove, toRemove->left);
    }
    else
    {
        current = rb_tree_node_min(toRemove->right);
        originalColor = current->color;
        replacement = current->right;
        if (current->parent == toRemove)
        {
            if (replacement != NULL)
            {
                replacement->parent = current;
            }
        }
        else
        {
            rb_tree_node_transplant(&tree->root, current, current->right);
            current->right = toRemove->right;
            if (current->right != NULL)
            {
                current->right->parent = current;
            }
        }
        rb_tree_node_transplant(&tree->root, toRemove, current);
        current->left = toRemove->left;
        if (current->left != NULL)
        {
            current->left->parent = current;
        }
        current->color = toRemove->color;
    }

    if (originalColor == C_BLACK)
    {
        rb_tree_remove_fixup(&tree->root, replacement);
    }

    rb_tree_node_free(toRemove, tree->freeData);

    tree->size--;
}

static void rb_tree_inorder_traverse_to_iterator(RBTreeNode *node, Iterator *treeIterator, size_t *elementIndex)
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
    if (tree->size > 0)
    {
        rb_tree_inorder_traverse_to_iterator(tree->root, treeIterator, &elementIndex);
    }

    return treeIterator;
}

// void rb_tree_print_rec(RBTreeNode *node, size_t iteration)
// {
//     printf("%zu[label=%d]\n", (size_t)node + iteration, *(int *)node->data);
//     if (node->left != NULL)
//     {
//         printf("%zu->%zu\n", (size_t)node + iteration, (size_t)node->left + iteration);
//         rb_tree_print_rec(node->left, iteration);
//     }

//     if (node->right != NULL)
//     {
//         printf("%zu->%zu\n", (size_t)node + iteration, (size_t)node->right + iteration);
//         rb_tree_print_rec(node->right, iteration);
//     }
// }

// void rb_tree_print(RBTree *tree)
// {
//     static size_t iteration = 0;

//     printf("subgraph %zu {\n", iteration);
//     rb_tree_print_rec(tree->root, iteration);
//     printf("}\n");

//     iteration++;
// }
