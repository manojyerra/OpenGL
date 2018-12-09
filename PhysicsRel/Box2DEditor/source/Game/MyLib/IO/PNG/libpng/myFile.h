#ifndef MYFILE_H
#define MYFILE_H

#include <stdio.h>

unsigned int pos;
unsigned char* buf;
size_t fileSize;

void myFileOpen(unsigned char* buffer, int bufferSize);
size_t myFileRead(unsigned char* dest, size_t size, size_t nmemb);
void myFileClose();

#endif
