#include "TextureManager.h"
#include "png.h"
#include "Platform.h"
#include "PNGReadWrite.h"
#include "JPEGReadWrite.h"
#include "Utils.h"

TextureManager* TextureManager::_ref = NULL;

TextureManager::TextureManager()
{
	_val = 0;
	_gray = false;
}

TextureManager* TextureManager::GetInstance()
{
	if(_ref == NULL)
		_ref = new TextureManager();

	return _ref;
}

TextureInfo* TextureManager::CreateTexture(string filePath)
{
	return CreateTexture(filePath, Platform::READ_FROM_ASSETS);
}

TextureInfo* TextureManager::CreateTexture(string filePath, int readFrom)
{
	//Check if texture already exists...

	for(int num=0; num<(int)_textureInfoVec.size(); num++)
	{
		if(_textureInfoVec[num]->filePath.compare(filePath) == 0)
		{
			_textureInfoVec[num]->refCount++;
			return(_textureInfoVec[num]);
		}
	}

	//Create texture if not already exists...

	if(filePath.find(".png") != -1 || filePath.find(".PNG") != -1)
	{
		TextureInfo* textureInfo = LoadPNG(filePath, readFrom);
		_textureInfoVec.push_back(textureInfo);
		return textureInfo;
	}
	else if(filePath.find(".jpg") != -1 || filePath.find(".jpeg") != -1 || filePath.find(".JPG") != -1 || filePath.find(".JPEG") != -1)
	{
		TextureInfo* textureInfo = LoadJPEG(filePath, readFrom);
		_textureInfoVec.push_back(textureInfo);
		return textureInfo;
	}

	return NULL;
}

void TextureManager::SetData(int val)
{
	_val = val;
}

int TextureManager::GetData()
{
	return _val;
}

void TextureManager::MakeGrayScale(bool gray)
{
	if(_gray != gray)
	{
		_gray = gray;
		
		DeleteAllTextures();
		CreateAllTextures();
	}
}

TextureInfo* TextureManager::LoadPNG(string filePath, int readFrom)
{
	TextureInfo* textureInfo = new TextureInfo();
	textureInfo->filePath = filePath;

	int imgFileSize = 0;
	char* imgBuf = Platform::GetPlatform()->GetFileData(filePath, &imgFileSize, readFrom);
	unsigned char* imageRawData = PNGReadWrite::Read(imgBuf, imgFileSize,  &textureInfo->width,
														&textureInfo->height, &textureInfo->bytesPerPixel);

	if(_gray)
	{
		int bufSize = textureInfo->width * textureInfo->height * textureInfo->bytesPerPixel;	
		MakeGrayBuffer(imageRawData, bufSize, textureInfo->bytesPerPixel);
	}

#ifdef _PLATFORM_ANDROID

	if(imageRawData)
	{
		if( !Is2Power(textureInfo->height) || !Is2Power(textureInfo->width) )
		{
			unsigned char* newMem = Get2PowerImgBuffer(imageRawData, textureInfo);
			free(imageRawData);
			imageRawData = newMem;
		}
	}
#endif

	free(imgBuf);

	int format = 0;
    if(textureInfo->bytesPerPixel == 4)	format = GL_RGBA;
	else if(textureInfo->bytesPerPixel == 3)	format = GL_RGB;

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glGenTextures(1, &textureInfo->textureID);
	glBindTexture(GL_TEXTURE_2D, textureInfo->textureID);
    glTexImage2D(GL_TEXTURE_2D,0, format, textureInfo->width, textureInfo->height, 0, format,
                 GL_UNSIGNED_BYTE, (GLvoid*)imageRawData);

	{
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
    
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	free(imageRawData);
    imageRawData = NULL;

	return textureInfo;
}

TextureInfo* TextureManager::LoadJPEG(string filePath, int readFrom)
{
	TextureInfo* textureInfo = new TextureInfo();
	textureInfo->filePath = filePath;
	textureInfo->readFrom = readFrom;

	int imgFileSize = 0;
	unsigned char* imageRawData = NULL;

	if(readFrom == Platform::READ_FROM_ASSETS)
	{
		char* fileData = Platform::GetPlatform()->GetFileData(filePath, &imgFileSize, readFrom);

		char pathAtIM[256];
		sprintf(pathAtIM,"%s/abcxyz123.jpg",Platform::GetPlatform()->GetSaveFolderPath().c_str());

		FILE* fp = fopen(pathAtIM, "wb");
		fwrite(fileData, imgFileSize, 1, fp);
		fflush(fp);
		fclose(fp);

		imageRawData = JPEGReadWrite::Read(pathAtIM, &textureInfo->width,
															&textureInfo->height, &textureInfo->bytesPerPixel);

		if(_gray)
		{
			int bufSize = textureInfo->width * textureInfo->height * textureInfo->bytesPerPixel;	
			MakeGrayBuffer(imageRawData, bufSize, textureInfo->bytesPerPixel);
		}
	
		if(imageRawData)
		{
			if( !Is2Power(textureInfo->height) || !Is2Power(textureInfo->width) )
			{
				unsigned char* newMem = Get2PowerImgBuffer(imageRawData, textureInfo);
				free(imageRawData);
				imageRawData = newMem;
			}
		}

		free(fileData);
	}
	else if(readFrom == Platform::READ_FROM_IM)
	{
		char pathAtIM[256];
		sprintf(pathAtIM,"%s/%s",Platform::GetPlatform()->GetSaveFolderPath().c_str(), filePath.c_str());

		imageRawData = JPEGReadWrite::Read(pathAtIM, &textureInfo->width,
															&textureInfo->height, &textureInfo->bytesPerPixel);

		if(imageRawData)
		{
			if( !Is2Power(textureInfo->height) || !Is2Power(textureInfo->width) )
			{
				unsigned char* newMem = Get2PowerImgBuffer(imageRawData, textureInfo);
				free(imageRawData);
				imageRawData = newMem;
			}
		}
	}

	int format = 0;
    if(textureInfo->bytesPerPixel == 4)	format = GL_RGBA;
	else if(textureInfo->bytesPerPixel == 3)	format = GL_RGB;

	glGenTextures(1, &textureInfo->textureID);
	glBindTexture(GL_TEXTURE_2D, textureInfo->textureID);
    glTexImage2D(GL_TEXTURE_2D,0, format, textureInfo->width, textureInfo->height, 0, format,
                 GL_UNSIGNED_BYTE, (GLvoid*)imageRawData);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	free(imageRawData);
    imageRawData = NULL;

	return textureInfo;	
}

void TextureManager::MakeGrayBuffer(unsigned char* buf, int size, int bytesPP)
{
	for(int i=0; i<size; i+=bytesPP)
	{
		int val = (buf[i] + buf[i+1] + buf[i+2])/3;
		buf[i] = buf[i+1] = buf[i+2] = val;
	}
}

void TextureManager::DeleteAllTextures()
{
	printLog("Going to delete %d textures", _textureInfoVec.size());
	for(int i=0;i<(int)_textureInfoVec.size();i++)
	{
		glDeleteTextures(1,&_textureInfoVec[i]->textureID);
	}
}

void TextureManager::CreateAllTextures()
{
	printLog("Going to create %d textures", _textureInfoVec.size());
	for(int i=0;i<(int)_textureInfoVec.size();i++)
	{
		string* filePath = &(_textureInfoVec[i]->filePath);

		if(filePath->find(".png") != -1 || filePath->find(".PNG") != -1)
		{
			TextureInfo* textureInfo = LoadPNG(_textureInfoVec[i]->filePath, _textureInfoVec[i]->readFrom);
			_textureInfoVec[i]->textureID = textureInfo->textureID;
			delete textureInfo;
		}
		else if(filePath->find(".jpg") != -1 || filePath->find(".jpeg") != -1 || filePath->find(".JPG") != -1 || filePath->find(".JPEG") != -1)
		{
			TextureInfo* textureInfo = LoadJPEG(_textureInfoVec[i]->filePath, _textureInfoVec[i]->readFrom);
			_textureInfoVec[i]->textureID = textureInfo->textureID;
			delete textureInfo;
		}
	}
}

void TextureManager::DeleteTexture(TextureInfo* textureInfo)
{
	for(int i=0;i<(int)_textureInfoVec.size();i++)
	{
		if(_textureInfoVec[i] == textureInfo)
		{
			int refCount = _textureInfoVec[i]->refCount;
			_textureInfoVec[i]->refCount--;

			if(_textureInfoVec[i]->refCount == 0)
			{
				glDeleteTextures(1,&_textureInfoVec[i]->textureID);

				//if(_textureInfoVec[i]->imageRawData)
				//	free(_textureInfoVec[i]->imageRawData);

				delete _textureInfoVec[i];
				_textureInfoVec[i] = NULL;

				_textureInfoVec.erase(_textureInfoVec.begin() + i);
			}
			return;
		}
	}
}

TextureManager::~TextureManager()
{
	_textureInfoVec.clear();
}

void TextureManager::DeleteInstance()
{
	if(_ref)
	{
		delete _ref;
		_ref = NULL;
	}
}

unsigned char* TextureManager::Get2PowerImgBuffer(unsigned char* mem, TextureInfo* textureInfo)
{
	int ow = textureInfo->width;
	int oh = textureInfo->height;

	int nw = GetNear2Power(ow);
	int nh = GetNear2Power(oh);

	int bytesPP = textureInfo->bytesPerPixel;

	unsigned char* newMem = (unsigned char*)malloc( nw*nh*bytesPP );
	memset(newMem, '\0', nw*nh*bytesPP);

	for(int i=0;i<oh; i++)
		memcpy(&(newMem[i*nw*bytesPP]), &(mem[i*ow*bytesPP]), ow*bytesPP);

	textureInfo->contentW = ow;
	textureInfo->contentH = oh;

	textureInfo->width = nw;
	textureInfo->height = nh;

	return newMem;
}

int TextureManager::GetNear2Power(float val)
{
	if(val <= 1) return 1;
	else if(val <= 2) return 2;
	else if(val <= 4) return 4;
	else if(val <= 8) return 8;
	else if(val <= 16) return 16;
	else if(val <= 32) return 32;
	else if(val <= 64) return 64;
	else if(val <= 128) return 128;
	else if(val <= 256) return 256;
	else if(val <= 512) return 512;
	else if(val <= 1024) return 1024;
	else if(val <= 2048) return 2048;
	else if(val <= 4096) return 4096;

	return -1;
}

int TextureManager::Is2Power(float val)
{
	return (GetNear2Power(val) == val);
}
