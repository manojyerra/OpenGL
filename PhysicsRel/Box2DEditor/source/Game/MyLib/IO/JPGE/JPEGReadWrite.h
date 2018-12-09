#ifndef JPEG_READ_WRITE_H
#define JPEG_READ_WRITE_H

class JPEGReadWrite
{
private:

public:
	static unsigned char* Read(char* path, int* imgWidth, int* imgHeight, int* bytesPerPixel);
	static bool Write(unsigned char* rawData, int width, int height, int bitsPerPixel, const char *savePath);
};

#endif
