#ifndef REF_H
#define REF_H
#pragma once

#include "../Platform/Platform.h"
#include <vector>
#include <string>
using namespace std;

class ImageInfo
{
public:
	string filePath;
	GLuint textureID;
	int width;
	int height;
	GLuint format;
	int refCount;
	int objectNum;
	unsigned char* imageRawData;
	static int totalObjectsCount;
	bool isMyImage;
	int data;
	ImageInfo();

};


class Ref
{
private:
	static Ref* _ref;
	Ref();
	~Ref();
	vector<ImageInfo*> _imageInfoVec;
	ImageInfo* CreateTexture(const char* filename, bool filterNearest, bool holdImageData);
	ImageInfo* LoadPNG(const char* filename, bool filterNearest, bool holdImageData);
	ImageInfo* LoadMyImage(const char* filename, bool filterNearest, bool holdImageData);
	//GLuint GetTextureID(const char* filename, bool filterNearest = false);

public:
	static Ref* GetInstance();
	void Init();
	ImageInfo* GetTexture(string filePath, bool filterNearest, bool holdImageData);
	void UnLoad(ImageInfo* imageInfo);
	void UnLoadAll();

	//void DeleteAllTextures();
	//void ReLoadAllTextures();

	void Terminate();
};

#endif