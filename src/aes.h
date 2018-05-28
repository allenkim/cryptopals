#ifndef AES_H
#define AES_H

#define AES_BLOCK_SIZE 16

int aes_128_ecb_encrypt(unsigned char* plaintext, int plaintext_len, unsigned char* key, unsigned char* iv, unsigned char* ciphertext);
int aes_128_ecb_decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *key, unsigned char *iv, unsigned char *plaintext);
int aes_128_cbc_encrypt(unsigned char* plaintext, int plaintext_len, unsigned char* key, unsigned char* iv, unsigned char* ciphertext);
int aes_128_cbc_decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *key, unsigned char *iv, unsigned char *plaintext);
unsigned char* pkcs7_pad(unsigned char* plaintext, int plaintext_len, int block_len, int* padded_lenp);
unsigned char* pkcs7_unpad(unsigned char* padded_plaintext, int padded_len, int* plaintext_lenp);

#endif

