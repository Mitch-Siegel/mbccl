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

void remove_int(RBTree *tree, int toRemove)
{
    rb_tree_remove(tree, &toRemove);
}

int try_find_int(RBTree *tree, int toFind)
{
    int *found = rb_tree_find(tree, &toFind);
    if (found == NULL)
    {
        return -1;
    }
    return *found;
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

int tree_check_ordering(RBTree *tree)
{
    // Validation
    int last = -1;
    Iterator *treeIterator = rb_tree_begin(tree);
    while (iterator_valid(treeIterator))
    {
        int current = *(int *)iterator_get(treeIterator);
        if (current <= last)
        {
            return 1;
        }
        printf("%d ", current);
        iterator_next(treeIterator);
        last = current;
    }
    printf("\n");

    iterator_free(treeIterator);

    return 0;
}

#define check_ordering(tree) MBCL_TEST_CHECK_OR_FAIL(!tree_check_ordering(tree), "Bad ordering in tree")

#define expect_tree_height(tree, height)                                                              \
    {                                                                                                 \
        size_t treeHeight = get_height(testTree);                                                     \
        MBCL_TEST_CHECK_OR_FAIL(treeHeight == (height), "Unexpected tree height of %zu", treeHeight); \
    }

int test_ascending_insertion()
{
    RBTree *testTree = create_int_tree();
    for (int i = 0; i <= 10; i++)
    {
        insert_int(testTree, i);
    }

    check_ordering(testTree);

    size_t treeHeight = get_height(testTree);
    MBCL_TEST_CHECK_OR_FAIL(treeHeight == 5, "Unexpected tree height of %zu", treeHeight);

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

    check_ordering(testTree);
    expect_tree_height(testTree, 5);

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

    check_ordering(testTree);
    expect_tree_height(testTree, 4);

    rb_tree_free(testTree);

    return 0;
}

int test_tree_find()
{
    RBTree *testTree = create_int_tree();
    for (int i = 0; i < 10; i++)
    {
        insert_int(testTree, i);
    }

    check_ordering(testTree);

    for (int i = 0; i < 10; i++)
    {
        MBCL_TEST_CHECK_OR_FAIL(try_find_int(testTree, i) == i, "Couldn't find %d", i);
    }

    rb_tree_free(testTree);

    return 0;
}

int test_tree_not_find()
{
    RBTree *testTree = create_int_tree();
    MBCL_TEST_CHECK_OR_FAIL(try_find_int(testTree, 5) == -1, "Found unexpected element");

    rb_tree_free(testTree);
    return 0;
}

int test_tree_remove_single()
{
    RBTree *testTree = create_int_tree();

    MBCL_TEST_CHECK_OR_FAIL(try_find_int(testTree, 123) == -1, "Found unexpected element");
    insert_int(testTree, 123);
    MBCL_TEST_CHECK_OR_FAIL(try_find_int(testTree, 123) == 123, "Couldn't find expected element");
    remove_int(testTree, 123);
    MBCL_TEST_CHECK_OR_FAIL(try_find_int(testTree, 123) == -1, "Found unexpected element");
    insert_int(testTree, 123);
    MBCL_TEST_CHECK_OR_FAIL(try_find_int(testTree, 123) == 123, "Couldn't find expected element");

    rb_tree_free(testTree);
    return 0;
}

int test_tree_remove()
{
    RBTree *testTree = create_int_tree();

    for (int i = 0; i < 10; i++)
    {
        insert_int(testTree, i);
    }

    MBCL_TEST_CHECK_OR_FAIL(try_find_int(testTree, 4) == 4, "Couldn't find expected element");
    MBCL_TEST_CHECK_OR_FAIL(try_find_int(testTree, 8) == 8, "Couldn't find expected element");
    remove_int(testTree, 4);
    MBCL_TEST_CHECK_OR_FAIL(try_find_int(testTree, 4) == -1, "Found unexpected element");
    MBCL_TEST_CHECK_OR_FAIL(try_find_int(testTree, 8) == 8, "Couldn't find expected element");
    remove_int(testTree, 8);
    MBCL_TEST_CHECK_OR_FAIL(try_find_int(testTree, 4) == -1, "Found unexpected element");
    MBCL_TEST_CHECK_OR_FAIL(try_find_int(testTree, 8) == -1, "Found unexpected element");

    check_ordering(testTree);

    rb_tree_free(testTree);

    return 0;
}

int test_tree_remove_ascending()
{
    RBTree *testTree = create_int_tree();

    for (int i = 0; i < 10; i++)
    {
        insert_int(testTree, i);
    }

    for (int i = 0; i < 10; i++)
    {
        remove_int(testTree, i);
        MBCL_TEST_CHECK_OR_FAIL(try_find_int(testTree, i) == -1, "Found unexpected element");
        check_ordering(testTree);
    }

    rb_tree_free(testTree);

    return 0;
}

int test_tree_remove_descending()
{
    RBTree *testTree = create_int_tree();

    for (int i = 0; i < 10; i++)
    {
        insert_int(testTree, i);
    }

    for (int i = 9; i >= 0; i--)
    {
        remove_int(testTree, i);
        MBCL_TEST_CHECK_OR_FAIL(try_find_int(testTree, i) == -1, "Found unexpected element");
        check_ordering(testTree);
    }

    rb_tree_free(testTree);

    return 0;
}

int test_tree_insert_million()
{
    RBTree *testTree = create_int_tree();

    for (int i = 0; i < 1000000; i++)
    {
        insert_int(testTree, i);
    }

    rb_tree_free(testTree);

    return 0;
}

int test_tree_remove_million()
{
    RBTree *testTree = create_int_tree();

    for (int i = 0; i < 1000000; i++)
    {
        insert_int(testTree, i);
    }

    for (int i = 0; i < 1000000; i++)
    {
        remove_int(testTree, i);
    }

    rb_tree_free(testTree);

    return 0;
}

int test_tree_remove_random()
{
    RBTree *testTree = create_int_tree();
    // srand(1); // causes pivot issues
    size_t now = (size_t)time(0);
    printf("seed: %zu\n", now);
    srand(now); // causes pivot issues
    for (int i = 0; i < 60000; i++)
    {
        if (((rand() % 3) == 0) && (testTree->size > 0))
        {
            Iterator *toRemove = rb_tree_begin(testTree);
            for (size_t removeSkip = 0; removeSkip < (size_t)rand() % testTree->size; removeSkip++)
            {
                iterator_next(toRemove);
            }
            int *removed = iterator_get(toRemove);
            // int removedVal = *removed;
            rb_tree_remove(testTree, removed);
            // printf("remove %d - new size %zu\n", removedVal, testTree->size);

            iterator_free(toRemove);
        }
        else
        {
            int toAdd = rand() % 100000;
            if (rb_tree_find(testTree, &toAdd) == NULL)
            {
                insert_int(testTree, toAdd);
                // printf("add %d - new size %zu\n", toAdd, testTree->size);
            }
        }
    }


    while (testTree->size > 0)
    {
        Iterator *toRemove = rb_tree_begin(testTree);
        rb_tree_remove(testTree, iterator_get(toRemove));
        iterator_free(toRemove);
    }

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

    case 3:
        return test_tree_find();
        break;

    case 4:
        return test_tree_not_find();
        break;

    case 5:
        return test_tree_remove_single();
        break;

    case 6:
        return test_tree_remove();
        break;

    case 7:
        test_tree_remove_ascending();
        break;

    case 8:
        test_tree_remove_descending();
        break;

    case 9:
        test_tree_insert_million();
        break;

    case 10:
        test_tree_remove_million();
        break;

    case 11:
        test_tree_remove_random();
        break;

    default:
        MBCL_ASSERT(0, "Invalid case number");
    }
}
