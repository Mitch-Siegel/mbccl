#include "hash_table.h"
#include "mbcl_test.h"
#include <stdlib.h>
#include <string.h>

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

ssize_t compare_int(void *a, void *b)
{
    return *(int *)a - *(int *)b;
}

// Unit test function for hash_table_insert and hash_table_find
int test_hash_table_insert_and_find()
{
    HashTable *table = hash_table_new(free, free, compare_int, hash_int, 10);
    hash_table_insert(table, int_new(1), strdup("one"));

    int key = 1;
    char *found = (char *)hash_table_find(table, &key);
    MBCL_TEST_CHECK_OR_FAIL(found != NULL, "Couldn't find expected value in hash table");
    printf("found: %s\n", found);
    MBCL_TEST_CHECK_OR_FAIL(strcmp(found, "one") == 0, "Hash table find returned unexpected value");

    hash_table_free(table);
    return 0;
}

// Unit test function for hash_table_remove
int test_hash_table_remove()
{
    HashTable *table = hash_table_new(free, free, compare_int, hash_int, 10);
    hash_table_insert(table, int_new(1), strdup("one"));

    int key = 1;
    char *found = (char *)hash_table_find(table, &key);
    MBCL_TEST_CHECK_OR_FAIL(found != NULL, "couldn't find expected value in hash table");
    MBCL_TEST_CHECK_OR_FAIL(strcmp(found, "one") == 0, "found value is incorrect");

    hash_table_remove(table, &key);
    found = (char *)hash_table_find(table, &key);
    MBCL_TEST_CHECK_OR_FAIL(found == NULL, "found unexpected value in hash table");

    hash_table_free(table);

    return 0;
}

int test_hash_table_iterator()
{
    HashTable *table = hash_table_new(free, free, compare_int, hash_int, 10);
    for (int i = 5; i < 1000; i += 5)
    {
        hash_table_insert(table, int_new(1), strdup("one"));
    }

    Iterator *tableIterator = hash_table_begin(table);
    size_t nElements = 0;
    // int last = 0;
    while (iterator_valid(tableIterator))
    {
        HashTableEntry *gotten = iterator_get(tableIterator);
        printf("%d:%s\n", *(int *)gotten->key, (char *)gotten->value);
        // MBCL_TEST_CHECK_OR_FAIL((last + 5) == (*gotten), "Unexpected value gotten from hash table iterator");
        nElements++;
        // last = *gotten;

        iterator_next(tableIterator);
    }

    MBCL_TEST_CHECK_OR_FAIL(nElements == table->size, "Number of elements gotten from iterator does not match hash table size");

    hash_table_free(table);

    return 0;
}

int main(int argc, char *argv[])
{
    MBCL_ASSERT(argc == 2, "Provide number of hash table test to run");
    int whichTest = atoi(argv[1]);
    switch (whichTest)
    {
    case 0:
        test_hash_table_insert_and_find();
        break;

    case 1:
        test_hash_table_remove();
        break;

    case 2:
        test_hash_table_iterator();
        break;

    default:
        MBCL_ASSERT(0, "Invalid case number");
    }
}