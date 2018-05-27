#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "xor.h"
#include "base.h"
#include "challenge.h"

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

void challenge_1(){
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
}

void challenge_2(){
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
}

void challenge_3(){
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
}

void challenge_4(){
	printf("Challenge 4:\n");
	FILE* fp;
	char hex[62], top_hex[62];
	unsigned char* top_top = NULL;
	unsigned char top_key = 0;
	int top_score = 0;
	fp = fopen("challenge_4.txt", "r");
	while (1){
		if (feof(fp))
			break;
		fgets(hex, 62, fp);
		size_t hexsize = strlen(hex) - 1;
		size_t byteslen;
		unsigned char* bytes = hex_to_bytes(hex, hexsize, &byteslen);
		unsigned char* top;
		unsigned char key;
		int score = score_single_byte_xor(bytes, byteslen, &top, &key);
		if (score > top_score){
			memcpy(top_hex, hex, hexsize);
			top_score = score;
			if (top_top)
				free(top_top);
			top_top = top;
			top_key = key;
		}
	}
	printf("Key: %c\n", top_key);
	printf("%s\n\n", top_top);
	free(top_top);
	fclose(fp);
}

void challenge_5(){
	printf("Challenge 5:\n");
	unsigned char key[3] = "ICE";

	char* str = "Burning 'em, if you ain't quick and nimble\nI go crazy when I hear a cymbal";
	printf("M: %s\n", str);
	size_t len = strlen(str);

	printf("Expected: 0b3637272a2b2e63622c2e69692a23693a2a3c6324202d623d63343c2a26226324272765272a282b2f20430a652e2c652a3124333a653e2b2027630c692b20283165286326302e27282f\nReceived: ");
	unsigned char* cipher = repeating_key_xor((unsigned char*)str, key, len, 3);
	for (int i = 0; i < len; i++){
		printf("%02x",cipher[i]);
	}
	printf("\n\n");
	free(cipher);
}

void challenge_6(){
	printf("Challenge 6:\n");
	FILE* fp = fopen("challenge_6.txt", "r");
	fseek(fp, 0, SEEK_END);
	int filelen = ftell(fp);
	rewind(fp);
	char* base64 = (char*)malloc((filelen+1)*sizeof(char));
	fread(base64, filelen, sizeof(char), fp);
	fclose(fp);
	/*
	char *pos;
	if ((pos=strchr(base64, '\n')) != NULL)
    	*pos = 0;*/
	base64[filelen] = 0;
	size_t base64len = strlen(base64);
	size_t byteslen;
	unsigned char* bytes = base64_to_bytes(base64, base64len, &byteslen);
	size_t MAX_KEYSIZE = 40;
	size_t* keysizes = find_best_keysizes(bytes, byteslen, MAX_KEYSIZE);
	unsigned char* key, *msg;
	int i;
	for (i = 0; i < MAX_KEYSIZE; i++){
		printf("KEYSIZE: %zu\n", keysizes[i]);
		key = find_repeating_xor_key(bytes, byteslen, keysizes[i]);
		if (key)
			break;
	}
	msg = repeating_key_xor(bytes, key, byteslen, keysizes[i]);
	printf("KEY: %s\n", key);
	printf("MSG: %s\n", msg);
	free(key);
	free(msg);
	free(keysizes);
	free(base64);
}

