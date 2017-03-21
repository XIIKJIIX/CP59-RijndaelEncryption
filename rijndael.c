#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "utility.h"
#include "base64.h"
#define MIN_ARGC 6

int fileMode(char *filepath, unsigned char *key, int is_encrypt) {

	unsigned char expandedKey[176];
	KeyExpansion(key, expandedKey);

	FILE *fp, *fn;
	fp = fopen(filepath, "rb");
	if (fp == NULL) {
		printf("File doesn't exist!!\n");
		return 0;
	}

	//Get file size
	fseek(fp, 0, SEEK_END);
	int size = ftell(fp);
	rewind(fp);

	printf("File name: %s\n", filepath);
	printf("File size: %d byte(s)\n", size);

	//Decrypt
	if (!is_encrypt) {
		char *decFile = (char*)calloc(strlen(filepath), sizeof(char));
		strcpy(decFile, filepath);
		decFile[strlen(decFile)-10] = '\0'; //Delete .encrypted
		printf("Decrypted file: %s\n", decFile);
		fn = fopen(decFile, "wb");

		for (int i = 0; i < size; i+=16) {
			unsigned char *buff = (unsigned char*)calloc(BLOCK_SIZE, sizeof(unsigned char));
			fread(buff, 1, BLOCK_SIZE, fp);
			Decrypt(buff, expandedKey);
			fwrite(buff, 1, BLOCK_SIZE, fn);
		}

		fclose(fn);
		fclose(fp);
		return 0;
	}

	//Encrypt
	char *encFile = (char*)calloc(strlen(filepath)+11, sizeof(char));
	strcpy(encFile, filepath);
	strcat(encFile, ".encrypted");
	fn = fopen(encFile, "wb");
	printf("Encrypted file: %s\n", encFile);

	//File doesn't need to be padded because calloc will auto padded with \0x00
	for (int i = 0; i < size; i+=16) {
		unsigned char *buff = (unsigned char*)calloc(BLOCK_SIZE, sizeof(unsigned char));
		fread(buff, 1, BLOCK_SIZE, fp);
		Encrypt(buff, expandedKey);
		fwrite(buff, 1, BLOCK_SIZE, fn);
	}
	fclose(fn);
	fclose(fp);
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
			continue;
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