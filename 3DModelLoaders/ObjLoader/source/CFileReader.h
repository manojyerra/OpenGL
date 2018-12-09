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

	static int GetLength(char* filePath);
	int GetLength();
	char* ReadLine();
	void Reset();
};
