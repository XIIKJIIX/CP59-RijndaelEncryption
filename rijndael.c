#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "b64.h"
#include "utility.h"
#define BLOCK_SIZE 16

void KeyExpansionCore(unsigned char *in, unsigned char i) {
	//Rotate to Left
	unsigned char t = in[0];
	in[0] = in[1];
	in[1] = in[2];
	in[2] = in[3];
	in[3] = t;

	//S-box four byte
	in[0] = S_BOX[in[0]];
	in[1] = S_BOX[in[1]];
	in[2] = S_BOX[in[2]];
	in[3] = S_BOX[in[3]];

	//RCon
	in[0] ^= RCON[i];
}

void KeyExpansion (unsigned char *InputKey, unsigned char *expandedKey) {
	//First 16 bytes are the original key
	for (int i = 0; i < 16; i++)
		expandedKey[i] = InputKey[i];

	//Variable
	int bytesGenerates = 16; //Now we've generated 16 bytes so far
	int rconIteration = 1; //RCon Iteration begin at 1
	unsigned char temp[4]; //Temporary storage for core

	while (bytesGenerates < 176) {
		//Read 4 bytes for the core
		for (int i = 0; i < 4; i++)
			temp[i] = expandedKey[i + bytesGenerates - 4];
		//Perform the core once for each 16 bytes
		if (bytesGenerates % 16 == 0)
			KeyExpansionCore(temp, rconIteration++);
		//XOR temp with [byteGenerates-16] and store in expandedKey
		for (int i = 0; i < 4; i++) {
			expandedKey[bytesGenerates] = expandedKey[bytesGenerates - 16] ^ temp[i];
			bytesGenerates++;
		}
	}
}
void SubBytes(unsigned char *state) {
	for (int i = 0; i < BLOCK_SIZE; i++)
		state[i] = S_BOX[state[i]];
}
void ShiftRows(unsigned char *state) {
	unsigned char tmp[BLOCK_SIZE];
	tmp[0] = state[0];
	tmp[1] = state[5];
	tmp[2] = state[10];
	tmp[3] = state[15];

	tmp[4] = state[4];
	tmp[5] = state[9];
	tmp[6] = state[14];
	tmp[7] = state[3];

	tmp[8] = state[8];
	tmp[9] = state[13];
	tmp[10] = state[2];
	tmp[11] = state[7];

	tmp[12] = state[12];
	tmp[13] = state[1];
	tmp[14] = state[6];
	tmp[15] = state[11];

	for (int i = 0; i < BLOCK_SIZE; i++)
		state[i] = tmp[i];
}
void MixColumns(unsigned char *state) {
	unsigned char tmp[BLOCK_SIZE];
	tmp[0] = (unsigned char)(MUL_2[state[0]] ^ MUL_3[state[1]] ^ state[2] ^ state[3]);
	tmp[1] = (unsigned char)(state[0] ^ MUL_2[state[1]] ^ MUL_3[state[2]] ^ state[3]);
	tmp[2] = (unsigned char)(state[0] ^ state[1] ^ MUL_2[state[2]] ^ MUL_3[state[3]]);
	tmp[3] = (unsigned char)(MUL_3[state[0]] ^ state[1] ^ state[2] ^ MUL_2[state[3]]);

	tmp[4] = (unsigned char)(MUL_2[state[4]] ^ MUL_3[state[5]] ^ state[6] ^ state[7]);
	tmp[5] = (unsigned char)(state[4] ^ MUL_2[state[5]] ^ MUL_3[state[6]] ^ state[7]);
	tmp[6] = (unsigned char)(state[4] ^ state[5] ^ MUL_2[state[6]] ^ MUL_3[state[7]]);
	tmp[7] = (unsigned char)(MUL_3[state[4]] ^ state[5] ^ state[6] ^ MUL_2[state[7]]);

	tmp[8] = (unsigned char)(MUL_2[state[8]] ^ MUL_3[state[9]] ^ state[10] ^ state[11]);
	tmp[9] = (unsigned char)(state[8] ^ MUL_2[state[9]] ^ MUL_3[state[10]] ^ state[11]);
	tmp[10] = (unsigned char)(state[8] ^ state[9] ^ MUL_2[state[10]] ^ MUL_3[state[11]]);
	tmp[11] = (unsigned char)(MUL_3[state[8]] ^ state[9] ^ state[10] ^ MUL_2[state[11]]);

	tmp[12] = (unsigned char)(MUL_2[state[12]] ^ MUL_3[state[13]] ^ state[14] ^ state[15]);
	tmp[13] = (unsigned char)(state[12] ^ MUL_2[state[13]] ^ MUL_3[state[14]] ^ state[15]);
	tmp[14] = (unsigned char)(state[12] ^ state[13] ^ MUL_2[state[14]] ^ MUL_3[state[15]]);
	tmp[15] = (unsigned char)(MUL_3[state[12]] ^ state[13] ^ state[14] ^ MUL_2[state[15]]);

	for (int i = 0; i < BLOCK_SIZE; i++)
		state[i] = tmp[i];
}
void AddRoundKey(unsigned char *state, unsigned char *roundKey) {
	for (int i = 0; i < BLOCK_SIZE; i++)
		state[i] ^= roundKey[i];
}

void Encrypt (unsigned char *message, unsigned char *expandedKey) {
	unsigned char state[BLOCK_SIZE];
	for (int i = 0; i < BLOCK_SIZE; i++)
		state[i] = message[i];
	int numberOfRounds = 9;

	AddRoundKey(state, expandedKey); //Whitenning/Add roundkey

	for (int i = 0; i < numberOfRounds; i++) {
		SubBytes(state);
		ShiftRows(state);
		MixColumns(state);
		AddRoundKey(state, expandedKey + (16*(i+1)));
	}

	// Final Round
	SubBytes(state);
	ShiftRows(state);
	AddRoundKey(state, expandedKey+160);

	for (int i = 0; i < BLOCK_SIZE; i++)
		message[i] = state[i];
}


int main() {
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