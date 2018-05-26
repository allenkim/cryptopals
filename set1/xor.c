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

