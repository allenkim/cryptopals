#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "bytes.h"
#include "aes.h"
#include "oracles.h"
#include "hash_table.h"

int main(){
	char email[] = "foo@bar.com";
	unsigned char enc_prof[25+strlen(email)+AES_BLOCK_SIZE];
	int enc_prof_len = profile_for(email, enc_prof);
	print_bytes(enc_prof, enc_prof_len);

	char dec_prof[25+strlen(email)];
	profile_dec(enc_prof, enc_prof_len, dec_prof);
	printf("%s\n", dec_prof);

	return 0;
}

