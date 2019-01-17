#include "DDSReadWrite.h"
#include "stdio.h"
#include <ddraw.h>

unsigned char* DDSReadWrite::Read(char * strFileName, int* imgW, int* imgH, int* bytesPerPixel, int* pixelFormat, int* numMipMaps, int* bufSizeInBytes)
{
	FILE *pFile;
	if((pFile = fopen(strFileName, "rb")) == NULL) 
		return NULL;

	int factor = 0;
	DDSURFACEDESC2 ddsd;   
	char filecode[4];
    int bufferSize = 0;
	int format = 0;

	fread( filecode, 1, 4, pFile );		// Verify the file is a true .dds file

    if( strncmp( filecode, "DDS ", 4 ) != 0 )
	{
		printf("The file doesn't appear to be a valid .dds file!");
		fclose( pFile );
		return NULL;
	}

    fread( &ddsd, sizeof(ddsd), 1, pFile );				// Get the surface descriptor

	unsigned int __GL_COMPRESSED_RGB_S3TC_DXT1_EXT = 0x83F0;
	unsigned int __GL_COMPRESSED_RGBA_S3TC_DXT1_EXT = 0x83F1;
	unsigned int __GL_COMPRESSED_RGBA_S3TC_DXT3_EXT = 0x83F2;
	unsigned int __GL_COMPRESSED_RGBA_S3TC_DXT5_EXT = 0x83F3;

    if(ddsd.ddpfPixelFormat.dwFourCC == FOURCC_DXT1)	// DXT1's compression ratio is 8:1
    {
		format = __GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
		factor = 2;
	}
	else if(ddsd.ddpfPixelFormat.dwFourCC == FOURCC_DXT3)	// DXT3's compression ratio is 4:1
	{
        format = __GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
        factor = 4;
	}
	else if(ddsd.ddpfPixelFormat.dwFourCC == FOURCC_DXT5)	// DXT5's compression ratio is 4:1
	{
		format = __GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
		factor = 4;
	}
	else
	{
		printf("The file doesn't appear to be compressed using DXT1, DXT3, or DXT5");
		return NULL;
    }

    if( ddsd.dwLinearSize == 0 )
    {
        printf("dwLinearSize is 0");
		return NULL;
    }

    if( ddsd.dwMipMapCount > 1 )
        bufferSize = ddsd.dwLinearSize * factor;
    else
        bufferSize = ddsd.dwLinearSize;

    unsigned char* data = (unsigned char*)malloc(bufferSize * sizeof(unsigned char));
	fread( data, 1, bufferSize, pFile );
    fclose( pFile );

	int components = 4;
	if( ddsd.ddpfPixelFormat.dwFourCC == FOURCC_DXT1 )
		components = 3;

	*imgW = ddsd.dwWidth;
	*imgH = ddsd.dwHeight;
	*pixelFormat = format;
	*bytesPerPixel = components;
	*numMipMaps = ddsd.dwMipMapCount;
	*bufSizeInBytes = bufferSize;

	return data;
}

bool DDSReadWrite::Write(unsigned char* rawData, int width, int height, int bitsPerPixel, const char *savePath)
{
	return false;
}
