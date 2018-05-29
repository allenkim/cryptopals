#ifndef AES_H
#define AES_H

#define AES_BLOCK_SIZE 16

unsigned char* pkcs7_pad(unsigned char* plaintext, int plaintext_len, int block_len, int* padded_lenp);
unsigned char* pkcs7_unpad(unsigned char* padded_plaintext, int padded_len, int* plaintext_lenp);

int aes_block_encrypt(unsigned char* plaintext, unsigned char* key, unsigned char* ciphertext);
int aes_block_decrypt(unsigned char *ciphertext, unsigned char* key, unsigned char *plaintext);

int aes_128_ecb_encrypt(unsigned char* plaintext, int plaintext_len, unsigned char* key, unsigned char* iv, unsigned char* ciphertext);
int aes_128_ecb_decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char* key, unsigned char* iv, unsigned char* plaintext);

int aes_128_cbc_encrypt(unsigned char* plaintext, int plaintext_len, unsigned char* key, unsigned char* iv, unsigned char* ciphertext);
int aes_128_cbc_decrypt(unsigned char* ciphertext, int ciphertext_len, unsigned char* key, unsigned char* iv, unsigned char* plaintext);

void rand_bytes(unsigned char* bytes, size_t len);
int randint(int low, int high);
int aes_128_rand_encrypt(unsigned char* plaintext, int plaintext_len, unsigned char* ciphertext, int* mode);

#endif

