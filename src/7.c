#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bytes.h"
#include "aes.h"

int main(){
	printf("Challenge 7:\n");
	FILE* fp = fopen("challenge_7.txt", "r");
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
	size_t byteslen;
	unsigned char* bytes = base64_to_bytes(base64, base64len, &byteslen);
	unsigned char* key = (unsigned char*)"YELLOW SUBMARINE";
	unsigned char msg[byteslen+127];
	int msglen = aes_128_ecb_decrypt(bytes, byteslen, key, NULL, msg);
	msg[msglen] = '\0';
	printf("KEY: %s\n", key);
	printf("MSG: %s\n", msg);
	free(bytes);
	free(base64);
	return 0;
}

