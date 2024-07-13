#include "rb_tree.h"
#include <stdlib.h>
#include "stdio.h"
#include <time.h>

#include "mbcl_test.h"

ssize_t intpcmp(void *dataA, void *dataB)
{
    int intA = *(int *)dataA;
    int intB = *(int *)dataB;
    return intA - intB;
}

RBTree *create_int_tree()
{
    return rb_tree_new(free, intpcmp);
}

void insert_int(RBTree *tree, int toInsert)
{
    int *stored = malloc(sizeof(int));
    *stored = toInsert;
    rb_tree_insert(tree, stored);
}

size_t depth_traverse(RBTreeNode *node, size_t depth)
{
    size_t newDepth = depth;
    if (node->left != NULL)
    {
        newDepth = MAX(newDepth, depth_traverse(node->left, depth + 1));
    }

    if (node->right != NULL)
    {
        newDepth = MAX(newDepth, depth_traverse(node->right, depth + 1));
    }

    return newDepth;
}

size_t get_height(RBTree *tree)
{
    return depth_traverse(tree->root, 1);
}

int check_ordering(RBTree *tree)
{
    // Validation
    int last = -1;
    Iterator *treeIterator = rb_tree_begin(tree);
    while (iterator_valid(treeIterator))
    {
        int current = *(int *)iterator_get(treeIterator);
        MBCL_TEST_CHECK_OR_FAIL(current > last);
        printf("%d ", current);
        iterator_next(treeIterator);
        last = current;
    }
    printf("\n");

    iterator_free(treeIterator);

    return 0;
}

int test_ascending_insertion()
{
    RBTree *testTree = create_int_tree();
    for (int i = 0; i <= 10; i++)
    {
        insert_int(testTree, i);
    }

    MBCL_TEST_CHECK_OR_FAIL(!check_ordering(testTree));

    MBCL_TEST_CHECK_OR_FAIL(get_height(testTree) == 5);

    rb_tree_free(testTree);

    return 0;
}

int test_descending_insertion()
{
    RBTree *testTree = create_int_tree();
    for (int i = 10; i >= 0; i--)
    {
        insert_int(testTree, i);
    }

    MBCL_TEST_CHECK_OR_FAIL(!check_ordering(testTree));

    MBCL_ASSERT(get_height(testTree) == 5, "inserting numbers 0-10 tree depth");

    rb_tree_free(testTree);

    return 0;
}

int test_tree_depth()
{
    RBTree *testTree = create_int_tree();
    insert_int(testTree, 10);
    insert_int(testTree, 85);
    insert_int(testTree, 15);
    insert_int(testTree, 70);
    insert_int(testTree, 20);
    insert_int(testTree, 60);
    insert_int(testTree, 30);
    insert_int(testTree, 50);
    insert_int(testTree, 65);
    insert_int(testTree, 80);
    insert_int(testTree, 90);

    MBCL_TEST_CHECK_OR_FAIL(!check_ordering(testTree));

    MBCL_TEST_CHECK_OR_FAIL(get_height(testTree) == 4);

    rb_tree_free(testTree);

    return 0;
}

int main(int argc, char *argv[])
{
    MBCL_ASSERT(argc == 2, "Provide number of rb tree test to run");
    int whichTest = atoi(argv[1]);
    switch (whichTest)
    {
    case 0:
        return test_ascending_insertion();
        break;

    case 1:
        return test_descending_insertion();
        break;

    case 2:
        return test_tree_depth();
        break;

    default:
        MBCL_ASSERT(0, "Invalid case number");
    }
}