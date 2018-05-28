#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "xor.h"
#include "base.h"

int main(){
	init_letter_freq();
	printf("Challenge 6:\n");
	FILE* fp = fopen("challenge_6.txt", "r");
	fseek(fp, 0, SEEK_END);
	int filelen = ftell(fp);
	rewind(fp);
	char* base64 = (char*)malloc((filelen+1)*sizeof(char));
	fread(base64, filelen, sizeof(char), fp);
	fclose(fp);
	char *pos;
	if ((pos=strchr(base64, '\n')) != NULL)
    	*pos = 0;
	base64[filelen] = 0;
	size_t base64len = strlen(base64);
	size_t byteslen;
	unsigned char* bytes = base64_to_bytes(base64, base64len, &byteslen);
	unsigned char* key;
	char* msg;
	size_t keylen;
	msg = break_repeating_key_xor(bytes, byteslen, &key, &keylen);
	printf("KEYSIZE: %zu\n", keylen);
	printf("KEY: %s\n", key);
	printf("MSG: %s\n", msg);
	free(bytes);
	free(key);
	free(msg);
	free(base64);
	return 0;
}

