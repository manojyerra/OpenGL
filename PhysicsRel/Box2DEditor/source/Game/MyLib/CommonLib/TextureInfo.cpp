#include "TextureInfo.h"
#include "Platform.h"

TextureInfo::TextureInfo()
{
	filePath = "";
	textureID = 0;

	width = 0;
	height = 0;


	contentW = -1;
	contentH = -1;

	refCount = 1;
	bytesPerPixel = 0;
	readFrom = Platform::READ_FROM_ASSETS;
	//imageRawData = NULL;
}

TextureInfo::~TextureInfo()
{
}
