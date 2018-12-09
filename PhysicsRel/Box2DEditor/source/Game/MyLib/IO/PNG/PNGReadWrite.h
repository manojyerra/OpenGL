#ifndef PNG_READ_WRITE_H
#define PNG_READ_WRITE_H

#include <string>
using namespace std;

class PNGReadWrite
{
public:
	static unsigned char* Read(char* buf, int bufSize, int* imgWidth, int* imgHeight, int* bytesPerPixel);
	//static bool Write(unsigned char* rawData, int width, int height, int bitsPerPixel, const char *savePath);

	//static bool BeginWrite(unsigned char* rawData, int width, int height, int bitsPerPixel, const char *savePath);
	//static int WritePercent(float percent);
	//static void EndWrite();
};

#endif
