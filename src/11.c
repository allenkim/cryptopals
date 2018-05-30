#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bytes.h"
#include "aes.h"
#include "oracles.h"
#include "hash_table.h"

void detect_mode(unsigned char* ct, int ctlen){
	int max_freq = 1;
	int freq = max_freq_bytes(ct, ctlen, 16);
	if (freq > max_freq){
		max_freq = freq;
	}
	printf("MODE GUESS: %s\n", (max_freq == 1) ? "CBC" : "ECB");
}

int main(){
	// Worst case: appends 5 bytes before, so we need at least 11+AES_BLOCK_SIZE+AES_BLOCK_SIZE=43 bytes for repeat
	int ptlen = 43;
	unsigned char pt[ptlen];
	memset(pt, 0, ptlen);
	unsigned char ct[ptlen+AES_BLOCK_SIZE];
	int mode;
	int ctlen = encrypt_oracle_11(pt, ptlen, ct, &mode);
	print_bytes(ct, ctlen);
	printf("MODE USED:  %s\n", mode ? "CBC" : "ECB");
	detect_mode(ct, ctlen);
	return 0;
}

