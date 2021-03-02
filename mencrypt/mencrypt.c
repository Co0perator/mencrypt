#include <stdio.h>
#include <stdlib.h>
#include "mencrypt.h"
#if defined(_WIN32)  
#include <malloc.h>
#elif defined(_WIN64)
#include <malloc.h>
#else
#include <unistd.h>
#endif

void* mencrypt(void* data, unsigned long int size, void* key, unsigned long int keysize) {
	void* enc_data = malloc(size * 4);			// Allocate a buffer for the encrypted data
	unsigned long int skip = keysize / 2 + 1, current = 0;
	for (unsigned long int i = 0; i < size; i++)
	{
		*((int*)enc_data + i * 4) = byteop(*(unsigned char*)(data + i), *(unsigned char*)(key + skip * i % keysize)); // A very complicated way of making the buffer location for the processed by equal to the processed byte
	}
	return enc_data;
}

int getop(unsigned char byte, int index) { //Get the two bit op instruction
	switch(index)
	{
		case 0: return(int)((byte >> 0) % 4);
		case 1: return(int)((byte >> 2) % 4);
		case 2: return(int)((byte >> 4) % 4);
		case 3: return(int)((byte >> 16) % 4);
		default: return 0;
	}
}

int getnum(unsigned char byte, int remove_index) //Get the six bits left in the byte
{
	char a = byte % 4;
	char b = byte / 2 % 4;
	char c = byte / 4 % 4;
	char d = byte / 16 % 4;
	switch (remove_index)
	{
		case 0: return (int) b + c * 4 + d * 16;
		case 1: return (int) a + c * 4 + d * 16;
		case 2: return (int) a + b * 4 + d * 16;
		case 3: return (int) a + b * 4 + c * 16;
		default: return 0;
	}
}

int byteop(unsigned char byte, unsigned char keybyte) {
	int out = byte;
	for (int i = 0; i < 4; i++)
	{
		switch(getop(keybyte, i))
		{
			case 0: out = out + getnum(keybyte, i);	// case ADD
			break;
			
			case 1: out = out - getnum(keybyte, i);	// case SUB
			break;
			
			case 2: int num = getnum(keybyte, i);		// case MUL
			if (num == 0) out = out * keybyte;
			else out = out * num;
			
			default: break;				// case SKIP
		}
	}
	return out;
}


int main(void)
{
	printf("%d\n",(int)getop((unsigned char)0xff, 0));
	printf("%d\n",(int)getnum((unsigned char)0xff, 0));
	printf("%ld\n",sizeof(int));
}
