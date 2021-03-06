#ifndef BYTES_H
#define BYTES_H
#include <stddef.h>

unsigned char char_to_hexval(char c);
unsigned char hex_to_byte(char* hexbyte, size_t len);
unsigned char* hex_to_bytes(char* hex, size_t hexsize, size_t* byteslenp);
void print_bytes(unsigned char* bytes, size_t byteslen);
void print_chars(unsigned char* bytes, size_t byteslen);
char* bytes_to_base64(unsigned char* bytes, size_t byteslen, size_t* base64lenp);
unsigned char* base64_to_bytes(char* base64str, size_t base64strlen, size_t* byteslenp);
char* hex_to_base64(char* hex, size_t hexsize, size_t* base64lenp);
size_t max_freq_bytes(unsigned char* bytes, size_t byteslen, size_t block_size);
void set_repeat_bytes(unsigned char* bytes, size_t byteslen, unsigned char byte);

#endif

