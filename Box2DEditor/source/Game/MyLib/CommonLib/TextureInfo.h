#ifndef TEXTURE_INFO_H
#define TEXTURE_INFO_H

#include <string>

class TextureInfo
{
public:
	std::string filePath;
	unsigned int textureID;
	int width;
	int height;
	int bytesPerPixel;

	int contentW;
	int contentH;

	int refCount;
	int readFrom;
	//unsigned char* imageRawData;

	TextureInfo();
	~TextureInfo();
};

#endif
