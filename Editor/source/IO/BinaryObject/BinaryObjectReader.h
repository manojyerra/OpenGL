#ifndef BinaryObjectReader_H
#define BinaryObjectReader_H

#include <string>
using namespace std;

class BinaryObjectReader
{
private:
	unsigned char* data;

	static int** GetIntDDPointer(int* intArr, int rows, int cols);
	static float** GetFloatDDPointer(float* floatArr, int rows, int cols);
	static short** GetShortDDPointer(short* shortArr, int rows, int cols);
	static char** GetCharDDPointer(char* charArr, int rows, int cols);
	static void WriteUInt(unsigned char* mem, int writeAtIndex, unsigned int val);

public:
	BinaryObjectReader(char* filePath, void* obj, int objSize);
	~BinaryObjectReader();
};

#endif