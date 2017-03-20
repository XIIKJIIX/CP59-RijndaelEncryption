#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "utility.h"
#include "base64.h"
#define MIN_ARGC 6

int fileMode(char *filepath, unsigned char *key, int is_encrypt) {

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