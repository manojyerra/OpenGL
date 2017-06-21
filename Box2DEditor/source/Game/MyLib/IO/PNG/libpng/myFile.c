#include "myFile.h"

void myFileOpen(unsigned char* buffer, int bufferSize)
{
	pos = 0;
	buf = buffer;
	fileSize = bufferSize;
}

size_t myFileRead(unsigned char* dest, size_t size, size_t nmemb)
{
    memcpy((void *)dest, (const void *)&buf[pos], nmemb);
    pos += nmemb;

    return nmemb;
}

void myFileClose()
{
	pos = 0;
	buf = NULL;
	fileSize = 0;
}
