#ifndef HASH_TABLE_H
#define HASH_TABLE_H
#include <stddef.h>
#include <stdbool.h>

typedef struct{
    unsigned char* key;
	size_t key_len;
	unsigned char* val;
	size_t val_len;
} ht_item;

typedef struct {
	size_t base_size;
    size_t size;
    size_t count;
    ht_item** items;
} ht_hash_table;

ht_hash_table* ht_new(void);
void ht_del_hash_table(ht_hash_table* ht);
void ht_insert(ht_hash_table* ht, unsigned char* key, size_t key_len, unsigned char* val, size_t val_len);
unsigned char* ht_search(ht_hash_table* ht, unsigned char* key, size_t key_len, size_t* val_len);
bool ht_delete(ht_hash_table* ht, unsigned char* key, size_t key_len);

#endif

