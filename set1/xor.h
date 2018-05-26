#ifndef XOR_H
#define XOR_H
#include <stddef.h>

unsigned char* xorbytes(unsigned char* bytes1, unsigned char* bytes2, size_t len);
unsigned char* single_byte_xor(unsigned char* bytes, unsigned char byte, size_t len);
int score_plaintext(unsigned char* bytes, size_t len);
int score_single_byte_xor(unsigned char* bytes, size_t len, unsigned char** topp,  unsigned char* byte);
unsigned char* repeating_key_xor(unsigned char* bytes, unsigned char* key, size_t len, size_t key_len);
size_t hamming_dist(unsigned char* str1, unsigned char* str2, size_t len);

#endif

