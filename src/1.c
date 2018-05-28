#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "xor.h"
#include "base.h"

void test_base64_to_str(char* base64, char* answer){
	size_t base64size = strlen(base64);
	printf("%s\n", base64);
	printf("Expected: %s\n", answer);
	size_t byteslen;
	char* bytes = (char*)base64_to_bytes(base64, base64size, &byteslen);
	printf("Received: %s\n", bytes);
	free(bytes);
}

void test_str_to_base64(char* str, char* answer){
	size_t strsize = strlen(str);
	printf("%s\n", str);
	printf("Expected: %s\n", answer);
	size_t base64len;
	char* base64 = bytes_to_base64((unsigned char*)str, strsize, &base64len);
	printf("Received: %s\n", base64);
	free(base64);
}

void test_hex_to_base64(char* hex, char* answer){
	size_t hexsize = strlen(hex);
	printf("%s\n", hex);
	printf("Expected: %s\n", answer);
	size_t base64len;
	char* base64 = hex_to_base64(hex, hexsize, &base64len);
	printf("Received: %s\n", base64);
	free(base64);
}

int main(){
	printf("Challenge 1:\n");
	char* hex = "49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d";
	char* answer = "SSdtIGtpbGxpbmcgeW91ciBicmFpbiBsaWtlIGEgcG9pc29ub3VzIG11c2hyb29t";
	test_hex_to_base64(hex, answer);
	/*
	char* str = "any carnal pleasure.";
	answer = "YW55IGNhcm5hbCBwbGVhc3VyZS4=";
	test_str_to_base64(str, answer);
	test_base64_to_str(answer,str);
	str = "any carnal pleasure";
	answer = "YW55IGNhcm5hbCBwbGVhc3VyZQ==";
	test_str_to_base64(str, answer);
	test_base64_to_str(answer,str);
	str = "any carnal pleasur";
	answer = "YW55IGNhcm5hbCBwbGVhc3Vy";
	test_str_to_base64(str, answer);
	test_base64_to_str(answer,str);
	*/
	printf("\n");
	return 0;
}



