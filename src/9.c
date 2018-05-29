#include <stdio.h>
#include <stdlib.h>
#include "aes.h"
#include "bytes.h"

int main(){
	printf("Challenge 9:\n");
	char* str = "YELLOW SUBMARINE";
	printf("Expected: ");
	for (int i = 0; i < 16; i++)
		printf("%02x", str[i]);
	for (int i = 0; i < 4; i++)
		printf("04");
	printf("\n");
	int padded_len;
	unsigned char* padded_str = pkcs7_pad((unsigned char*)str, 16, 20, &padded_len);
	printf("Received: ");
	for (int i = 0; i < padded_len; i++)
		printf("%02x", padded_str[i]);
	printf("\n");
	return 0;
}



