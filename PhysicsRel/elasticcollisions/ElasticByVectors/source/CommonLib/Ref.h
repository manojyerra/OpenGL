#ifndef REF_H
#define REF_H
#pragma once

#include <vector>
#include <string>
using namespace std;

class ImageInfo
{
public:
	string filePath;
	unsigned int textureID;
	int width;
	int height;
	unsigned int format;
	int refCount;
	int objectNum;
	unsigned char* imageRawData;
	static int totalObjectsCount;
	bool isMyImage;

	ImageInfo();
};

class Ref
{
private:
	static Ref* _ref;
	Ref();
	vector<ImageInfo*> _imageInfoVec;
	ImageInfo* CreateTexture(const char* filename, bool filterNearest = false);
	ImageInfo* LoadPNG(const char* filename, bool filterNearest);
	//ImageInfo* LoadMyImage(const char* filename, bool filterNearest);
	//GLuint GetTextureID(const char* filename, bool filterNearest = false);

public:
	static Ref* GetInstance();
	void Init();
	ImageInfo* GetTexture(string filePath, bool filterNearest = false);
	ImageInfo* GetTexture(int width, int height, int bpp);
	void UnLoad(int textureID);
	//void DeleteAllTextures();
	//void ReLoadAllTextures();
	void Terminate();
};

#endif