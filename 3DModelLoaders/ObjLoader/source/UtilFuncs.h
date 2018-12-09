#ifndef UtilFuncs_H
#define UtilFuncs_H

#include "ImageBuffer.h"
#include <vector>
#include <string>
using namespace std;

class UtilFuncs
{
public:
	static unsigned int GenerateGLTextureID(ImageBuffer* imgBuf);
	static unsigned int GenerateGLTextureID(int width, int height, bool hasAlpha, unsigned char r, unsigned char g, unsigned char b, unsigned char a);
};


#endif