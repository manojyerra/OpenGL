#ifndef CFileReader_H
#define CFileReader_H

#include <stdio.h>
#include <string>
using namespace std;

class CFileReader
{
private:
	FILE* fp;
	unsigned int pos;
	unsigned int length;

	char* buf;

public:
	CFileReader(string filePath, string readMode);
	~CFileReader();

	static int GetLength(string filePath);
	static bool IsFileExists(string filePath);

	int GetLength();
	char* GetData();
	char* ReadLine();
	void Reset();
};

#endif
