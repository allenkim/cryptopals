#ifndef HASH_TABLE_H
#define HASH_TABLE_H
#include <stddef.h>
#include <stdbool.h>

typedef struct{
    unsigned char* key;
    int value;
} ht_item;

typedef struct {
    size_t size;
    size_t count;
    ht_item** items;
} ht_hash_table;

ht_hash_table* ht_new(size_t init_size);
void ht_del_hash_table(ht_hash_table* ht);
void ht_insert(ht_hash_table* ht, unsigned char* key, size_t keylen, int value);
bool ht_search(ht_hash_table* ht, unsigned char* key, size_t keylen, int* value);
bool ht_delete(ht_hash_table* ht, unsigned char* key, size_t keylen);

#endif
