#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "b64.h"
#include "utility.h"
#define BLOCK_SIZE 16


int main(int argc, char const *argv[]) {
	unsigned char message[21] = "aaaaaaaaaaaaaaaaabbbb";
	unsigned char key[16] = "aaaaaaaaaaaaaaaa";

	//Initialize key schedule
	unsigned char expandedKey[176];
	KeyExpansion(key, expandedKey);

	//Padding For plaintext
	int length = strlen((const char*)message);
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
	{
		Encrypt(paddedMessage+i, expandedKey); //paddedMessage has been encrypted
	}
	for (int i = 0; i < paddedLength; i++)
		printf("%c", paddedMessage[i]);
	//For file


	return 0;
}