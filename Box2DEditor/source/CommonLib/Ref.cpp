#include "Ref.h"
#include "../libpng/png.h"
#include "../CommonLib/DialogBox.h"

extern "C" {
	FILE* myFileOpen(const char *path, const char *mode);
	size_t myFileRead(unsigned char* dest, size_t size, size_t nmemb, FILE *stream);
	void myFileClose(FILE* fp);
}

Ref* Ref::_ref = NULL;
int ImageInfo::totalObjectsCount = 0;

ImageInfo::ImageInfo()
{
	filePath = "";
	textureID = 0;
	width = 0;
	height = 0;
	refCount = 1;
	format = 0;
	imageRawData = NULL;
	isMyImage = false;
	totalObjectsCount++;
	objectNum = totalObjectsCount; 
	if(objectNum == 2)
		printf("mem leak here");

	data = 0;
}


Ref::Ref()
{
}

Ref::~Ref()
{
}

Ref* Ref::GetInstance()
{
	if (_ref == NULL)
	{
		_ref = new Ref();
		ImageInfo::totalObjectsCount = 0;
	}
	return _ref;
}

void Ref::Init()
{
}

ImageInfo* Ref::GetTexture(string filePath, bool filterNearest, bool holdImageData)
{
	for(int num=0; num<(int)_imageInfoVec.size(); num++)
	{
		//if(stricmp(_imageInfoVec[num]->filePath.c_str(), filePath.c_str() ) == 0 )
		if(_imageInfoVec[num]->filePath.compare(filePath) == 0)
		{
			_imageInfoVec[num]->refCount++;
			return(_imageInfoVec[num]);
		}
	}

	ImageInfo* imageInfo = CreateTexture(filePath.c_str(), filterNearest, holdImageData);
	_imageInfoVec.push_back(imageInfo);

	return imageInfo;
}


ImageInfo* Ref::CreateTexture(const char* filename, bool filterNearest, bool holdImageData)
{
	string fileNameStr(filename);
	ImageInfo* imageInfo;
	
	if(fileNameStr.find(".png") != -1 || fileNameStr.find(".PNG") != -1)
	{
		imageInfo = LoadPNG(filename, filterNearest, holdImageData);
	}
	else if(fileNameStr.find(".my") != -1 || fileNameStr.find(".MY") != -1)
	{
		imageInfo = LoadMyImage(filename, filterNearest, holdImageData);
	}

	//else if(fileNameStr.find(".pvr") != -1)
	//{
	//	imageInfo = (ImageInfo*)GLNew(new ImageInfo(), sizeof(ImageInfo));

	//	uint32 texID;

	//	if(!PVRTLoadTextureFromPVR((char*)filename, &texID, imageInfo))
	//		printf("**ERROR** Failed to load texture %s.\n", filename);
	//}

	return imageInfo;
}


ImageInfo* Ref::LoadMyImage(const char* fileName, bool filterNearest, bool holdImageData)
{
	//ImageInfo* imageInfo = (ImageInfo*)GLNew(new ImageInfo(), sizeof(ImageInfo));

	//unsigned char* arrData;

	////s3eFile* g_FileHandle;
	//FILE* g_FileHandle;

	//g_FileHandle = fopen(fileName, "rb");
	//fseek(g_FileHandle, 0, SEEK_END);
	//int fileSize = ftell(g_FileHandle);
	//fseek(g_FileHandle, 0, SEEK_SET);

	////g_FileHandle = s3eFileOpen(fileName, "rb");
	////int fileSize = s3eFileGetSize(g_FileHandle);

	//arrData = (unsigned char*)GLMalloc(fileSize);
	////arrData = (unsigned char*)s3eMallocBase(fileSize+1);

	////memset(arrData, 0, s3eFileGetSize(g_FileHandle));

	////if (s3eFileRead(&arrData[0], filesize, 1, g_FileHandle) == 1)
	////{
	////	arrData[s3eFileGetSize(g_FileHandle)] = '\0';
	////	s3eDebugTracePrintf("\nLoaded %s", fileName);
	////}
	////else
	////	s3eDebugTracePrintf("Error while loading %s", fileName);

	//if(fread(&arrData[0], fileSize, 1, g_FileHandle) == 0)
	//	s3eDebugTracePrintf("Error while loading %s", fileName);

	////if (g_FileHandle)
	////	s3eFileClose(g_FileHandle);

	//if (g_FileHandle)
	//	fclose(g_FileHandle);

	//GLuint textureID;
	//glGenTextures(1, &textureID);
	//glBindTexture(GL_TEXTURE_2D, textureID);

	//glTexImage2D(GL_TEXTURE_2D,0, GL_LUMINANCE, 512, 512, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, (GLvoid*)arrData);

	//if(filterNearest)
	//{
	//	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//}
	//else
	//{
	//	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//}

	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	//imageInfo->textureID = textureID;
	//imageInfo->width = 512;
	//imageInfo->height = 512;
	//imageInfo->format = GL_LUMINANCE;
	//imageInfo->isMyImage = true;
	//string str(fileName);
	//imageInfo->filePath = str;
	//imageInfo->imageRawData = arrData;
	////s3eFreeBase(arrData);						//if not storing the arrDat in ImageInfo::imageRawData, then uncomment this line

	//return imageInfo;

	return NULL;
}


ImageInfo* Ref::LoadPNG(const char* filename, bool filterNearest, bool holdImageData)
{
	ImageInfo* imageInfo = new ImageInfo();
	png_byte header[8]; //8 is the maximum header size that can be checked
 	
	FILE* fp = NULL;

	#ifdef PLATFORM_WIN32
		string filePath_InWin32(filename);
		filePath_InWin32 = "data/"+filePath_InWin32;
		//fp = fopen(filePath_InWin32.c_str(), "rb");		//Changed to MYFILE
		fp = myFileOpen(filePath_InWin32.c_str(), "rb");
	#endif

	#ifdef PLATFORM_MARMALADE
		//fp = fopen(filename, "rb");		//Changed to MYFILE
		fp = myFileOpen(filename, "rb");
	#endif


	if(!fp)
	{
		char arr[256];
		sprintf(arr,"File: %s not found", filename);
		Show(arr);

		return imageInfo;
	}
	//fread(header, 1, 8, fp);	//Changed to MYFILE
	myFileRead(header, 1, 8, fp);

	if(png_sig_cmp(header, 0, 8)) 
	{
		myFileClose(fp);
		return imageInfo;
	}

	png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);	//initialize stuff
	if(!png_ptr)
	{
		myFileClose(fp);
		return imageInfo;
	}

	png_infop info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr)
	{
		png_destroy_read_struct(&png_ptr, (png_infopp) NULL, (png_infopp) NULL);
		myFileClose(fp);
		return imageInfo;
	}

	png_infop end_info = png_create_info_struct(png_ptr);
	if (!end_info)
	{
		png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp) NULL);
		myFileClose(fp);
		return imageInfo;
	}

	//png error stuff, not sure libpng man suggests this.
	if (setjmp(png_jmpbuf(png_ptr)))
	{
		png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
		myFileClose(fp);
		return imageInfo;
	}

	//init png reading
	png_init_io(png_ptr, (FILE*)fp);

	//let libpng know you already read the first 8 bytes
	png_set_sig_bytes(png_ptr, 8);

	// read all the info up to the image data
	png_read_info(png_ptr, info_ptr);

	//variables to pass to get info

	int pixle_depth;
	int bit_depth;
	int color_type;
	int twidth;
	int theight;
	
	// get info about png
	png_get_IHDR(png_ptr, info_ptr, (png_uint_32*)&twidth, (png_uint_32*)&theight, &bit_depth, &color_type, NULL, NULL, NULL);

	// Update the png info struct.
	png_read_update_info(png_ptr, info_ptr);

	pixle_depth = info_ptr->pixel_depth;

	// Row size in bytes.
	int rowbytes = png_get_rowbytes(png_ptr, info_ptr);

	// Allocate the image_data as a big block, to be given to opengl
	//png_byte *image_data = new png_byte[rowbytes * theight];
	png_byte *image_data = (png_byte*)malloc(sizeof(png_byte) * (rowbytes*theight));

	if (!image_data)
	{
		png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);	//clean up memory and close stuff
		myFileClose(fp);
		return imageInfo;
	}

	//row_pointers is for pointing to image_data for reading the png with libpng
	//png_bytep *row_pointers = new png_bytep[theight];
	png_bytep *row_pointers = (png_bytep*)malloc(sizeof(png_bytep)*theight);

	if (!row_pointers)
	{
		png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);	//clean up memory and close stuff
		//delete[] image_data;
		free(image_data);
		myFileClose(fp);
		return imageInfo;
	}

	// set the individual row_pointers to point at the correct offsets of image_data
	for (int i = 0; i < theight; ++i)
		row_pointers[theight - 1 - i] = image_data + i * rowbytes;

	//read the png into image_data through row_pointers
	png_read_image(png_ptr, row_pointers);

	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	if(pixle_depth == 32)		glTexImage2D(GL_TEXTURE_2D,0, GL_RGBA, twidth, theight, 0, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*)image_data);
	else if(pixle_depth == 24)	glTexImage2D(GL_TEXTURE_2D,0, GL_RGB, twidth, theight, 0, GL_RGB, GL_UNSIGNED_BYTE, (GLvoid*)image_data);
	else						
	{
		//s3eDebugTracePrintf("Unknown pixle depth");
	}

	if(filterNearest)
	{
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}
	else
	{
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

	#ifdef PLATFORM_MARMALADE
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	#endif

	#ifdef PLATFORM_WIN32
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	#endif

	png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);


	//fclose(fp);	//Changed to MYFILE
	myFileClose(fp);

	imageInfo->textureID = textureID;
	imageInfo->width = twidth;
	imageInfo->height = theight;


#ifdef _DEBUG
	int twoPow = 1;

	bool widthIs2Pow = false;
	bool heightIs2Pow = false;
	for(int i=0;i<12;i++)
	{
		if(imageInfo->width == twoPow)
			widthIs2Pow = true;
		if(imageInfo->height == twoPow)
			heightIs2Pow = true;

		if(widthIs2Pow && heightIs2Pow )
			break;

		twoPow*=2;
	}

	if(widthIs2Pow == false || heightIs2Pow == false )
	{
		char arr[256];
		sprintf(arr,"File: %s dont have 2power dimentions ", filename);
		Show(arr);
	}
#endif


	if(!holdImageData)
	{
		free(image_data);
	}
	else
	{
		imageInfo->imageRawData = image_data;
	}

	free(row_pointers);

	if(pixle_depth == 32)		imageInfo->format = GL_RGBA;
	if(pixle_depth == 24)		imageInfo->format = GL_RGB;

	string str(filename);
	imageInfo->filePath = str;

	return imageInfo;
 }


//GLuint Ref::GetTextureID(const char* filename, bool filterNearest)
//{
//	GLuint textureID = 0;
//
//	png_byte header[8]; //8 is the maximum header size that can be checked
// 	
//	s3eFile  *fp = s3eFileOpen(filename, "rb");
//	if (!fp)
//		return textureID;
//
//	s3eFileRead(header, 1, 8, fp);
//	if(png_sig_cmp(header, 0, 8)) { s3eFileClose(fp); return textureID; }
//
//	png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);	//initialize stuff
//	if (!png_ptr) { s3eFileClose(fp); return textureID; }
//
//	png_infop info_ptr = png_create_info_struct(png_ptr);
//	if (!info_ptr)
//	{
//		png_destroy_read_struct(&png_ptr, (png_infopp) NULL, (png_infopp) NULL);
//		s3eFileClose(fp);
//		return textureID;
//	}
//
//	png_infop end_info = png_create_info_struct(png_ptr);
//	if (!end_info)
//	{
//		png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp) NULL);
//		s3eFileClose(fp);
//		return textureID;
//	}
//
//	//png error stuff, not sure libpng man suggests this.
//	if (setjmp(png_jmpbuf(png_ptr)))
//	{
//		png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
//		s3eFileClose(fp);
//		return textureID;
//	}
//
//	//init png reading
//	png_init_io(png_ptr, fp);
//
//	//let libpng know you already read the first 8 bytes
//	png_set_sig_bytes(png_ptr, 8);
//
//	// read all the info up to the image data
//	png_read_info(png_ptr, info_ptr);
//
//	//variables to pass to get info
//
//	int pixle_depth;
//	int bit_depth;
//	int color_type;
//	int twidth;
//	int theight;
//	
//	// get info about png
//	png_get_IHDR(png_ptr, info_ptr, (png_uint_32*)&twidth, (png_uint_32*)&theight, &bit_depth, &color_type, NULL, NULL, NULL);
//
//	// Update the png info struct.
//	png_read_update_info(png_ptr, info_ptr);
//
//	pixle_depth = info_ptr->pixel_depth;
//
//	// Row size in bytes.
//	int rowbytes = png_get_rowbytes(png_ptr, info_ptr);
//
//	// Allocate the image_data as a big block, to be given to opengl
//	//png_byte *image_data = new png_byte[rowbytes * theight];
//	png_byte *image_data = (png_byte*)GLMalloc(sizeof(png_byte) * (rowbytes*theight));
//
//	if (!image_data)
//	{
//		png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);	//clean up memory and close stuff
//		s3eFileClose(fp);
//		return textureID;
//	}
//
//	//row_pointers is for pointing to image_data for reading the png with libpng
//	//png_bytep *row_pointers = new png_bytep[theight];
//	png_bytep *row_pointers = (png_bytep*)GLMalloc(sizeof(png_bytep)*theight);
//
//	if (!row_pointers)
//	{
//		png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);	//clean up memory and close stuff
//		//delete[] image_data;
//		GLFree(image_data);
//		s3eFileClose(fp);
//		return textureID;
//	}
//
//	// set the individual row_pointers to point at the correct offsets of image_data
//	for (int i = 0; i < theight; ++i)
//		row_pointers[theight - 1 - i] = image_data + i * rowbytes;
//
//	//read the png into image_data through row_pointers
//	png_read_image(png_ptr, row_pointers);
//
//
//	glGenTextures(1, &textureID);
//	MyGL::GetInstance()->gl_BindTexture(GL_TEXTURE_2D, textureID);
//
//	if(pixle_depth == 32)		MyGL::GetInstance()->gl_TexImage2D(GL_TEXTURE_2D,0, GL_RGBA, twidth, theight, 0, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*)image_data);
//	else if(pixle_depth == 24)	MyGL::GetInstance()->gl_TexImage2D(GL_TEXTURE_2D,0, GL_RGB, twidth, theight, 0, GL_RGB, GL_UNSIGNED_BYTE, (GLvoid*)image_data);
//	else						s3eDebugTracePrintf("Unknown pixle depth");
//
//	if(filterNearest)
//	{
//		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//	}
//	else
//	{
//		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//	}
//
//	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//
//	png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
//
//	GLFree(image_data);
//	GLFree(row_pointers);
//
//	s3eFileClose(fp);
//
//	return textureID;
// }


//void Ref::DeleteAllTextures()
//{
//	for(int i=0;i<(int)_imageInfoVec.size();i++)
//		MyGL::GetInstance()->gl_DeleteTextures(1,&_imageInfoVec[i]->textureID);
//}
//
//void Ref::ReLoadAllTextures()
//{
//	for(int i=0;i<(int)_imageInfoVec.size();i++)
//		_imageInfoVec[i]->textureID = GetTextureID(_imageInfoVec[i]->filePath.c_str());
//}


void Ref::UnLoadAll()
{
	for(int i=0;i<(int)_imageInfoVec.size();i++)
	{
		glDeleteTextures(1,&_imageInfoVec[i]->textureID);

		if(_imageInfoVec[i]->isMyImage && _imageInfoVec[i]->imageRawData)
			free(_imageInfoVec[i]->imageRawData);

		delete _imageInfoVec[i];
		_imageInfoVec[i] = NULL;

		_imageInfoVec.erase(_imageInfoVec.begin() + i);
	}
}


void Ref::UnLoad(ImageInfo* imageInfo)
{
	for(int i=0;i<(int)_imageInfoVec.size();i++)
	{
		if(_imageInfoVec[i] == imageInfo)
		{
			int refCount = _imageInfoVec[i]->refCount;
			_imageInfoVec[i]->refCount--;

			if(_imageInfoVec[i]->refCount == 0)
			{
				glDeleteTextures(1,&_imageInfoVec[i]->textureID);

				if(_imageInfoVec[i]->imageRawData)
					free(_imageInfoVec[i]->imageRawData);

				delete _imageInfoVec[i];
				_imageInfoVec[i] = NULL;

				_imageInfoVec.erase(_imageInfoVec.begin() + i);
				return;
			}
		}
	}
}

void Ref::Terminate()
{
	_imageInfoVec.clear();

	if(_ref)
	{
		delete  _ref;
		_ref = NULL;
	}
}


