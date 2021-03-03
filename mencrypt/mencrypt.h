#ifndef MENCRYPT_H
#define MENCRYPT_H

/*********************   Encrypt and Decrypt   ******************************/

void* mencrypt(void* data, unsigned long int size, void* key, unsigned long int keysize);
void* mdecrypt(void* data, unsigned long int size, void* key, unsigned long int keysize);

/****************************************************************************/

int getop(unsigned char byte, int index);
int getnum(unsigned char byte, int remove_index);
int byteop(unsigned char byte, unsigned char keybyte);
unsigned char bytedeop(int enc_byte, unsigned char keybyte);
int doop(int in, unsigned char keybyte, int index);
int deop(int in, unsigned char keybyte, int index);

#endif
