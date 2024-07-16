#include "hash_table.h"
#include "mbcl_test.h"
#include <stdlib.h>

int *int_new(int i)
{
    int *ip = malloc(sizeof(int *));
    *ip = i;
    return ip;
}

size_t hash_int(void *data)
{
    ssize_t hash = 0x1234;
    for (size_t i = 0; i < sizeof(int); i++)
    {
        hash <<= 1;
        hash += ((unsigned char *)(data))[i];
    }
    return hash;
}

ssize_t dummy_compare(void *a, void *b)
{
    return *(int *)a - *(int *)b;
}

// Unit test function for hash_table_insert and hash_table_find
int test_hash_table_insert_and_find()
{
    HashTable *table = hash_table_new(free, dummy_compare, hash_int, 10);
    hash_table_insert(table, int_new(1));

    int key = 1;
    int *found = (int *)hash_table_find(table, &key);
    MBCL_TEST_CHECK_OR_FAIL(found != NULL, "Couldn't find expected value in hash table");
    MBCL_TEST_CHECK_OR_FAIL(*found == 1, "Hash table find returned unexpected value");

    hash_table_free(table);
    return 0;
}

// Unit test function for hash_table_remove
int test_hash_table_remove()
{
    HashTable *table = hash_table_new(free, dummy_compare, hash_int, 10);
    hash_table_insert(table, int_new(1));

    int key = 1;
    int *found = (int *)hash_table_find(table, &key);
    MBCL_TEST_CHECK_OR_FAIL(found != NULL, "couldn't find expected value in hash table");
    MBCL_TEST_CHECK_OR_FAIL(*found == 1, "found value is incorrect");

    hash_table_remove(table, &key);
    found = (int *)hash_table_find(table, &key);
    MBCL_TEST_CHECK_OR_FAIL(found == NULL, "found unexpected value in hash table");

    hash_table_free(table);

    return 0;
}

int main(int argc, char *argv[])
{
    MBCL_ASSERT(argc == 2, "Provide number of deque test to run");
    int whichTest = atoi(argv[1]);
    switch (whichTest)
    {
    case 0:
        test_hash_table_insert_and_find();
        break;

    case 1:
        test_hash_table_remove();
        break;

    default:
        MBCL_ASSERT(0, "Invalid case number");
    }
}