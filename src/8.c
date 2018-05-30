#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bytes.h"
#include "hash_table.h"

int main(){
	printf("Challenge 8:\n");
	FILE* fp = fopen("challenge_8.txt", "r");
	char* hex = (char*)malloc(322); // new line and \0
	char* top_hex = (char*)malloc(322);
	int max_freq = 1;
	while (321 == fread(hex, 1, 321, fp)){
		hex[320] = 0;
		size_t byteslen; // should be 160
		unsigned char* bytes = hex_to_bytes(hex, 320, &byteslen);
		int freq = max_freq_bytes(bytes, byteslen, 16);
		if (freq > max_freq){
			max_freq = freq;
			strcpy(top_hex, hex);
		}
		free(bytes);
	}
	printf("%s\n", top_hex);
	
	free(top_hex);
	free(hex);
	fclose(fp);
	return 0;
}

