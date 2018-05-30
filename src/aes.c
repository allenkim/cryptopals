#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/rand.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "xor.h"
#include "bytes.h"
#include "aes.h"

static bool rand_seed_initialized;

unsigned char* pkcs7_pad(unsigned char* plaintext, int plaintext_len, int block_len, int* padded_lenp){
	int extra_len = block_len - plaintext_len % block_len;
	if (!extra_len)
		extra_len = block_len;
	int padded_len = plaintext_len + extra_len;
	if (extra_len > 255){
		fprintf(stderr, "Cannot pad '%d' for pkcs7_pad\n", extra_len);
		exit(EXIT_FAILURE);
	}
	unsigned char* padded_plaintext = (unsigned char*)malloc(padded_len);
	memcpy(padded_plaintext, plaintext, plaintext_len);
	for (int i = 0; i < extra_len; i++){
		padded_plaintext[padded_len-i-1] = (unsigned char)extra_len;
	}
	*padded_lenp = padded_len;
	return padded_plaintext;
}

unsigned char* pkcs7_unpad(unsigned char* padded_plaintext, int padded_len, int* plaintext_lenp){
	int extra_len = padded_plaintext[padded_len-1];
	int plaintext_len = padded_len - extra_len;
	unsigned char* plaintext = (unsigned char*)malloc(plaintext_len);
	memcpy(plaintext, padded_plaintext, plaintext_len);
	*plaintext_lenp = plaintext_len;
	return plaintext;
}

int aes_block_encrypt(unsigned char* plaintext, unsigned char* key, unsigned char* ciphertext){
	EVP_CIPHER_CTX *ctx;
	int len;
	int ciphertext_len;

	if (!(ctx = EVP_CIPHER_CTX_new()))
		ERR_print_errors_fp(stderr);

	if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_128_ecb(), NULL, key, NULL))
		ERR_print_errors_fp(stderr);

	EVP_CIPHER_CTX_set_padding(ctx, 0);

	if (1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, AES_BLOCK_SIZE))
		ERR_print_errors_fp(stderr);
	ciphertext_len = len;

	if (1 != EVP_EncryptFinal_ex(ctx, ciphertext + len, &len))
		ERR_print_errors_fp(stderr);
	ciphertext_len += len;

	EVP_CIPHER_CTX_free(ctx);

	return ciphertext_len;
}

int aes_block_decrypt(unsigned char *ciphertext, unsigned char *key, unsigned char *plaintext){
	EVP_CIPHER_CTX *ctx;
	int len;
	int plaintext_len;

	if (!(ctx = EVP_CIPHER_CTX_new()))
		ERR_print_errors_fp(stderr);

	if (1 != EVP_DecryptInit_ex(ctx, EVP_aes_128_ecb(), NULL, key, NULL))
		ERR_print_errors_fp(stderr);

	EVP_CIPHER_CTX_set_padding(ctx, 0);

	if (1 != EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, AES_BLOCK_SIZE))
		ERR_print_errors_fp(stderr);
	plaintext_len = len;

	if (1 != EVP_DecryptFinal_ex(ctx, plaintext + len, &len))
		ERR_print_errors_fp(stderr);
	plaintext_len += len;

	EVP_CIPHER_CTX_free(ctx);

	return plaintext_len;
}


int aes_128_ecb_encrypt(unsigned char* plaintext, int plaintext_len, unsigned char* key, unsigned char* iv, unsigned char* ciphertext){
	EVP_CIPHER_CTX *ctx;
	int len;
	int ciphertext_len;

	if (!(ctx = EVP_CIPHER_CTX_new()))
		ERR_print_errors_fp(stderr);

	if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_128_ecb(), NULL, key, NULL))
		ERR_print_errors_fp(stderr);

	if (1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len))
		ERR_print_errors_fp(stderr);
	ciphertext_len = len;

	if (1 != EVP_EncryptFinal_ex(ctx, ciphertext + len, &len))
		ERR_print_errors_fp(stderr);
	ciphertext_len += len;

	EVP_CIPHER_CTX_free(ctx);

	return ciphertext_len;
}

int aes_128_ecb_decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *key, unsigned char *iv, unsigned char *plaintext){
	EVP_CIPHER_CTX *ctx;
	int len;
	int plaintext_len;

	if (!(ctx = EVP_CIPHER_CTX_new()))
		ERR_print_errors_fp(stderr);

	if (1 != EVP_DecryptInit_ex(ctx, EVP_aes_128_ecb(), NULL, key, NULL))
		ERR_print_errors_fp(stderr);

	if (1 != EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len))
		ERR_print_errors_fp(stderr);
	plaintext_len = len;

	if (1 != EVP_DecryptFinal_ex(ctx, plaintext + len, &len))
		ERR_print_errors_fp(stderr);
	plaintext_len += len;

	EVP_CIPHER_CTX_free(ctx);

	return plaintext_len;
}


int aes_128_cbc_encrypt(unsigned char* plaintext, int plaintext_len, unsigned char* key, unsigned char* iv, unsigned char* ciphertext){
	int padded_len;
	unsigned char* padded_plaintext = pkcs7_pad(plaintext, plaintext_len, AES_BLOCK_SIZE, &padded_len);
	unsigned char* prev = iv;
	for (int i = 0; i < padded_len; i+=AES_BLOCK_SIZE){
		unsigned char* xor_input = xorbytes(padded_plaintext+i, prev, AES_BLOCK_SIZE);
		aes_block_encrypt(xor_input, key, ciphertext+i);
		prev = ciphertext+i;
		free(xor_input);
	}
	free(padded_plaintext);
	return padded_len;
}

int aes_128_cbc_decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *key, unsigned char *iv, unsigned char *plaintext){
	unsigned char* prev = iv;
	for (int i = 0; i < ciphertext_len; i+=AES_BLOCK_SIZE){
		aes_block_decrypt(ciphertext+i, key, plaintext+i);
		xorbytes_inplace(plaintext+i, prev, AES_BLOCK_SIZE);
		prev = ciphertext+i;
	}
	return ciphertext_len - (unsigned)plaintext[ciphertext_len-1];
}

void rand_bytes(unsigned char* bytes, size_t len){
	if (!RAND_bytes(bytes, len))
		ERR_print_errors_fp(stderr);
}

int randint(int low, int high){
	if (!rand_seed_initialized){
		srand((unsigned int)time(NULL));
		rand_seed_initialized = true;
	}
	return rand() % (high - low + 1) + low;
}

