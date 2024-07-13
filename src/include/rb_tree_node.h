#include "mbcl.h"

typedef enum RB_TREE_NODE_COLOR {
    C_RED,
    C_BLACK,
} RB_TREE_NODE_COLOR;

typedef struct RBTreeNode
{
    void *data;
    struct RBTreeNode *parent;
    struct RBTreeNode *left;
    struct RBTreeNode *right;
    RB_TREE_NODE_COLOR color; 
} RBTreeNode;

RBTreeNode *rb_tree_node_new(void *data);

void rb_tree_node_free(RBTreeNode *node, MBCL_DATA_FREE_FUNCTION freeData);

void rb_tree_node_transplant(RBTreeNode **root, RBTreeNode *toReplace, RBTreeNode *replacement);

RBTreeNode *rb_tree_node_min(RBTreeNode *node);
