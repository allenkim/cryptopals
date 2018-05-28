#ifndef AES_H
#define AES_H

int aes_128_ecb_encrypt(unsigned char* plaintext, int plaintext_len, unsigned char* key, unsigned char* iv, unsigned char* ciphertext);
int aes_128_ecb_decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *key, unsigned char *iv, unsigned char *plaintext);
unsigned char* pkcs7_pad(unsigned char* plaintext, int plaintext_len, int block_len, int* padded_len);

#endif

