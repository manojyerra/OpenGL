#include "JPEGReadWrite.h"
#include "stdlib.h"
#include "jpeglib.h"

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


	//FILE* infile;
 //   if((infile = fopen(filename, "rb")) == NULL)
 //       return 0;

	//struct jpeg_decompress_struct info;
 //   struct jpeg_error_mgr err;
	//
	//JSAMPARRAY buffer;
 //   int row_stride;

 //   info.err = jpeg_std_error(&err);
	//jpeg_create_decompress(&info);
	//jpeg_stdio_src(&info, infile);
	//jpeg_read_header(&info, TRUE);
	//jpeg_start_decompress(&info);

 //   row_stride = info.output_width * info.output_components;
 //   buffer = (*info.mem->alloc_sarray) ((j_common_ptr) &info, JPOOL_IMAGE, row_stride, 1);

 //   int imageWidth = info.output_width;
 //   int imageHeight = info.output_height;
 //   unsigned char* imagePixels = new unsigned char [info.output_width * info.output_height * info.output_components];

 //  ////step 6, read the image line by line

	//// long counter = 0;
 //  // while (cinfo.output_scanline < cinfo.output_height) {
 //  //     //IT ALWAYS crash ON THIS JPEG_READ_SCANLINES FUNCTION CALL BELOW
 //  //     (void) jpeg_read_scanlines(&cinfo, (JSAMPARRAY)(imageData->pixels), 1);
 //  //     counter +=row_stride;
 //  // }

	//JSAMPROW output_data;
	//unsigned int scanline_len = info.output_width * info.output_components;

	//unsigned int scanline_count = 0;
	//while (info.output_scanline < info.output_height)
	//{
	//	output_data = (imagePixels + (scanline_count * scanline_len));
	//	jpeg_read_scanlines(&info, &output_data, 1);
	//	scanline_count++;
	//}

 //   jpeg_finish_decompress(&info);
	//jpeg_destroy_decompress(&info);
 //   fclose(infile);

	//*imgW = imageWidth;
	//*imgH = imageHeight;
	//*bytesPerPixel = info.output_components;

    //return imagePixels;




//unsigned char* SuiJPEGReadWrite::Read(char* FileName, int* imgWidth, int* imgHeight, int* bytesPerPixel)
//{
//	bool Fast = true;
//	unsigned long w;
//	unsigned long h;
//	unsigned short int bpp;		//bits per pixels   unsigned short int
//	unsigned char* data;		//the data of the image
//	unsigned int ID;			//the id ogl gives it
//	unsigned long size;			//length of the file
//	int channels;				//the channels of the image 3 or 4 bytes Per Pixel.
//	GLuint type;  
//
//	FILE* file = fopen(FileName, "rb");  //open the file
//	if(!file)
//		return false;
//
//	struct jpeg_decompress_struct info;  //the jpeg decompress info
//	struct jpeg_error_mgr err;           //the error handler
//
//	info.err = jpeg_std_error(&err);     //tell the jpeg decompression handler to send the errors to err
//	jpeg_create_decompress(&info);       //sets info to all the default stuff
//	jpeg_stdio_src(&info, file);    //tell the jpeg lib the file we'er reading
//	jpeg_read_header(&info, TRUE);   //tell it to start reading it
//
//	//if it wants to be read fast or not
//	if(Fast)
//		info.do_fancy_upsampling = FALSE;
//
//	jpeg_start_decompress(&info);    //decompress the file
//
//	//set the x and y
//	w = info.output_width;
//	h = info.output_height;
//	channels = info.num_components;
//
//	type = GL_RGB;
//	if(channels == 4)
//		type = GL_RGBA;
//
//	bpp = channels * 8;
//	size = w * h * 3;
//
//	//read turn the uncompressed data into something ogl can read
//	data = new BYTE[size];      //setup data for the data its going to be handling
//
//	BYTE* p1 = data;
//	BYTE** p2 = &p1;
//	int numlines = 0;
//
//	while(info.output_scanline < info.output_height)
//	{
//		numlines = jpeg_read_scanlines(&info, p2, 1);
//		*p2 += numlines * 3 * info.output_width;
//	}
//
//	jpeg_finish_decompress(&info);   //finish decompressing this file
//	fclose(file);                    //close the file
//
//	*imgWidth = w;
//	*imgHeight = h;
//	*bytesPerPixel = channels;
//
//    return data;
//}
//