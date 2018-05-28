#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "xor.h"
#include "base.h"

int main(){
	init_letter_freq();
	printf("Challenge 3:\n");
	char* hex = "1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736";
	size_t hexsize = strlen(hex);
	size_t byteslen;
	unsigned char* bytes = hex_to_bytes(hex, hexsize, &byteslen);
	unsigned char* top;
	unsigned char key;
	score_single_byte_xor(bytes, byteslen, &top, &key);
	printf("Key: %c\n", key);
	printf("%s\n\n", top);
	free(bytes);
	return 0;
}

