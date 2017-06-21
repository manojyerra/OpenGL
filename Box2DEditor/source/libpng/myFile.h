#ifndef MYFILE_H
#define MYFILE_H

#include <stdio.h>

unsigned int pos;
int tempPos;
unsigned char* buf;//[2*1048576];
FILE* myFP;
size_t fileSize;

FILE* myFileOpen(const char *path, const char *mode);
size_t myFileRead(unsigned char* dest, size_t size, size_t nmemb, FILE *stream);
void myFileClose(FILE* fp);

#endif