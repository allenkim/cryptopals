#include <stdio.h>
#include <stdlib.h>
#include "xor.h"

unsigned char* xorbytes(unsigned char* bytes1, unsigned char* bytes2, size_t len){
	unsigned char* xorbytes_arr = (unsigned char*)malloc(len*sizeof(unsigned char));
	for (int i = 0; i < len; i++){
		xorbytes_arr[i] = bytes1[i] ^ bytes2[i];
	}
	return xorbytes_arr;
}

unsigned char* single_byte_xor(unsigned char* bytes, unsigned char byte, size_t len){
	unsigned char* xorbytes_arr = (unsigned char*)malloc(len*sizeof(unsigned char));
	unsigned char* charbytes = (unsigned char*)malloc(len*sizeof(unsigned char));
	for (int i = 0; i < len; i++)
		charbytes[i] = byte;
	xorbytes_arr = xorbytes(bytes, charbytes, len);
	free(charbytes);
	return xorbytes_arr;
}

int score_plaintext(unsigned char* bytes, size_t len){
	int score = 0;
	for (int i = 0; i < len; i++){
		char c = bytes[i];
		if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == ' ')
			score++;
		else if ((c > 13 && c < 32) || c > 127)
			score -= 10;
		else
			score--;
	}
	return score;
}

int score_single_byte_xor(unsigned char* bytes, size_t len, unsigned char** topp,  unsigned char* byte){
	unsigned char* top = NULL;
	int top_score = 0;
	unsigned char key = 0;
	for (unsigned char i = 0; i < 255; i++){
		unsigned char* xorbytes = single_byte_xor(bytes, i, len);
		int score = score_plaintext(xorbytes, len);
		if (score > top_score){
			top_score = score;
			top = xorbytes;
			key = i;
		}
		else{
			free(xorbytes);
		}
	}
	*topp = top;
	*byte = key;
	return top_score;
}

unsigned char* repeating_key_xor(unsigned char* bytes, unsigned char* key, size_t len, size_t key_len){
	unsigned char* xorbytes_arr = (unsigned char*)malloc(len*sizeof(unsigned char));
	unsigned char* charbytes = (unsigned char*)malloc(len*sizeof(unsigned char));
	for (int i = 0; i < len; i++){
		charbytes[i] = key[i % key_len];
	}
	xorbytes_arr = xorbytes(bytes, charbytes, len);
	free(charbytes);
	return xorbytes_arr;
}

size_t hamming_dist(unsigned char* str1, unsigned char* str2, size_t len){
	size_t dist = 0;
	for (int i = 0; i < len; i++){
		unsigned char xorbyte = str1[i] ^ str2[i];
		while (xorbyte){
			xorbyte &= (xorbyte - 1);
			dist++;
		}
	}
	return dist;
}

float score_keysize(unsigned char* bytes, size_t len, size_t keysize){
	if (keysize*2 > len){
		fprintf(stderr, "Key size '%zu' too large for len %zu\n", keysize, len);
		exit(EXIT_FAILURE);
	}
	float score = (float)(hamming_dist(bytes,bytes+keysize,keysize)) / keysize;
	return score;
}

typedef struct key{
	size_t keysize;
	float score;
} key;

static int keycmp (const void* a, const void* b){
	key* k1 = (key*)a;
	key* k2 = (key*)b;
	if (k1->score < k2->score)
		return -1;
	else if (k1->score > k2->score)
		return 1;
	else
		return 0;
}

size_t* find_best_keysizes(unsigned char* bytes, size_t len, size_t max){
	key* keys = (key*)malloc(len*sizeof(key));
	for (size_t keysize = 1; keysize <= max; keysize++){
		float score = score_keysize(bytes, len, keysize);
		keys[keysize-1].keysize = keysize;
		keys[keysize-1].score = score;
	}
	qsort(keys, max, sizeof(key), keycmp);
	size_t* keysizes = (size_t*)malloc(max*sizeof(size_t));
	for (int i = 0; i < max; i++){
		keysizes[i] = keys[i].keysize;
		printf("%zu %f\n", keys[i].keysize, keys[i].score);
	}
	return keysizes;
}

