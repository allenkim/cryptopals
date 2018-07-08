#ifndef ORACLES_H
#define ORACLES_H

// Oracle for challenge 11
int encrypt_oracle_11(unsigned char* plaintext, int plaintext_len, unsigned char* ciphertext, int* mode);

// Oracle for challenge 12
int encrypt_oracle_12(unsigned char* plaintext, int plaintext_len, unsigned char* ciphertext);

// Oracle for challenge 13
int profile_for(char* email, unsigned char* ciphertext);
int profile_dec(unsigned char* ciphertext, int ciphertext_len, char* profile);

#endif

