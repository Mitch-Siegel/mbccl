#ifndef MBCL_RB_TREE_H
#define MBCL_RB_TREE_H

#include "rb_tree_node.h"
#include "mbcl.h"

typedef struct
{
    MBCL_CONTAINER_BASE;
    struct RBTreeNode *root;
} RBTree;

void rb_tree_init(RBTree *tree, MBCL_DATA_FREE_FUNCTION freeData, MBCL_DATA_COMPARE_FUNCTION compareData);

RBTree *rb_tree_new(MBCL_DATA_FREE_FUNCTION freeData, MBCL_DATA_COMPARE_FUNCTION compareData);

void rb_tree_deinit(RBTree *tree);

void rb_tree_free(RBTree *tree);

void rb_tree_insert(RBTree *tree, void *data);

void *rb_tree_find(RBTree *tree, void *data);

void rb_tree_remove(RBTree *tree, void *data);

Iterator *rb_tree_begin(RBTree *tree);

#endif
