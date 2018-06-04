#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "bytes.h"
#include "aes.h"
#include "oracles.h"
#include "hash_table.h"

int detect_block_size(int max_size){
	int test_size = 0;
	unsigned char* test_pt = (unsigned char*)malloc(test_size);
	unsigned char* ct = (unsigned char*)malloc(max_size + test_size);
	int init_block_size = encrypt_oracle_12(test_pt, test_size, ct);
	int curr_block_size;
	while (1){
		free(test_pt);
		free(ct);
		test_size++;
		test_pt = (unsigned char*)malloc(test_size);
		ct = (unsigned char*)malloc(max_size + test_size);
		for (int i = 0; i < test_size; i++)
			test_pt[i] = 'A';
		curr_block_size = encrypt_oracle_12(test_pt, test_size, ct);
		if (curr_block_size > init_block_size)
			break;
	}
	free(test_pt);
	free(ct);
	return curr_block_size - init_block_size;
}

int detect_ecb(int block_size, int max_len){
	int test_len = block_size * 2;
	unsigned char test_pt[test_len];
	memset(test_pt, 0, test_len);
	unsigned char ct[test_len + max_len];
	int ctlen = encrypt_oracle_12(test_pt, test_len, ct);
	int max_freq = 1;
	int freq = max_freq_bytes(ct, ctlen, block_size);
	if (freq > max_freq){
		max_freq = freq;
	}
	return max_freq > 1;
}

int detect_unknown_str_len(int max_len){
	int test_len = 0;
	unsigned char* test_pt = (unsigned char*)malloc(test_len);
	unsigned char* ct = (unsigned char*)malloc(max_len + test_len);
	int init_block_size = encrypt_oracle_12(test_pt, test_len, ct);
	int curr_block_size;
	while (1){
		free(test_pt);
		free(ct);
		test_len++;
		test_pt = (unsigned char*)malloc(test_len);
		ct = (unsigned char*)malloc(max_len + test_len);
		for (int i = 0; i < test_len; i++)
			test_pt[i] = 'A';
		curr_block_size = encrypt_oracle_12(test_pt, test_len, ct);
		if (curr_block_size > init_block_size)
			break;
	}
	free(test_pt);
	free(ct);
	return init_block_size - test_len;
}

/*
 * pt should have last byte be modifiable
 */
ht_hash_table* create_last_byte_dict(unsigned char* pt, size_t ptlen, size_t max_ctlen){
	unsigned char ct[max_ctlen];
	ht_hash_table* ht = ht_new();
	for (int c = 0; c < 256; c++){
		pt[ptlen-1] = (unsigned char)c;
		encrypt_oracle_12(pt, ptlen, ct);
		ht_insert(ht, ct, ptlen, pt, ptlen);
	}
	return ht;
}

void find_unknown_str(unsigned char* unknown_str, size_t unknown_str_len, int block_size){
	int padded_len = block_size * (unknown_str_len / block_size + 1) - 1;

	size_t ptlen = padded_len + 1;
	unsigned char pt[ptlen];
	set_repeat_bytes(pt, padded_len, (unsigned char)'A');

	size_t max_ctlen = ptlen + unknown_str_len + block_size;
	unsigned char ct[max_ctlen];
	int num_chars_found = 0;
	ht_hash_table* ht;
	size_t val_len;
	unsigned char* val;
	unsigned char next_byte;
	while (num_chars_found < unknown_str_len){
		ht = create_last_byte_dict(pt, ptlen, max_ctlen);
		encrypt_oracle_12(pt, padded_len - num_chars_found, ct);
		val = ht_search(ht, ct, ptlen, &val_len);
		next_byte = val[val_len-1];
		unknown_str[num_chars_found++] = next_byte;
		for (int i = num_chars_found-1; i >= 0; i--)
			pt[padded_len-i-2] = pt[padded_len-i-1]; 
		pt[padded_len-1] = next_byte;
		ht_del_hash_table(ht);
	}
}

int main(){
	const int MAX_UNKNOWN_LEN = 200;
	int block_size = detect_block_size(MAX_UNKNOWN_LEN);
	printf("Block Size: %d\n", block_size);
	int ecb = detect_ecb(block_size, MAX_UNKNOWN_LEN);
	printf("Using ECB: %s\n", ecb ? "YES" : "NO");
	int unknown_str_len = detect_unknown_str_len(MAX_UNKNOWN_LEN);
	printf("Unknown String Length: %d\n", unknown_str_len);
	unsigned char unknown_str[unknown_str_len+1];
	unknown_str[unknown_str_len] = '\0';
	find_unknown_str(unknown_str, unknown_str_len, block_size);
	printf("%s", unknown_str);
	return 0;
}

