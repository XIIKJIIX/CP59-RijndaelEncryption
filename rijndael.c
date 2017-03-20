#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "utility.h"
#include "base64.h"
#define MIN_ARGC 6

int displayHelp() {
	printf("Usage (Text Mode): ./rijndael -m [plaintext|base64Encrypted Message] -p [password] [--encrypt|--decrypt]\n");
	printf("	For special character use \\ before. Ex. space use '\\ '.\n");
	printf("Usage (File Mode): ./rijndael -f [/path/to/file] -p [password] [--encrypt|--decrypt]\n");
	printf("	The output encrypted file will be named [filename].[type].encrypted\n");
	printf("REMARK: MAX length of PASSWORD is 16 characters.\n");
	return 0;
}

int fileMode(char *filepath, unsigned char *key, int is_encrypt) {

	return 0;
}

int textMode(unsigned char *message, unsigned char *key, int is_encrypt) {
	int length = strlen((const char*)message);
	//Initialize key schedule
	unsigned char expandedKey[176];
	KeyExpansion(key, expandedKey);

	//Decrypt
	if (!is_encrypt) {
		int decodedLength = Base64decode_len((const char*)message);
		decodedLength = (int)(decodedLength/16)*16; //Correct the actual length of decoded
		printf("Encoded Encrypted Message: %s (len:%d)\nPassword: %s (len:%lu)\n", message, length, key, strlen((const char*)key));
		printf("Decoded length: %d\n", decodedLength);

		char *decoded = (char*)calloc(length, sizeof(char));
		Base64decode(decoded, (const char*)message);

		for (int i = 0; i < decodedLength; i+=16)
			Decrypt((unsigned char*)decoded+i, expandedKey);
 
		printf("Decrypted Message: %s\n", decoded);
		return 0;
	}

	printf("Message: %s (len:%d)\nPassword: %s (len:%lu)\n", message, length, key, strlen((const char*)key));
	//Padding For plaintext
	int paddedLength = length;
	if (length % 16 != 0)
		paddedLength = (length/16+1)*16;

	unsigned char *paddedMessage = (unsigned char*)calloc(paddedLength, sizeof(unsigned char));
	for (int i = 0; i < paddedLength; i++)
		if (i < length)
			paddedMessage[i] = message[i];
		else
			break;
	for (int i = 0; i < paddedLength; i+=16)
		Encrypt(paddedMessage+i, expandedKey); //paddedMessage has been encrypted

	char *encoded = (char*)calloc(paddedLength, sizeof(char));
	Base64encode(encoded, (const char*)paddedMessage, paddedLength);

	printf("Base64 encoded message: %s\n", encoded);
	return 0;
}

int main(int argc, char const *argv[]) {
	unsigned char *message = NULL;
	unsigned char key[17];
	char *filepath = NULL;
	int is_encrypt = -1;
	
	if (argc < MIN_ARGC) return displayHelp();
	
	for (int i = 1; i < (argc); i++) {
		if (strcmp("-m", argv[i]) == 0) {
			message = (unsigned char *)argv[++i];
			continue;
		}
		if (strcmp("-p", argv[i]) == 0) {
			if (strlen(argv[i+1]) > 16) return displayHelp();
			strcpy((char*)key, (const char*)argv[++i]);
			continue;
		}
		if (strcmp("-f", argv[i]) == 0) {
			filepath = (char*)argv[++i];
		}
		if (strcmp("--encrypt", argv[i]) == 0) {
			is_encrypt = 1;
			continue;
		}
		if (strcmp("--decrypt", argv[i]) == 0) {
			is_encrypt = 0;
			continue;
		}
		if (filepath != NULL && message != NULL) {
			printf("Can use only one mode at time.\n");
			return displayHelp();
		}
		return displayHelp();
	}
	if (is_encrypt < 0) {
		printf("What would you like to do? Encrypt or Decrypt?\n");
		return displayHelp();
	}

	if (message) return textMode((unsigned char*)message, key, is_encrypt);
	else return fileMode(filepath, key, is_encrypt);
	return displayHelp();
}