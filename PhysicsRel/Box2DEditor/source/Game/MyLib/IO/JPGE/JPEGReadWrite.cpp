#include "JPEGReadWrite.h"
#include "stdio.h"
#include "jpeglib.h"
#include "stdlib.h"


unsigned char* JPEGReadWrite::Read(char * strFileName, int* imgW, int* imgH, int* bytesPerPixel)
{
	struct jpeg_decompress_struct cinfo;
	FILE *pFile;
		
	if((pFile = fopen(strFileName, "rb")) == NULL)
		return NULL;

	jpeg_error_mgr jerr;
	cinfo.err = jpeg_std_error(&jerr);	
	jpeg_create_decompress(&cinfo);
	jpeg_stdio_src(&cinfo, pFile);	

	//Begin : Decode JPG
	jpeg_read_header(&cinfo, TRUE);
	jpeg_start_decompress(&cinfo);

	int channels = cinfo.num_components;
	int sizeX    = cinfo.image_width;
	int sizeY    = cinfo.image_height;

	int rowSpan = cinfo.image_width * cinfo.num_components;
	
	unsigned char* data = ((unsigned char*)malloc(sizeof(unsigned char)*rowSpan*sizeY));
	unsigned char** rowPtr = new unsigned char*[sizeY];

	for (int i = 0; i < sizeY; i++)
		rowPtr[i] = &(data[i * rowSpan]);

	int rowsRead = 0;
	while (cinfo.output_scanline < cinfo.output_height) 
		rowsRead += jpeg_read_scanlines(&cinfo, &rowPtr[rowsRead], cinfo.output_height - rowsRead);
	
	delete [] rowPtr;
	jpeg_finish_decompress(&cinfo);
	//End : Decode JPG
	
	jpeg_destroy_decompress(&cinfo);
	fclose(pFile);

	*imgW = sizeX;
	*imgH = sizeY;
	*bytesPerPixel = channels;

	return data;
}


bool JPEGReadWrite::Write(unsigned char* rawData, int width, int height, int bitsPerPixel, const char *savePath)
{
	return false;
}