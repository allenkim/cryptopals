#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char base64table[64] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
                        	  'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
                        	  'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
                        	  'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
                        	  'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
                        	  'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
                        	  'w', 'x', 'y', 'z', '0', '1', '2', '3',
                        	  '4', '5', '6', '7', '8', '9', '+', '/'};


unsigned char char2hexval(char c){
	if (c >= '0' && c <= '9')
		return c - '0';
	if (c >= 'a' && c <= 'f')
		return c - 'a' + 10;
	fprintf(stderr, "Invalid char '%c' for hex\n", c);
	exit(EXIT_FAILURE);
}

unsigned char hex2byte(char* hexbyte, size_t len){
	if (len < 1 || len > 2){
		fprintf(stderr, "Invalid len '%zu' for hex2byte\n", len);
		exit(EXIT_FAILURE);
	}
	unsigned char byte = char2hexval(hexbyte[0]);
	if (len == 1)
		return byte;
	byte <<= 4;
	byte += char2hexval(hexbyte[1]);
	return byte;
}

unsigned char* hex2bytes(char* hex, size_t hexsize, size_t* byteslenp){
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
		bytes[0] = hex2byte(hex,1);
	}
	else{
		bytes = (unsigned char*)malloc(byteslen*sizeof(unsigned char));
		if (!bytes){
			perror("Failure to malloc in hex2bytes");
			exit(EXIT_FAILURE);
		}
	}
	for (int i = odd; i < hexsize; i += 2){
		bytes[i/2+odd] = hex2byte(hex+i,2);
	}
	*byteslenp = byteslen;
	return bytes;
}

char* bytes2base64(unsigned char* bytes, size_t byteslen, size_t* base64lenp){
	size_t base64len = (byteslen * 4 + 2) / 3;
	char* base64 = (char*)malloc(base64len*sizeof(char));
	int state = 0;
	int i64 = base64len - 1;
	unsigned char curr, left;
	for (int i = byteslen - 1; i >= 0; i--){
		switch (state){
			case 0:
				curr = bytes[i] & 0x3f;
				base64[i64--] = base64table[curr];
				left = bytes[i] >> 6;
				state = 1;
				break;
			case 1:
				curr = bytes[i] & 0x0f;
				curr <<= 2;
				curr |= left;
				base64[i64--] = base64table[curr];
				left = bytes[i] >> 4;
				state = 2;
				break;
			case 2:
				curr = bytes[i] & 0x03;
				curr <<= 4;
				curr |= left;
				base64[i64--] = base64table[curr];
				base64[i64--] = base64table[bytes[i]>>2];
				left = 0;
				state = 0;
				break;
		}
	}
	*base64lenp = base64len;
	return base64;
}

int main(int argc, char* argv[]){
	char* hex = "49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d";
	size_t hexsize = strlen(hex);
	size_t byteslen;
	unsigned char* bytes = hex2bytes(hex, hexsize, &byteslen);
	for (int i = 0; i < byteslen; i++){
		printf("%02x",bytes[i]);
	}
	printf("\n");
	size_t base64len;
	char* base64 = bytes2base64(bytes, byteslen, &base64len);
	printf("%s\n", base64);
	free(bytes);
	free(base64);
	return 0;
}

