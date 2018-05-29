#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "xor.h"
#include "bytes.h"

int main(){
	init_letter_freq();
	printf("Challenge 4:\n");
	FILE* fp;
	char hex[62], top_hex[62];
	unsigned char* top_top = NULL;
	unsigned char top_key = 0;
	float top_score = 1000.0;
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
		float score = score_single_byte_xor(bytes, byteslen, &top, &key);
		if (score < top_score){
			memcpy(top_hex, hex, hexsize);
			top_score = score;
			if (top_top)
				free(top_top);
			top_top = top;
			top_key = key;
		}
		free(bytes);
	}
	printf("Key: %c\n", top_key);
	printf("%s\n", top_top);
	free(top_top);
	fclose(fp);
	return 0;
}

