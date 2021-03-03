#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mencrypt.h"
#if defined(_WIN32)  
#include <malloc.h>
#elif defined(_WIN64)
#include <malloc.h>
#else
#include <unistd.h>
#endif



/*************************************************************************************************************************************/


void* mencrypt(void* data, unsigned long int size, void* key, unsigned long int keysize) {
	//printf("Encrypting data\n");
	void* enc_data = malloc(size * 4);			// Allocate a buffer for the encrypted data
	unsigned long int skip = keysize / 2 + 1;
	for (unsigned long int i = 0; i < size; i++)
	{
		*(int*)(enc_data + (i * 4)) = byteop(*(unsigned char*)(data + i), *(unsigned char*)(key + skip * i % keysize)); 
	}
	return enc_data;
}


void* mdecrypt(void* enc_data, unsigned long int size, void* key, unsigned long int keysize) // Use size IN BYTES of the encrypted data, this will be 4x the size in bytes of the original data
{
	//printf("Decrypting data\n");
	void* data = malloc(size / 4);
	unsigned long int skip = keysize / 2 + 1;
	for (int i = 0; i < size / 4; i++)
	{
		*(unsigned char*)(data + i) = bytedeop(*(int*)(enc_data + (4 * i)),*(unsigned char*)(key + skip * i % keysize));
	}
	return data;
}


/****************************************************************************************************************************************/



int getop(unsigned char byte, int index) { //Get the two bit op instruction
	switch(index)
	{
		case 0: //printf("Getop called, returned: %d\n", (int)((byte >> 0) % 4));
		return(int)((byte >> 0) % 4);
		case 1: //printf("Getop called, returned: %d\n", (int)((byte >> 2) % 4));
		return(int)((byte >> 2) % 4);
		case 2: //printf("Getop called, returned: %d\n", (int)((byte >> 4) % 4));
		return(int)((byte >> 4) % 4);
		case 3: //printf("Getop called, returned: %d\n", (int)((byte >> 16) % 4));
		return(int)((byte >> 16) % 4);
		default://printf("Getop called, returned: %d\n", 0);
		return 0;
	}
}

int getnum(unsigned char byte, int remove_index) //Get the six bits left in the byte
{
	unsigned char a = byte % 4;
	unsigned char b = byte / 2 % 4;
	unsigned char c = byte / 4 % 4;
	unsigned char d = byte / 16 % 4;
	//printf("Getnum Called, index: %d\n", remove_index); 
	switch (remove_index)
	{
		case 0: return (int) b + c * 4 + d * 16;
		case 1: return (int) a + c * 4 + d * 16;
		case 2: return (int) a + b * 4 + d * 16;
		case 3: return (int) a + b * 4 + c * 16;
		default: return 0;
	}
}

int doop(int in, unsigned char keybyte, int index)
{
	switch(getop(keybyte, index))
	{
		case 0: return in + getnum(keybyte, index);	// case ADD
		
		case 1: return in - getnum(keybyte, index);	// case SUB
		
		case 2:		// case MUL
		if (getnum(keybyte, index) == 0) return in * keybyte;
		else return in * getnum(keybyte, index);
		case 3: return in;
	}
}

int deop(int in, unsigned char keybyte, int index)
{
	switch(getop(keybyte, index))
	{
		case 0: return in - getnum(keybyte, index);	// case ADD
		
		case 1: return in + getnum(keybyte, index);	// case SUB
		
		case 2:		// case MUL
		if (getnum(keybyte, index) == 0) return in / keybyte;
		else return in / getnum(keybyte, index);
		case 3: return in;
	}
}

int byteop(unsigned char byte, unsigned char keybyte)
{
	int out = byte;
	for (int i = 0; i < 4; i++)
	{
		out = doop(out, keybyte, i);
	}
	return out;
}

unsigned char bytedeop(int enc_byte, unsigned char keybyte)
{
	int out = enc_byte;
	for (int i = 3; i > -1; i--)
	{
		out = deop(out, keybyte, i);
	}
	return*(unsigned char*)(&out);
}


/**********************************************************
int main(void)
{

	//printf("%d\n",(int)getop((unsigned char)0xff, 0));
	//printf("%d\n",(int)getnum((unsigned char)0xff, 0));
	//printf("%ld\n",sizeof(int));

	char* data = "Hello World!";
	char* key = "lelxd";
	
	printf("%s\n", data);
	void* enc_data = mencrypt((void*)data, 13, (void*)key, 5);
	write(1, (const void*)enc_data, (size_t)(13 * 4));
	printf("\n");
	for (int i = 0; i < 13; i++)
	{
		printf("%d\n", *(int*)(enc_data + i * 4));
	}
	
	void* unenc_data = mdecrypt(enc_data, 13*4, (void*)key, 5);
	printf("Decryption output: ");
	write(1, (const void*)unenc_data, (size_t)13);
	printf("\n");
	for (int i = 0; i < 13; i++)
	{
		printf("%d\n", (int)*(unsigned char*)(unenc_data + i));
	}
	
	free(enc_data);
	free(unenc_data);
	return 0;
}
**********************************************************/
