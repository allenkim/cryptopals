#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bytes.h"
#include "aes.h"
#include "hash_table.h"

void detect_mode(unsigned char* ct, int ctlen){
	unsigned char top_key[AES_BLOCK_SIZE];
	unsigned char key[AES_BLOCK_SIZE];
	int max_rep = 1;
	ht_hash_table* ht = ht_new(17);
	for (int i = 0; i < ctlen; i += AES_BLOCK_SIZE){
		memcpy(key, ct+i, AES_BLOCK_SIZE);
		int freq = 1;
		bool found = ht_search(ht, key, AES_BLOCK_SIZE, &freq);
		if (found){
			if (++freq > max_rep){
				max_rep = freq;
				memcpy(top_key, key, AES_BLOCK_SIZE);
			}
		}
		ht_insert(ht, key, AES_BLOCK_SIZE, freq);
	}
	ht_del_hash_table(ht);
	printf("MODE GUESS: %s\n", (max_rep == 1) ? "CBC" : "ECB");
}

int main(){
	// Worst case: appends 5 bytes before, so we need at least 11+AES_BLOCK_SIZE+AES_BLOCK_SIZE=43 bytes for repeat
	int ptlen = 43;
	unsigned char pt[ptlen];
	memset(pt, 0, ptlen);
	unsigned char ct[ptlen+AES_BLOCK_SIZE];
	int mode;
	int ctlen = aes_128_rand_encrypt(pt, ptlen, ct, &mode);
	printf("MODE USED:  %s\n", mode ? "CBC" : "ECB");
	print_bytes(ct, ctlen);
	detect_mode(ct, ctlen);
	return 0;
}

