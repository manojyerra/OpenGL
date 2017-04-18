#include "CFileReader.h"


CFileReader::CFileReader(string filePath, string readMode)
{
	fp = fopen(filePath.c_str(), readMode.c_str());
	pos = 0;
	length = 0;
	buf = NULL;

    if(fp)
    {
		length = GetLength((char*)filePath.c_str());

		buf = (char*)malloc(length+1);

		memset(buf, (int)'\0', length+1);

		fread(buf, 1, length, fp);
	}
}

int CFileReader::GetLength(char* filePath)
{
    FILE  *fp = fopen(filePath, "rb");
    
    if(fp)
    {
        fseek(fp, 0, SEEK_END);
        unsigned int fileSize = (unsigned int)ftell(fp);
        fseek(fp, 0, SEEK_SET);
        fclose(fp);
        return fileSize;
    }
    
    return 0;
}


int CFileReader::GetLength()
{
    return length;
}

void CFileReader::Reset()
{
	pos = 0;
}

char* CFileReader::ReadLine()
{
	if(pos >= length)
		return NULL;

	int i = pos;

	for(; i<length && buf[i] != '\n' && buf[i] != '\0'; i++);

	if(buf[i] == '\0')
		i = length;

	int lineLen = i - pos;

	if(buf[i] == '\n' || i == length)
	{
		char* lineBuf = (char*)malloc(lineLen+1);
		memset(lineBuf, (int)'\0', lineLen+1);
		memcpy(lineBuf, &buf[pos], lineLen);
		pos = i+1;
		return lineBuf;
	}

	return NULL;
}

CFileReader::~CFileReader()
{
	if(buf)
	{
		free(buf);
		buf = NULL;
	}
}