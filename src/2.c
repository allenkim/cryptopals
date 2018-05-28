#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "xor.h"
#include "base.h"

int main(){
	printf("Challenge 2:\n");
	char* hex1 = "1c0111001f010100061a024b53535009181c";
	size_t hex1size = strlen(hex1);
	size_t bytes1len;
	unsigned char* bytes1 = hex_to_bytes(hex1, hex1size, &bytes1len);
	char* hex2 = "686974207468652062756c6c277320657965";
	size_t hex2size = strlen(hex2);
	size_t bytes2len;
	unsigned char* bytes2 = hex_to_bytes(hex2, hex2size, &bytes2len);
	unsigned char* xorbytes_arr = xorbytes(bytes1, bytes2, bytes1len);
	printf("IN1: %s\nIN2: %s\nXOR: ", hex1, hex2);
	for (int i = 0; i < bytes1len; i++){
		printf("%02x",xorbytes_arr[i]);
	}
	printf("\n\n");
	free(bytes1);
	free(bytes2);
	free(xorbytes_arr);
	return 0;
}



