#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "bytes.h"
#include "aes.h"

static bool key_12_set;
static unsigned char key_12[16];

int encrypt_oracle_11(unsigned char* plaintext, int plaintext_len, unsigned char* ciphertext, int* mode){
	unsigned char key[16];
	unsigned char iv[16];
	rand_bytes(key, 16);
	rand_bytes(iv, 16);
	int prelen = randint(5,10);
	int postlen = randint(5,10);
	int padded_len = prelen + plaintext_len + postlen;
	unsigned char* padded_pt = (unsigned char*)malloc(padded_len);
	rand_bytes(padded_pt, prelen);
	memcpy(padded_pt+prelen, plaintext, plaintext_len);
	rand_bytes(padded_pt+prelen+plaintext_len, postlen);
	int ciphertext_len = -1;
	*mode = randint(0,1);
	switch (*mode){
		case 0: // ECB
			ciphertext_len = aes_128_ecb_encrypt(padded_pt, padded_len, key, NULL, ciphertext);
			break;
		case 1: // CBC
			ciphertext_len = aes_128_cbc_encrypt(padded_pt, padded_len, key, iv, ciphertext);
			break;
	}
	free(padded_pt);

	return ciphertext_len;
}

int encrypt_oracle_12(unsigned char* plaintext, int plaintext_len, unsigned char* ciphertext){
	if (!key_12_set){
		rand_bytes(key_12, 16);
		key_12_set = true;
	}
	char* unknown_str = "Um9sbGluJyBpbiBteSA1LjAKV2l0aCBteSByYWctdG9wIGRvd24gc28gbXkgaGFpciBjYW4gYmxvdwpUaGUgZ2lybGllcyBvbiBzdGFuZGJ5IHdhdmluZyBqdXN0IHRvIHNheSBoaQpEaWQgeW91IHN0b3A/IE5vLCBJIGp1c3QgZHJvdmUgYnkK";
	size_t unknown_bytes_len;
	unsigned char* unknown_bytes = base64_to_bytes(unknown_str, strlen(unknown_str), &unknown_bytes_len);
	int padded_len = plaintext_len + unknown_bytes_len;
	unsigned char* padded_pt = (unsigned char*)malloc(padded_len);
	memcpy(padded_pt, plaintext, plaintext_len);
	memcpy(padded_pt+plaintext_len, unknown_bytes, unknown_bytes_len);
	int ciphertext_len = aes_128_ecb_encrypt(padded_pt, padded_len, key_12, NULL, ciphertext);
	free(padded_pt);
	free(unknown_bytes);
	return ciphertext_len;
}

