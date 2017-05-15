#include "CFileReader.h"
#include "DefinesAndIncludes.h"

CFileReader::CFileReader(string filePath, string readMode)
{
	fp = fopen(filePath.c_str(), readMode.c_str());
	pos = 0;
	length = 0;
	buf = NULL;

    if(fp)
    {
		length = GetLength((char*)filePath.c_str());

		buf = (char*)mallocTrace(length+1);

		memset(buf, (int)'\0', length+1);

		fread(buf, 1, length, fp);

		fclose(fp);
	}
}

int CFileReader::GetLength(string filePath)
{
	FILE  *fp = fopen(filePath.c_str(), "rb");
    
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

bool CFileReader::IsFileExists(string filePath)
{
	FILE  *fp = fopen(filePath.c_str(), "r");
    
    if(fp)
    {
        fclose(fp);
        return true;
    }

	return false;
}

char* CFileReader::GetData()
{
	return buf;
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

	unsigned int i = pos;

	for(; i<length && buf[i] != '\n' && buf[i] != '\0'; i++);

	if(buf[i] == '\0')
		i = length;

	int lineLen = i - pos;

	if(buf[i] == '\n' || i == length)
	{
		char* lineBuf = (char*)mallocTrace(lineLen+1);
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
		freeTrace(buf);
		buf = NULL;
	}
}


//char* CFileReader::GetData()
//{
//    if(fp)
//    {
//        fseek(fp, 0, SEEK_END);
//        unsigned int fileSize = (unsigned int)ftell(fp);
//        
//        fseek(fp, 0, SEEK_SET);
//        char* arrData = (char*)malloc(fileSize+1);
//        memset(arrData, (int)'\0', fileSize+1);
//        
//        if(fread(arrData, 1, fileSize, fp) != fileSize)
//        {
//            free(arrData);
//            fclose(fp);
//            return NULL;
//        }
//        
//        fclose(fp);
//        return arrData;
//    }
//    
//    return NULL;
//}
