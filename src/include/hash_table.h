#ifndef MBCL_HASH_TABLE_H
#define MBCL_HASH_TABLE_H

#include "mbcl.h"
#include "rb_tree.h"

typedef struct
{
    MBCL_CONTAINER_BASE;
    RBTree *buckets;
    size_t nBuckets;
    size_t (*hashData)(void *data);
} HashTable;

HashTable *hash_table_new(MBCL_DATA_FREE_FUNCTION freeData,
                          MBCL_DATA_COMPARE_FUNCTION compareData,
                          size_t (*hashData)(void *data),
                          size_t nBuckets);

void hash_table_free(HashTable *table);

void hash_table_insert(HashTable *table, void *data);

void *hash_table_find(HashTable *table, void *data);

void hash_table_remove(HashTable *table, void *data);

#endif