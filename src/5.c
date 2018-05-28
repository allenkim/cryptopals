#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "xor.h"
#include "base.h"

int main(){
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
	return 0;
}

