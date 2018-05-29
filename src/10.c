#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "base.h"
#include "aes.h"

void test_identity(){
	unsigned char* key = (unsigned char*)"YELLOW SUBMARINE";
	unsigned char*  iv = (unsigned char*)"\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0";
	unsigned char*  pt = (unsigned char*)"I'm back and I'm ringin' the be";
	size_t ptlen = strlen((char*)pt);
	unsigned char ct[ptlen+AES_BLOCK_SIZE];
	unsigned char msg[ptlen+AES_BLOCK_SIZE];
	int ctlen = aes_128_cbc_encrypt(pt, ptlen, key, iv, ct);
	print_bytes(ct, ctlen);
	int msglen = aes_128_cbc_decrypt(ct, ctlen, key, iv, msg);
	msg[msglen] = '\0';
	printf("KEY: %s\n", key);
	printf("OPT: %s\n", pt);
	printf("MSG: %s\n", msg);

}

int main(){
	//test_identity();
	printf("Challenge 10:\n");
	FILE* fp = fopen("challenge_10.txt", "r");
	fseek(fp, 0, SEEK_END);
	int filelen = ftell(fp);
	rewind(fp);
	char* base64 = (char*)malloc((filelen+1)*sizeof(char));
	fread(base64, filelen, sizeof(char), fp);
	fclose(fp);
	char *pos;
	if ((pos=strchr(base64, '\n')) != NULL){
    	*pos = 0;
	}
	base64[filelen] = 0;
	size_t base64len = strlen(base64);
	size_t cipherlen;
	unsigned char* cipher = base64_to_bytes(base64, base64len, &cipherlen);
	unsigned char* key = (unsigned char*)"YELLOW SUBMARINE";
	unsigned char*  iv = (unsigned char*)"\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0";
	unsigned char msg[cipherlen+AES_BLOCK_SIZE];
	int msglen = aes_128_cbc_decrypt(cipher, cipherlen, key, iv, msg);
	msg[msglen] = '\0';
	printf("%s\n", msg);
	free(cipher);
	free(base64);
	return 0;
}

