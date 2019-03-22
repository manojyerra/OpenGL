#include "PNGReadWrite.h"
#include "libpng/png.h"

FILE * fpToWrite;

png_uint_32 rowIndex = 0;
int pass = 0;
int num_pass = 0;
png_bytepp rp;

png_structp png_ptr = NULL;
png_infop info_ptr = NULL;
png_byte ** row_pointers = NULL;

float prevPercent = 0;
float currPercent = 0;
int _height = 0;

unsigned char* PNGReadWrite::Read(string path, int* imgWidth, int* imgHeight, int* bytesPerPixel)
{
    png_byte header[8]; //8 is the maximum header size that can be checked

	FILE* fp = fopen(path.c_str(), "rb");

	if(!fp)
		return NULL;
    
	fread(header, 1, 8, fp);

	if(png_sig_cmp(header, 0, 8)) 
	{
		fclose(fp);
		return NULL;
	}

	png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);	//initialize stuff
	if(!png_ptr)
	{
		fclose(fp);
		return NULL;
	}

	png_infop info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr)
	{
		png_destroy_read_struct(&png_ptr, (png_infopp) NULL, (png_infopp) NULL);
		fclose(fp);
		return NULL;
	}

	png_infop end_info = png_create_info_struct(png_ptr);
	if (!end_info)
	{
		png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp) NULL);
		fclose(fp);
		return NULL;
	}

	//png error stuff, not sure libpng man suggests this.
	if (setjmp(png_jmpbuf(png_ptr)))
	{
		png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
		fclose(fp);
		return NULL;
	}

	png_init_io(png_ptr, (FILE*)fp);		//init png reading
	png_set_sig_bytes(png_ptr, 8);			//let libpng know you already read the first 8 bytes
	png_read_info(png_ptr, info_ptr);		//read all the info up to the image data

	//variables to pass to get info

	int pixle_depth;
	int bit_depth;
	int color_type;
	int twidth;
	int theight;
	
	// get info about png
	png_get_IHDR(png_ptr, info_ptr, (png_uint_32*)&twidth, (png_uint_32*)&theight, &bit_depth, &color_type, NULL, NULL, NULL);
	png_read_update_info(png_ptr, info_ptr);				// Update the png info struct.
	pixle_depth = info_ptr->pixel_depth;
	int rowbytes = png_get_rowbytes(png_ptr, info_ptr);		// Row size in bytes.

	// Allocate the image_data as a big block, to be given to opengl
	
	int fialSize = sizeof(png_byte) * (rowbytes*theight);

	png_byte *image_data = (png_byte*)malloc(sizeof(png_byte) * (rowbytes*theight));

	if (!image_data)
	{
		png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);	//clean up memory and close stuff
		fclose(fp);
		return NULL;
	}

	//row_pointers is for pointing to image_data for reading the png with libpng
	png_bytep *row_pointers = (png_bytep*)malloc(sizeof(png_bytep)*theight);

	if (!row_pointers)
	{
		png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);	//clean up memory and close stuff
		free(image_data);
		fclose(fp);
		return NULL;
	}

	// set the individual row_pointers to point at the correct offsets of image_data
	for (int i = 0; i < theight; ++i)
	{
		row_pointers[i] = image_data + i * rowbytes;
		//row_pointers[theight - 1 - i] = image_data + i * rowbytes;
	}

	png_read_image(png_ptr, row_pointers);	//read the png into image_data through row_pointers

	png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
	fclose(fp);

	free(row_pointers);

    *imgWidth = twidth;
    *imgHeight = theight;
    *bytesPerPixel = pixle_depth / 8;
    
    return image_data;
}


bool PNGReadWrite::Write(unsigned char* rawData, int width, int height, int bitsPerPixel, const char *savePath)
{
	png_structp png_ptr = NULL;
	png_infop info_ptr = NULL;
	png_byte ** row_pointers = NULL;
	int x=0;
	int y=0;
	int depth = 8;
	int pixel_size = 3;
	int colorType = PNG_COLOR_TYPE_RGB;

	bool status = false;

	if(bitsPerPixel == 32)
	{
		pixel_size = 4;
		colorType = PNG_COLOR_TYPE_RGBA;
	}

	if(!(fpToWrite = fopen(savePath, "wb")))
		return status;

	png_ptr = png_create_write_struct (PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (png_ptr == NULL)
	{
		fclose(fpToWrite);
		return status;
	}

	png_set_compression_level(png_ptr, 9);

	info_ptr = png_create_info_struct (png_ptr);
	if (info_ptr == NULL || setjmp (png_jmpbuf (png_ptr)))
	{
		png_destroy_write_struct (&png_ptr, &info_ptr);
		fclose(fpToWrite);
		return status;
	}

	png_set_IHDR (png_ptr,
			  info_ptr,
			  width,
			  height,
			  depth,
			  colorType,
			  PNG_INTERLACE_NONE,
			  PNG_COMPRESSION_TYPE_DEFAULT,
			  PNG_FILTER_TYPE_DEFAULT);
    
    /* Initialize rows of PNG. */
    row_pointers = (png_byte**) png_malloc (png_ptr, height * sizeof (png_byte *));
    
	int index = 0;

	for( y = 0; y < height; ++y)
	{
        png_byte* row = (png_byte*) png_malloc (png_ptr, sizeof(unsigned char)*width*pixel_size);
        row_pointers[y] = row;
        
		for (x=0; x<width*pixel_size; ++x)
		{
            *row++ = rawData[index];
			index++;
		}
    }
    
    png_init_io (png_ptr, fpToWrite);
    png_set_rows (png_ptr, info_ptr, row_pointers);
    png_write_png (png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, NULL);

	if (info_ptr->valid & PNG_INFO_IDAT)
		png_write_image(png_ptr, info_ptr->row_pointers);
	png_write_end(png_ptr, info_ptr);

	for (y = 0; y < height; y++)
        png_free (png_ptr, row_pointers[y]);

	png_free (png_ptr, row_pointers);
	png_destroy_write_struct (&png_ptr, &info_ptr);
	fclose (fpToWrite);
	return true;
}

//******************************************** Writting in intervals ****************************************************/

bool PNGReadWrite::BeginWrite(unsigned char* rawData, int width, int height, int bitsPerPixel, const char *savePath)
{
	printf("PNG writing started...");
	png_ptr = NULL;
	info_ptr = NULL;
	row_pointers = NULL;
	_height = height;

	unsigned int x=0;
	unsigned int y=0;
	int depth = 8;
	int pixel_size = 3;
	int colorType = PNG_COLOR_TYPE_RGB;

	bool status = false;

	if(bitsPerPixel == 32)
	{
		pixel_size = 4;
		colorType = PNG_COLOR_TYPE_RGBA;
	}

	FILE * fp;
	fp = fopen(savePath, "wb");
	if(!fp)
		return status;

	png_ptr = png_create_write_struct (PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (png_ptr == NULL)
	{
		 fclose (fp);
		 return status;
	}

	info_ptr = png_create_info_struct (png_ptr);
	if (info_ptr == NULL)
	{
		 png_destroy_write_struct (&png_ptr, &info_ptr);
		 fclose (fp);
		 return status;
	}
	if (setjmp (png_jmpbuf (png_ptr)))
	{
		png_destroy_write_struct (&png_ptr, &info_ptr);
		fclose (fp);
		return status;
	}

	png_set_IHDR (png_ptr,
				  info_ptr,
				  width,
				  height,
				  depth,
				  colorType,
				  PNG_INTERLACE_NONE,
				  PNG_COMPRESSION_TYPE_DEFAULT,
				  PNG_FILTER_TYPE_DEFAULT);

	row_pointers = (png_byte**) png_malloc (png_ptr, height * sizeof (png_byte *));		//// Initialize rows of PNG.

	int index = 0;
	for (y=0; y <(unsigned int)height; ++y)
	{
		png_byte* row = (png_byte*) png_malloc (png_ptr, sizeof(unsigned char)*width*pixel_size);
		
		row_pointers[y] = row;
		//row_pointers[height-1-y] = row;
	    
		for (x=0; x<(unsigned int)(width*pixel_size); ++x)
		{
			*row++ = rawData[index];
			index++;
		}
	}

	/* Write the image data to "fp". */
	png_init_io (png_ptr, fp);
	png_set_rows (png_ptr, info_ptr, row_pointers);
	png_write_png (png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, NULL);


	///*********************************************************************************/

   if (info_ptr->valid & PNG_INFO_IDAT)
   {
		if (png_ptr == NULL)
			return status;

		#ifdef PNG_WRITE_INTERLACING_SUPPORTED
			num_pass = png_set_interlace_handling(png_ptr);
		#else
			num_pass = 1;
		#endif

		pass = 0;
		rp = info_ptr->row_pointers;
		rowIndex = 0;
	}

   return true;
}


int PNGReadWrite::WritePercent(float percent)
{
	for(; pass < num_pass; pass++)
	{
		for(; rowIndex < png_ptr->height; rowIndex++, rp++)
		{
			png_write_row(png_ptr, *rp);

			currPercent = rowIndex * 100.0f / png_ptr->height;
			currPercent /= num_pass;

			if(currPercent - prevPercent >= percent)
			{
				printf("\n WritePercent : %f", currPercent);
				prevPercent = currPercent;
				return (int)currPercent;
			}
		}
	}

	return 100;
}

void PNGReadWrite::EndWrite()
{
	png_write_end(png_ptr, info_ptr);

	for (int y = 0; y < _height; y++)
        png_free (png_ptr, row_pointers[y]);

	png_free (png_ptr, row_pointers);
	png_destroy_write_struct (&png_ptr, &info_ptr);
	fclose (fpToWrite);

	printf("PNG writing ended...");
}