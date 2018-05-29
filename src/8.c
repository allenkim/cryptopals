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
	unsigned char top_key[16];
	int max_rep = 1;
	while (321 == fread(hex, 1, 321, fp)){
		hex[320] = 0;
		size_t byteslen; // should be 160
		unsigned char* bytes = hex_to_bytes(hex, 320, &byteslen);
		ht_hash_table* ht = ht_new(17);
		unsigned char key[16];
		for (int i = 0; i < 160; i+=16){
			memcpy(key, bytes+i, 16);
			int freq = 1;
			bool found = ht_search(ht, key, 16, &freq);
			if (found){
				if (++freq > max_rep){
					max_rep = freq;
					memcpy(top_key, key, 16);
					strcpy(top_hex, hex);
				}
			}
			ht_insert(ht, key, 16, freq);
		}
		ht_del_hash_table(ht);
		free(bytes);
	}
	printf("BLOCK: ");
	print_bytes(top_key, 16);
	printf("Repeats %d times!\n%s\n", max_rep, top_hex);
	
	free(top_hex);
	free(hex);
	fclose(fp);
	return 0;
}

