#include "hash_table.h"

#include "list.h"
#include <stdlib.h>
#include <string.h>

#include <stdio.h>

ssize_t hash_table_entry_compare(void *dataA, void *dataB)
{
    HashTableEntry *entryA = dataA;
    HashTableEntry *entryB = dataB;
    if (entryA->hash == entryB->hash)
    {
        return entryA->compareKey(entryA->key, entryB->key);
    }
    return (ssize_t)entryA->hash - (ssize_t)entryB->hash;
}

HashTableEntry *hash_table_entry_new(void *key,
                                     void *value,
                                     size_t hash,
                                     MBCL_DATA_COMPARE_FUNCTION compareKey)
{

    HashTableEntry *wipEntry = malloc(sizeof(HashTableEntry));
    wipEntry->key = key;
    wipEntry->value = value;
    wipEntry->hash = hash;
    wipEntry->compareKey = compareKey;
    return wipEntry;
}

void hash_table_entry_free(HashTableEntry *entry,
                           MBCL_DATA_FREE_FUNCTION freeKey,
                           MBCL_DATA_FREE_FUNCTION freeValue)
{
    if (freeKey)
    {
        freeKey(entry->key);
    }

    if (freeValue)
    {
        freeValue(entry->value);
    }

    free(entry);
}

void hash_table_init(HashTable *table,
                     MBCL_DATA_FREE_FUNCTION freeKey,
                     MBCL_DATA_FREE_FUNCTION freeValue,
                     MBCL_DATA_COMPARE_FUNCTION compareKey,
                     size_t (*hashData)(void *data),
                     size_t nBuckets)
{
    array_init(&table->buckets, NULL, nBuckets);
    table->nBuckets = nBuckets;
    table->compareData = compareKey;
    table->freeData = freeKey;
    table->freeValue = freeValue;
    table->hashData = hashData;
}

HashTable *hash_table_new(MBCL_DATA_FREE_FUNCTION freeKey,
                          MBCL_DATA_FREE_FUNCTION freeValue,
                          MBCL_DATA_COMPARE_FUNCTION compareKey,
                          size_t (*hashData)(void *data),
                          size_t nBuckets)
{
    HashTable *wipTable = malloc(sizeof(HashTable));
    memset(wipTable, 0, sizeof(HashTable));
    hash_table_init(wipTable, freeKey, freeValue, compareKey, hashData, nBuckets);
    return wipTable;
}

void hash_table_deinit(HashTable *table)
{
    for (size_t bucketIdx = 0; bucketIdx < table->nBuckets; bucketIdx++)
    {
        List *bucket = array_at(&table->buckets, bucketIdx);
        if (bucket != NULL)
        {
            while (bucket->size > 0)
            {
                hash_table_entry_free(list_pop_back(bucket), table->freeData, table->freeValue);
            }
            list_free(bucket);
        }
    }
    array_deinit(&table->buckets);
}

void hash_table_free(HashTable *table)
{
    hash_table_deinit(table);
    free(table);
}

void hash_table_insert(HashTable *table, void *key, void *value)
{
    size_t hash = table->hashData(key);
    List *bucket = array_at(&table->buckets, hash % table->nBuckets);
    if (bucket == NULL)
    {
        bucket = list_new(NULL, hash_table_entry_compare);
        array_emplace(&table->buckets, hash % table->nBuckets, bucket);
    }
    list_append(bucket, hash_table_entry_new(key, value, hash, table->compareData));
    table->size++;
}

void *hash_table_find(HashTable *table, void *data)
{
    size_t hash = table->hashData(data);
    List *bucket = array_at(&table->buckets, hash % table->nBuckets);
    if (bucket == NULL)
    {
        return NULL;
    }
    HashTableEntry dummyEntry = {0};
    dummyEntry.key = data;
    dummyEntry.hash = hash;
    dummyEntry.compareKey = table->compareData;

    HashTableEntry *foundEntry = list_find(bucket, &dummyEntry);
    if (foundEntry == NULL)
    {
        return NULL;
    }
    return foundEntry->value;
}

void hash_table_remove(HashTable *table, void *data)
{
    MBCL_ASSERT(hash_table_find(table, data) != NULL, "Remove data not in hash table");
    size_t hash = table->hashData(data);
    hash %= table->nBuckets;
    List *bucket = array_at(&table->buckets, hash);
    HashTableEntry dummyEntry = {0};
    dummyEntry.key = data;
    dummyEntry.hash = hash;
    dummyEntry.compareKey = table->compareData;
    list_remove(bucket, &dummyEntry);
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
    memset(bucketIterators, 0, table->nBuckets * sizeof(Iterator *));

    for (size_t bucketIdx = 0; bucketIdx < table->nBuckets; bucketIdx++)
    {
        List *bucket = array_at(&table->buckets, bucketIdx);
        if (bucket != NULL)
        {
            bucketIterators[bucketIdx] = list_begin(bucket);
        }
    }

    size_t placedIdx = 0;
    while (placedIdx < table->size)
    {
        Iterator *smallest = NULL;
        for (size_t bucketIdx = 0; bucketIdx < table->nBuckets; bucketIdx++)
        {
            if ((bucketIterators[bucketIdx] != NULL) && iterator_gettable(bucketIterators[bucketIdx]))
            {
                struct HashTableEntry *compared = iterator_get(bucketIterators[bucketIdx]);
                if ((smallest == NULL) || (hash_table_entry_compare(compared, iterator_get(smallest)) < 0))
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
        if (bucketIterators[bucketIdx] != NULL)
        {
            iterator_free(bucketIterators[bucketIdx]);
        }
    }
    free(bucketIterators);

    return tableIterator;
}
