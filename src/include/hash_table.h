#ifndef MBCL_HASH_TABLE_H
#define MBCL_HASH_TABLE_H

#include "mbcl.h"
#include "array.h"

typedef struct
{
    void *key;
    void *value;
    size_t hash;
    MBCL_DATA_COMPARE_FUNCTION compareKey;
} HashTableEntry;

HashTableEntry *hash_table_entry_new(void *key,
                                     void *value,
                                     size_t hash,
                                     MBCL_DATA_COMPARE_FUNCTION compareData);

void hash_table_entry_free(HashTableEntry *entry,
                           MBCL_DATA_FREE_FUNCTION freeKey,
                           MBCL_DATA_FREE_FUNCTION freeValue);

typedef struct
{
    MBCL_CONTAINER_BASE;
    MBCL_DATA_FREE_FUNCTION freeValue;
    Array buckets;
    size_t nBuckets;
    size_t (*hashData)(void *data);
} HashTable;

void hash_table_init(HashTable *table,
                     MBCL_DATA_FREE_FUNCTION freeKey,
                     MBCL_DATA_FREE_FUNCTION freeValue,
                     MBCL_DATA_COMPARE_FUNCTION compareKey,
                     size_t (*hashData)(void *data),
                     size_t nBuckets);

HashTable *hash_table_new(MBCL_DATA_FREE_FUNCTION freeKey,
                          MBCL_DATA_FREE_FUNCTION freeValue,
                          MBCL_DATA_COMPARE_FUNCTION compareKey,
                          size_t (*hashData)(void *data),
                          size_t nBuckets);

void hash_table_deinit(HashTable *table);

void hash_table_free(HashTable *table);

void hash_table_insert(HashTable *table, void *key, void *value);

void *hash_table_find(HashTable *table, void *key);

void hash_table_remove(HashTable *table, void *key);

Iterator *hash_table_begin(HashTable *table);

#endif