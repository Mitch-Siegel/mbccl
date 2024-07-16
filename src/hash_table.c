#include "hash_table.h"

#include <stdlib.h>
#include <string.h>

HashTable *hash_table_new(MBCL_DATA_FREE_FUNCTION freeData,
                          MBCL_DATA_COMPARE_FUNCTION compareData,
                          size_t (*hashData)(void *data),
                          size_t nBuckets)
{
    HashTable *wipTable = malloc(sizeof(HashTable));
    memset(wipTable, 0, sizeof(HashTable));
    wipTable->buckets = malloc(nBuckets * sizeof(RBTree));
    wipTable->nBuckets = nBuckets;
    for (size_t bucketInitIdx = 0; bucketInitIdx < nBuckets; bucketInitIdx++)
    {
        rb_tree_init(&wipTable->buckets[bucketInitIdx], freeData, compareData);
    }
    wipTable->hashData = hashData;

    return wipTable;
}

void hash_table_free(HashTable *table)
{
    for (size_t bucketInitIdx = 0; bucketInitIdx < table->nBuckets; bucketInitIdx++)
    {
        rb_tree_deinit(&table->buckets[bucketInitIdx]);
    }

    free(table);
}

void hash_table_insert(HashTable *table, void *data)
{
    size_t hash = table->hashData(data);
    hash %= table->nBuckets;
    rb_tree_insert(&table->buckets[hash], data);
    table->size++;
}

void *hash_table_find(HashTable *table, void *data)
{
    size_t hash = table->hashData(data);
    hash %= table->nBuckets;
    return rb_tree_find(&table->buckets[hash], data);
}

void hash_table_remove(HashTable *table, void *data)
{
    size_t hash = table->hashData(data);
    hash %= table->nBuckets;
    rb_tree_remove(&table->buckets[hash], data);
    table->size--;
}
