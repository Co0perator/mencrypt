#ifndef MENCRYPT_H
#define MENCRYPT_H

void* mencrypt(void* data, unsigned int size, void* key, unsigned int keysize);
int getop(unsigned char byte, int index);
int getnum(unsigned char byte, int remove_index);

#endif
