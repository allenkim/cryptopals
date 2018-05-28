#include <stdlib.h>
#include <string.h>
#include "hash_table.h"

static ht_item HT_DELETED_ITEM = {NULL, 0};

static ht_item* ht_new_item(unsigned char* k, size_t klen, int v){
	ht_item* it = (ht_item*)malloc(sizeof(ht_item));
	it->key = (unsigned char*)malloc(klen);
	memcpy(it->key, k, klen);
	it->value = v;
	return it;
}

ht_hash_table* ht_new(size_t init_size){
	ht_hash_table* ht = (ht_hash_table*)malloc(sizeof(ht_hash_table));
	ht->size = init_size;
	ht->count = 0;
	ht->items = (ht_item**)calloc(ht->size, sizeof(ht_item*));
	return ht;
}

static void ht_del_item(ht_item* i){
	free(i->key);
	free(i);
}

void ht_del_hash_table(ht_hash_table* ht){
	for (int i = 0; i < ht->size; i++){
		ht_item* it = ht->items[i];
		if (it)
			ht_del_item(it);
	}
	free(ht->items);
	free(ht);
}

static size_t ht_hash(unsigned char* str, size_t strlen, const int m) {
	size_t hash = 5381;
	int c;
	for (int i = 0; i < strlen; i++){
		c = str[i];
		hash = ((hash << 5) + hash) + c;
	}
	return hash % m;
}

void ht_insert(ht_hash_table* ht, unsigned char* key, size_t keylen, int value){
	size_t idx = ht_hash(key, keylen, ht->size);
	ht_item* curr = ht->items[idx];
	while (curr){
		if (curr == &HT_DELETED_ITEM)
			break;
		if (!memcmp(curr->key, key, keylen)){
			curr->value = value;
			ht->count++;
			return;
		}
		idx = (idx+1) % ht->size;
		curr = ht->items[idx];
	}
	ht_item* it = ht_new_item(key, keylen, value);
	ht->items[idx] = it;
	ht->count++;
}

bool ht_search(ht_hash_table* ht, unsigned char* key, size_t keylen, int* value){
	size_t idx = ht_hash(key, keylen, ht->size);
	ht_item* curr = ht->items[idx];
	while (curr){
		if (curr != &HT_DELETED_ITEM){
			if (!memcmp(curr->key, key, keylen)){
				*value = curr->value;
				return true;
			}
		}
		idx = (idx+1) % ht->size;
		curr = ht->items[idx];
	}
	return false;
}

bool ht_delete(ht_hash_table* ht, unsigned char* key, size_t keylen){
	size_t idx = ht_hash(key, keylen, ht->size);
	ht_item* curr = ht->items[idx];
	while (curr){
		if (!memcmp(curr->key, key, keylen)){
			ht_del_item(curr);
			ht->items[idx] = &HT_DELETED_ITEM;
			ht->count--;
			return true;
		}
		idx = (idx+1) % ht->size;
		curr = ht->items[idx];
	}
	return false;
}

