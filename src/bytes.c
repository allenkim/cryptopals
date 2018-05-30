#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "hash_table.h"
#include "bytes.h"

const char base64table[64] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
                        	  'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
                        	  'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
                        	  'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
                        	  'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
                        	  'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
                        	  'w', 'x', 'y', 'z', '0', '1', '2', '3',
                        	  '4', '5', '6', '7', '8', '9', '+', '/'};


unsigned char char_to_hexval(char c){
	if (c >= '0' && c <= '9')
		return c - '0';
	if (c >= 'a' && c <= 'f')
		return c - 'a' + 10;
	fprintf(stderr, "Invalid char '%c' for hex\n", c);
	exit(EXIT_FAILURE);
}

unsigned char char_to_base64val(char c){
	if (c >= 'A' && c <= 'Z')
		return c - 'A';
	if (c >= 'a' && c <= 'z')
		return c - 'a' + 26;
	if (c >= '0' && c <= '9')
		return c - '0' + 52;
	if (c == '+')
		return 62;
	if (c == '/')
		return 63;
	if (c == '=')
		return 0;
	fprintf(stderr, "Invalid char '%c' for hex\n", c);
	exit(EXIT_FAILURE);
}


void print_bytes(unsigned char* bytes, size_t byteslen){
	for (int i = 0; i < byteslen; i++){
		printf("%02x", bytes[i]);
	}
	printf("\n");
}

unsigned char hex_to_byte(char* hexbyte, size_t len){
	if (len < 1 || len > 2){
		fprintf(stderr, "Invalid len '%zu' for hex2byte\n", len);
		exit(EXIT_FAILURE);
	}
	unsigned char byte = char_to_hexval(hexbyte[0]);
	if (len == 1)
		return byte;
	byte <<= 4;
	byte += char_to_hexval(hexbyte[1]);
	return byte;
}

unsigned char* hex_to_bytes(char* hex, size_t hexsize, size_t* byteslenp){
	if (hexsize <= 0){
		fprintf(stderr, "Invalid len '%zu' for hex2bytes\n", hexsize);
		exit(EXIT_FAILURE);
	}
	size_t byteslen = hexsize / 2;
	size_t odd = hexsize & 1;
	unsigned char* bytes;
	if (odd){
		byteslen++;
		bytes = (unsigned char*)malloc(byteslen*sizeof(unsigned char));
		if (!bytes){
			perror("Failure to malloc in hex2bytes");
			exit(EXIT_FAILURE);
		}
		bytes[0] = hex_to_byte(hex,1);
	}
	else{
		bytes = (unsigned char*)malloc(byteslen*sizeof(unsigned char));
		if (!bytes){
			perror("Failure to malloc in hex2bytes");
			exit(EXIT_FAILURE);
		}
	}
	for (int i = odd; i < hexsize; i += 2){
		bytes[i/2+odd] = hex_to_byte(hex+i,2);
	}
	*byteslenp = byteslen;
	return bytes;
}

char* bytes_to_base64(unsigned char* bytes, size_t byteslen, size_t* base64lenp){
	size_t base64len = 4*((byteslen+2) / 3);
	char* base64 = (char*)malloc((base64len+1)*sizeof(char));
	int state = 0;
	int i64 = 0;
	unsigned char curr, left;
	for (int i = 0; i < byteslen; i++){
		switch (state){
			case 0:
				curr = (bytes[i] & 0xfc) >> 2;
				base64[i64++] = base64table[curr];
				left = bytes[i] << 6;
				state = 1;
				break;
			case 1:
				curr = bytes[i] & 0xf0;
				curr >>= 2;
				curr |= left;
				curr >>= 2;
				base64[i64++] = base64table[curr];
				left = bytes[i] << 4;
				state = 2;
				break;
			case 2:
				curr = bytes[i] & 0xc0;
				curr >>= 4;
				curr |= left;
				curr >>= 2;
				base64[i64++] = base64table[curr];
				base64[i64++] = base64table[bytes[i] & 0x3f];
				left = 0;
				state = 0;
				break;
		}
	}
	switch (state){
		case 1:
			base64[i64++] = base64table[left >> 2];
			base64[i64++] = '=';
			base64[i64++] = '=';
			break;
		case 2:
			base64[i64++] = base64table[left >> 2];
			base64[i64++] = '=';
			break;
	}
	base64[base64len] = 0;
	*base64lenp = base64len;
	return base64;
}

unsigned char* base64_to_bytes(char* base64str, size_t base64strlen, size_t* byteslenp){
	size_t byteslen = (base64strlen/4)*3;
	unsigned char* bytes = (unsigned char*)malloc(byteslen+1*sizeof(unsigned char));
	int state = 0;
	int i64 = 0;
	unsigned char curr, left;
	for (int i = 0; i < byteslen; i++){
		curr = char_to_base64val(base64str[i64++]);
		switch (state){
			case 0:
				bytes[i] = curr;
				bytes[i] <<= 2;
				curr = char_to_base64val(base64str[i64++]);
				left = curr;
				curr >>= 4;
				bytes[i] |= curr;
				left <<= 4;
				state = 1;
				break;
			case 1:
				bytes[i] = left;
				left = curr;
				curr >>= 2;
				bytes[i] |= curr;
				left <<= 6;
				state = 2;
				break;
			case 2:
				bytes[i] = left;
				bytes[i] |= curr;
				left = 0;
				state = 0;
				break;
		}
	}
	bytes[byteslen] = 0;
	if (base64str[base64strlen-1] == '='){
		byteslen--;
	}
	if (base64str[base64strlen-2] == '='){
		byteslen--;
	}
	*byteslenp = byteslen;
	return bytes;
}

char* hex_to_base64(char* hex, size_t hexsize, size_t* base64lenp){
	size_t byteslen;
	unsigned char* bytes = hex_to_bytes(hex, hexsize, &byteslen);
	size_t base64len;
	char* base64 = bytes_to_base64(bytes, byteslen, &base64len);
	free(bytes);
	*base64lenp = base64len;
	return base64;
}

size_t max_freq_bytes(unsigned char* bytes, size_t byteslen, size_t block_size){
	unsigned char top_key[block_size];
	unsigned char key[block_size];
	size_t max_freq = 1;
	ht_hash_table* ht = ht_new();
	for (int i = 0; i < byteslen; i += block_size){
		memcpy(key, bytes+i, block_size);
		int freq = 1;
		bool found = ht_search(ht, key, block_size, &freq);
		if (found){
			if (++freq > max_freq){
				max_freq = freq;
				memcpy(top_key, key, block_size);
			}
		}
		ht_insert(ht, key, block_size, freq);
	}
	ht_del_hash_table(ht);
	return max_freq;
}

