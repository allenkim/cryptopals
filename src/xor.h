#ifndef XOR_H
#define XOR_H
#include <stddef.h>

void init_letter_freq();
unsigned char* xorbytes(unsigned char* bytes1, unsigned char* bytes2, size_t len);
unsigned char* single_byte_xor(unsigned char* bytes, unsigned char byte, size_t len);
float score_plaintext(unsigned char* bytes, size_t len);
float score_single_byte_xor(unsigned char* bytes, size_t len, unsigned char** topp,  unsigned char* byte);
unsigned char* repeating_key_xor(unsigned char* bytes, unsigned char* key, size_t len, size_t key_len);
size_t hamming_dist(unsigned char* str1, unsigned char* str2, size_t len);
float score_keysize(unsigned char* bytes, size_t len, size_t keysize);
size_t* find_best_keysizes(unsigned char* bytes, size_t len, size_t max);
unsigned char* transpose_bytes(unsigned char* bytes, size_t len, size_t keysize, size_t** tranpose_lens);
unsigned char* find_repeating_xor_key(unsigned char* bytes, size_t len, size_t keysize);
char* break_repeating_key_xor(unsigned char* bytes, size_t byteslen, unsigned char** keyp, size_t* keylenp);

size_t vigenere_encrypt_file(const char* fnout, const char* fnin, unsigned char* key);
size_t vigenere_decrypt_file(const char* fnout, const char* fnin);

#endif

