#include <stdlib.h>
#include <string.h>
#include "hash_table.h"
#include "prime.h"

#define HT_INIT_SIZE 256

static ht_item HT_DELETED_ITEM = {NULL, 0};

static ht_item* ht_new_item(unsigned char* k, size_t klen, unsigned char* v, size_t vlen){
	ht_item* it = (ht_item*)malloc(sizeof(ht_item));

	it->key = (unsigned char*)malloc(klen);
	memcpy(it->key, k, klen);
	it->key_len = klen;

	it->val = (unsigned char*)malloc(vlen);
	memcpy(it->val, v, vlen);
	it->val_len = vlen;

	return it;
}

static ht_hash_table* ht_new_sized(const size_t base_size){
	ht_hash_table* ht = (ht_hash_table*)malloc(sizeof(ht_hash_table));
	ht->base_size = base_size;
	ht->size = next_prime(base_size);
	ht->count = 0;
	ht->items = (ht_item**)calloc(ht->size, sizeof(ht_item*));
	return ht;
}

ht_hash_table* ht_new(void){
	return ht_new_sized(HT_INIT_SIZE);
}

static void ht_resize(ht_hash_table* ht, const size_t base_size){
	if (base_size <= HT_INIT_SIZE)
		return;
	ht_hash_table* new_ht = ht_new_sized(base_size);
	for (int i = 0; i < ht->size; i++){
		ht_item* item = ht->items[i];
		if (item && item != &HT_DELETED_ITEM)
			ht_insert(new_ht, item->key, item->key_len, item->val, item->val_len);
	}
	ht->base_size = new_ht->base_size;
	ht->count = new_ht->count;

	ht_item** tmp_items = ht->items;
    ht->items = new_ht->items;
    new_ht->items = tmp_items;

	const size_t tmp_size = ht->size;
	ht->size = new_ht->size;
	new_ht->size = tmp_size;

	ht_del_hash_table(new_ht);
}

static void ht_resize_up(ht_hash_table* ht){
	ht_resize(ht, ht->base_size * 2);
}

static void ht_resize_down(ht_hash_table* ht){
	ht_resize(ht, ht->base_size / 2);
}

static void ht_del_item(ht_item* i){
	free(i->key);
	free(i->val);
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

void ht_insert(ht_hash_table* ht, unsigned char* key, size_t key_len, unsigned char* val, size_t val_len){
	const size_t load = ht->count * 100 / ht->size;
	if (load > 70)
		ht_resize_up(ht);
	size_t idx = ht_hash(key, key_len, ht->size);
	ht_item* curr = ht->items[idx];
	ht_item* it = ht_new_item(key, key_len, val, val_len);
	while (curr){
		if (curr == &HT_DELETED_ITEM)
			break;
		if (!memcmp(curr->key, key, key_len)){
			ht_del_item(curr);
			break;
		}
		idx = (idx+1) % ht->size;
		curr = ht->items[idx];
	}
	ht->items[idx] = it;
	ht->count++;
}

unsigned char* ht_search(ht_hash_table* ht, unsigned char* key, size_t key_len, size_t* val_len){
	size_t idx = ht_hash(key, key_len, ht->size);
	ht_item* curr = ht->items[idx];
	while (curr){
		if (curr != &HT_DELETED_ITEM){
			if (!memcmp(curr->key, key, key_len)){
				*val_len = curr->val_len;
				return curr->val;
			}
		}
		idx = (idx+1) % ht->size;
		curr = ht->items[idx];
	}
	*val_len = 0;
	return NULL;
}

bool ht_delete(ht_hash_table* ht, unsigned char* key, size_t key_len){
	const size_t load = ht->count * 100 / ht->size;
	if (load < 10)
		ht_resize_down(ht);
	size_t idx = ht_hash(key, key_len, ht->size);
	ht_item* curr = ht->items[idx];
	while (curr){
		if (!memcmp(curr->key, key, key_len)){
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

