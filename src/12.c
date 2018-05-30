#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "bytes.h"
#include "aes.h"
#include "oracles.h"

int encrypt_oracle_12(unsigned char* plaintext, int plaintext_len, unsigned char* ciphertext);

int detect_block_size(int max_size){
	int test_size = 1;
	unsigned char* test_pt = (unsigned char*)malloc(test_size);
	unsigned char* ct = (unsigned char*)malloc(max_size + test_size);
	test_pt[0] = 'A';
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

int detect_ecb(int block_size, int max_size){
	int test_len = block_size * 2;
	unsigned char test_pt[test_len];
	memset(test_pt, 0, test_len);
	unsigned char ct[test_len + max_size];
	int ctlen = encrypt_oracle_12(test_pt, test_len, ct);
	int max_freq = 1;
	int freq = max_freq_bytes(ct, ctlen, block_size);
	if (freq > max_freq){
		max_freq = freq;
	}
	return max_freq > 1;
}



int main(){
	const int MAX_UNKNOWN_SIZE = 1000;
	int block_size = detect_block_size(MAX_UNKNOWN_SIZE);
	printf("Block Size: %d\n", block_size);
	int ecb = detect_ecb(block_size, MAX_UNKNOWN_SIZE);
	printf("Using ECB: %s\n", ecb ? "YES" : "NO");
	return 0;
}

