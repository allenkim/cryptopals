#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <string.h>
#include <errno.h>
#include "xor.h"
#include "bytes.h"

unsigned char* xorbytes(unsigned char* bytes1, unsigned char* bytes2, size_t len){
	unsigned char* xorbytes_arr = (unsigned char*)malloc(len*sizeof(unsigned char));
	for (int i = 0; i < len; i++){
		xorbytes_arr[i] = bytes1[i] ^ bytes2[i];
	}
	return xorbytes_arr;
}

void xorbytes_inplace(unsigned char* bytes1, unsigned char* bytes2, size_t len){
	for (int i = 0; i < len; i++){
		bytes1[i] ^= bytes2[i];
	}
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

float score_single_byte_xor(unsigned char* bytes, size_t len, unsigned char** topp,  unsigned char* byte){
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
	unsigned char* xorbytes_arr = (unsigned char*)malloc((len+1)*sizeof(unsigned char));
	xorbytes_arr[len] = 0;
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
	char* key = (char*)malloc((keysize+1)*sizeof(char));
	key[keysize] = 0;
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
	return (unsigned char*)key;
}

char* break_repeating_key_xor(unsigned char* bytes, size_t byteslen, unsigned char** keyp, size_t* keylenp){
	init_letter_freq();
	size_t MAX_KEYSIZE = 40;
	size_t* keysizes = find_best_keysizes(bytes, byteslen, MAX_KEYSIZE);
	unsigned char* key, *msg;
	int i;
	for (i = 0; i < MAX_KEYSIZE; i++){
		key = find_repeating_xor_key(bytes, byteslen, keysizes[i]);
		if (key)
			break;
	}
	*keyp = key;
	*keylenp = keysizes[i];
	msg = repeating_key_xor(bytes, key, byteslen, keysizes[i]);
	free(keysizes);
	return (char*)msg;
}

size_t vigenere_encrypt_file(const char* fnout, const char* fnin, unsigned char* key){
	FILE *fdin, *fdout;
	size_t size;

	fdin = fopen(fnin,"r");
	fseek(fdin, 0, SEEK_END);
	size = ftell(fdin);
	rewind(fdin);

	char* buff = (char*)malloc((size+1)*sizeof(char));
	fread(buff, size, sizeof(char), fdin);
	buff[size] = 0;

	unsigned char* cipher = repeating_key_xor((unsigned char*)buff, key, size, strlen((char*)key));
	size_t b64len;
	char* b64cipher = bytes_to_base64(cipher, size, &b64len);

	fdout = fopen(fnout,"w");
	fwrite(b64cipher, sizeof(char), b64len, fdout);

	free(buff);
	free(cipher);
	free(b64cipher);
	fclose(fdin);
	fclose(fdout);

	return b64len;
}

size_t vigenere_decrypt_file(const char* fnout, const char* fnin){
	FILE *fdin, *fdout;
	size_t size;

	fdin = fopen(fnin,"r");
	fseek(fdin, 0, SEEK_END);
	size = ftell(fdin);
	rewind(fdin);

	char* buff = (char*)malloc((size+1)*sizeof(char));
	fread(buff, size, sizeof(char), fdin);
	buff[size] = 0;
	size_t byteslen;
	unsigned char* bytes = base64_to_bytes(buff, size, &byteslen);

	unsigned char* key;
	size_t keylen;
	char* msg = break_repeating_key_xor(bytes, byteslen, &key, &keylen);
	size_t msglen = strlen(msg);

	fdout = fopen(fnout,"w");
	fwrite(msg, sizeof(char), msglen, fdout);

	free(bytes);
	free(msg);
	free(key);
	fclose(fdin);
	fclose(fdout);

	return msglen;
}

