#include "hash_table.h"

#include <stdlib.h>
#include <string.h>

void hash_table_init(HashTable *table,
                     MBCL_DATA_FREE_FUNCTION freeData,
                     MBCL_DATA_COMPARE_FUNCTION compareData,
                     size_t (*hashData)(void *data),
                     size_t nBuckets)
{
    table->buckets = malloc(nBuckets * sizeof(RBTree));
    table->nBuckets = nBuckets;
    for (size_t bucketInitIdx = 0; bucketInitIdx < nBuckets; bucketInitIdx++)
    {
        rb_tree_init(&table->buckets[bucketInitIdx], freeData, compareData);
    }
    table->compareData = compareData;
    table->hashData = hashData;
}

HashTable *hash_table_new(MBCL_DATA_FREE_FUNCTION freeData,
                          MBCL_DATA_COMPARE_FUNCTION compareData,
                          size_t (*hashData)(void *data),
                          size_t nBuckets)
{
    HashTable *wipTable = malloc(sizeof(HashTable));
    memset(wipTable, 0, sizeof(HashTable));
    hash_table_init(wipTable, freeData, compareData, hashData, nBuckets);

    return wipTable;
}

void hash_table_deinit(HashTable *table)
{
    for (size_t bucketInitIdx = 0; bucketInitIdx < table->nBuckets; bucketInitIdx++)
    {
        rb_tree_deinit(&table->buckets[bucketInitIdx]);
    }
}

void hash_table_free(HashTable *table)
{
    hash_table_deinit(table);
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

ssize_t ptr_compare(void *a, void *b)
{
    return (ssize_t)a - (ssize_t)b;
}

Iterator *hash_table_begin(HashTable *table)
{
    Iterator *tableIterator = iterator_new(table->size);

    Iterator **bucketIterators = malloc(table->nBuckets * sizeof(Iterator *));

    for (size_t bucketIdx = 0; bucketIdx < table->nBuckets; bucketIdx++)
    {
        bucketIterators[bucketIdx] = rb_tree_begin(&table->buckets[bucketIdx]);
    }

    size_t placedIdx = 0;
    while (placedIdx < table->size)
    {
        Iterator *smallest = NULL;
        for (size_t bucketIdx = 0; bucketIdx < table->nBuckets; bucketIdx++)
        {
            if (iterator_valid(bucketIterators[bucketIdx]))
            {
                void *compared = iterator_get(bucketIterators[bucketIdx]);
                if ((smallest == NULL) || (table->compareData(compared, iterator_get(smallest)) < 0))
                {
                    smallest = bucketIterators[bucketIdx];
                }
            }
        }
        tableIterator->data[placedIdx] = iterator_get(smallest);
        iterator_next(smallest);
        placedIdx++;
    }

    for (size_t bucketIdx = 0; bucketIdx < table->nBuckets; bucketIdx++)
    {
        iterator_free(bucketIterators[bucketIdx]);
    }
    free(bucketIterators);

    return tableIterator;
}
