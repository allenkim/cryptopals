#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
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

float letter_freq[26];

void init_letter_freq(){
	letter_freq['a' % 26] = 0.08167;
	letter_freq['b' % 26] = 0.01492;
	letter_freq['c' % 26] = 0.02782;
	letter_freq['d' % 26] = 0.04253;
	letter_freq['e' % 26] = 0.12702;
	letter_freq['f' % 26] = 0.02228;
	letter_freq['g' % 26] = 0.02015;
	letter_freq['h' % 26] = 0.06094;
	letter_freq['i' % 26] = 0.06966;
	letter_freq['j' % 26] = 0.00153;
	letter_freq['k' % 26] = 0.00772;
	letter_freq['l' % 26] = 0.04025;
	letter_freq['m' % 26] = 0.02406;
	letter_freq['n' % 26] = 0.06749;
	letter_freq['o' % 26] = 0.07507;
	letter_freq['p' % 26] = 0.01929;
	letter_freq['q' % 26] = 0.00095;
	letter_freq['r' % 26] = 0.05987;
	letter_freq['s' % 26] = 0.06327;
	letter_freq['t' % 26] = 0.09056;
	letter_freq['u' % 26] = 0.02758;
	letter_freq['v' % 26] = 0.00978;
	letter_freq['w' % 26] = 0.02360;
	letter_freq['x' % 26] = 0.00150;
	letter_freq['y' % 26] = 0.01974;
	letter_freq['z' % 26] = 0.00074;
}

float letter_freq_stat_dist(float freq[26]){
	float dist = 0;
	for (int i = 0; i < 26; i++)
		dist += fabsf(freq[i]-letter_freq[i]);
	return dist;
}

float score_plaintext(unsigned char* bytes, size_t len){
	float freq[26];
	for (int i = 0; i < 26; i++)
		freq[i] = 0.0;
	size_t count = 0;
	for (int i = 0; i < len; i++){
		int c = tolower(bytes[i]);
		if (c >= 'a' && c <= 'z'){
			freq[c % 26]++;
			count++;
		}
		if (c != '\n' && c != ' ' && (c <= 31 || c >= 127))
			return 100.0;
	}
	for (int i = 0; i < 26; i++){
		freq[i] /= count;
	}
	return letter_freq_stat_dist(freq);
}

int score_single_byte_xor(unsigned char* bytes, size_t len, unsigned char** topp,  unsigned char* byte){
	unsigned char* top = NULL;
	float top_score = 100.0;
	unsigned char key = 0;
	for (unsigned char i = 0; i < 255; i++){
		unsigned char* xorbytes = single_byte_xor(bytes, i, len);
		float score = score_plaintext(xorbytes, len);
		if (score < top_score){
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
	float score = 0;
	int count = 0;
	for (int i = 0; i <= len-2*keysize; i += 2*keysize){
		score += (float)(hamming_dist(bytes+i,bytes+i+keysize,keysize)) / keysize;
		count++;
	}
	return score/count;
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

unsigned char* transpose_bytes(unsigned char* bytes, size_t len, size_t keysize, size_t** transposed_lens){
	unsigned char* transposed_bytes = (unsigned char*)malloc(len*sizeof(unsigned char));
	for (size_t i = 0; i < keysize; i++){
		(*transposed_lens)[i] = len / keysize + (i < (len % keysize));
	}
	int k = 0;
	for (int i = 0; i < keysize; i++){
		for (int j = 0; j < (*transposed_lens)[i]; j++){
			transposed_bytes[k++] = bytes[j*keysize+i];
		}
	}
	return transposed_bytes;
}

unsigned char* find_repeating_xor_key(unsigned char* bytes, size_t len, size_t keysize){
	unsigned char* key = (unsigned char*)malloc(keysize*sizeof(unsigned char));
	size_t* transposed_lens = (size_t*)malloc(keysize*sizeof(size_t));
	unsigned char* transposed_bytes = transpose_bytes(bytes, len, keysize, &transposed_lens);
	unsigned char* topp;
	unsigned char byte;
	int offset = 0;
	for (int i = 0; i < keysize; i++){
		score_single_byte_xor(transposed_bytes + offset, transposed_lens[i], &topp, &byte);
		if (!topp){
			free(transposed_bytes);
			free(transposed_lens);
			free(key);
			return NULL;
		}
		offset += transposed_lens[i];
		key[i] = byte;
	}
	free(topp);
	free(transposed_bytes);
	free(transposed_lens);
	return key;
}

