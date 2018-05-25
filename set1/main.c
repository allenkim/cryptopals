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

unsigned char* xor(unsigned char* bytes1, unsigned char* bytes2, size_t len){
	unsigned char* xorbytes = (unsigned char*)malloc(len*sizeof(unsigned char));
	for (int i = 0; i < len; i++){
		xorbytes[i] = bytes1[i] ^ bytes2[i];
	}
	return xorbytes;
}

void challenge_1(){
	printf("Challenge 1:\n");
	char* hex = "49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d";
	size_t hexsize = strlen(hex);
	size_t byteslen;
	unsigned char* bytes = hex2bytes(hex, hexsize, &byteslen);
	for (int i = 0; i < byteslen; i++){
		printf("%02x",bytes[i]);
	}
	printf("\n");
	char* answer = "SSdtIGtpbGxpbmcgeW91ciBicmFpbiBsaWtlIGEgcG9pc29ub3VzIG11c2hyb29t";
	printf("Expected: %s\n", answer);
	size_t base64len;
	char* base64 = bytes2base64(bytes, byteslen, &base64len);
	printf("Received: %s\n", base64);
	free(bytes);
	free(base64);
	printf("\n");
}

void challenge_2(){
	printf("Challenge 2:\n");
	char* hex1 = "1c0111001f010100061a024b53535009181c";
	size_t hex1size = strlen(hex1);
	size_t bytes1len;
	unsigned char* bytes1 = hex2bytes(hex1, hex1size, &bytes1len);
	char* hex2 = "686974207468652062756c6c277320657965";
	size_t hex2size = strlen(hex2);
	size_t bytes2len;
	unsigned char* bytes2 = hex2bytes(hex2, hex2size, &bytes2len);
	unsigned char* xorbytes = xor(bytes1, bytes2, bytes1len);
	printf("IN1: %s\nIN2: %s\nXOR: ", hex1, hex2);
	for (int i = 0; i < bytes1len; i++){
		printf("%02x",xorbytes[i]);
	}
	printf("\n\n");
	free(bytes1);
	free(bytes2);
	free(xorbytes);
}

unsigned char* single_byte_xor(unsigned char* bytes, unsigned char byte, size_t len){
	unsigned char* xorbytes = (unsigned char*)malloc(len*sizeof(unsigned char));
	unsigned char* charbytes = (unsigned char*)malloc(len*sizeof(unsigned char));
	for (int i = 0; i < len; i++)
		charbytes[i] = byte;
	xorbytes = xor(bytes, charbytes, len);
	free(charbytes);
	return xorbytes;
}

int score_plaintext(unsigned char* bytes, size_t len){
	int score = 0;
	for (int i = 0; i < len; i++){
		char c = bytes[i];
		if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == ' ')
			score++;
		else if ((c > 13 && c < 32) || c > 127)
			score -= 10;
		else
			score--;
	}
	return score;
}

int score_single_byte_xor(unsigned char* bytes, size_t len, unsigned char** topp,  unsigned char* byte){
	unsigned char* top = NULL;
	int top_score = 0;
	unsigned char key = 0;
	for (unsigned char i = 0; i < 255; i++){
		unsigned char* xorbytes = single_byte_xor(bytes, i, len);
		int score = score_plaintext(xorbytes, len);
		if (score > top_score){
			top_score = score;
			top = xorbytes;
			key = i;
		}
		else{
			free(xorbytes);
		}
	}
	*topp = top;
	*byte = key;
	return top_score;
}

void challenge_3(){
	printf("Challenge 3:\n");
	char* hex = "1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736";
	size_t hexsize = strlen(hex);
	size_t byteslen;
	unsigned char* bytes = hex2bytes(hex, hexsize, &byteslen);
	unsigned char* top;
	unsigned char key;
	score_single_byte_xor(bytes, byteslen, &top, &key);
	printf("Key: %c\n", key);
	printf("%s\n\n", top);
	free(bytes);
}

void challenge_4(){
	printf("Challenge 4:\n");
	FILE* fp;
	char hex[62], top_hex[62];
	unsigned char* top_top = NULL;
	unsigned char top_key = 0;
	int top_score = 0;
	fp = fopen("challenge_4.txt", "r");
	while (1){
		if (feof(fp))
			break;
		fgets(hex, 62, fp);
		size_t hexsize = strlen(hex) - 1;
		size_t byteslen;
		unsigned char* bytes = hex2bytes(hex, hexsize, &byteslen);
		unsigned char* top;
		unsigned char key;
		int score = score_single_byte_xor(bytes, byteslen, &top, &key);
		if (score > top_score){
			memcpy(top_hex, hex, hexsize);
			top_score = score;
			if (top_top)
				free(top_top);
			top_top = top;
			top_key = key;
		}
	}
	printf("Key: %c\n", top_key);
	printf("%s\n\n", top_top);
	free(top_top);
	fclose(fp);
}

unsigned char* repeating_key_xor(unsigned char* bytes, unsigned char* key, size_t len, size_t key_len){
	unsigned char* xorbytes = (unsigned char*)malloc(len*sizeof(unsigned char));
	unsigned char* charbytes = (unsigned char*)malloc(len*sizeof(unsigned char));
	for (int i = 0; i < len; i++){
		charbytes[i] = key[i % key_len];
	}
	xorbytes = xor(bytes, charbytes, len);
	free(charbytes);
	return xorbytes;
}

void challenge_5(){
	printf("Challenge 5:\n");
	unsigned char key[3] = "ICE";

	char* str = "Burning 'em, if you ain't quick and nimble\nI go crazy when I hear a cymbal";
	size_t len = strlen(str);

	unsigned char* cipher = repeating_key_xor((unsigned char*)str, key, len, 3);
	for (int i = 0; i < len; i++){
		printf("%02x",cipher[i]);
	}
	printf("\n\n");
	free(cipher);
}

size_t hamming_dist(unsigned char* str1, unsigned char* str2, size_t len){
	size_t dist = 0;
	for (int i = 0; i < len; i++){
		unsigned char xorbyte = str1[i] ^ str2[i];
		while (xorbyte){
			xorbyte &= (xorbyte - 1);
			dist++;
		}
	}
	return dist;
}

void challenge_6(){
	printf("Challenge 6:\n");
	FILE* fp;
	fp = fopen("challenge_6.txt", "r");
	fseek(fp, 0, SEEK_END);
	int filelen = ftell(fp);
	rewind(fp);
	char* buff = (char*)malloc((filelen+1)*sizeof(char));
	fread(buff, filelen, 1, fp);
	fclose(fp);
	printf("%s\n", buff);
	free(buff);
}


int main(int argc, char* argv[]){
	/*challenge_1();
	challenge_2();
	challenge_3();
	challenge_4();
	challenge_5();*/
	challenge_6();
	return 0;
}

