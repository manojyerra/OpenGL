#include "UtilFuncs.h"
#include "math.h"
#include <windows.h>
#include <gl/gl.h>

unsigned int UtilFuncs::GenerateGLTextureID(ImageBuffer* imgBuf)
{
	int width = imgBuf->GetWidth();
	int height = imgBuf->GetHeight();
	int bytesPerPixel = imgBuf->GetBytesPerPixel();
	void* buffer = imgBuf->GetBuffer();

	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//if(imgBuf->IsDDS())
	//{
	//	unsigned int format = 0;

	//	if(imgBuf->IsDDS_DXT1())
	//		format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
	//	else if(imgBuf->IsDDS_DXT3())
	//		format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
	//	else if(imgBuf->IsDDS_DXT5())
	//		format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;

	//	glCompressedTexImage2DARB( GL_TEXTURE_2D, 0, format, width, height, 0, imgBuf->GetBufferSize(), imgBuf->GetBuffer());	
	//}
	//else
	{
		if(bytesPerPixel == 4)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
		else if(bytesPerPixel == 3)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, buffer);
		else if(bytesPerPixel == 1)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, width, height, 0, GL_RGB8, GL_UNSIGNED_BYTE, buffer);
		else
			return 0;
	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	return textureID;
}

unsigned int UtilFuncs::GenerateGLTextureID(int width, int height, bool hasAlpha, unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	int bytesPerPixel = hasAlpha ? 4 : 3;
	int size = width * height * bytesPerPixel;
	unsigned char* buffer = (unsigned char*)malloc(size);

	for(int i=0; i<size; i+=bytesPerPixel)
	{
		buffer[i+0] = r;
		buffer[i+1] = g;
		buffer[i+2] = b;

		if(bytesPerPixel == 4)
			buffer[i+3] = a;
	}

	unsigned int textureID = 0;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	if(bytesPerPixel == 4)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
	else if(bytesPerPixel == 3)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, buffer);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	free(buffer);

	return textureID;
}

