#ifndef DDS_READ_WRITE_H
#define DDS_READ_WRITE_H

class DDSReadWrite
{
private:

public:
	static unsigned char* Read(char* path, int* imgW, int* imgH, int* bytesPerPixel, int* pixelFormat, int* numMipMaps, int* bufSizeInBytes);
	static bool Write(unsigned char* rawData, int width, int height, int bitsPerPixel, const char *savePath);
};

#endif
