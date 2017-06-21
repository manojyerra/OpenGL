#ifndef REF_H
#define REF_H
#pragma once

#include "Includes.h"
#include "TextureInfo.h"
#include <vector>
#include <string>
using namespace std;

class TextureManager
{
private:
	static TextureManager* _ref;
	int _val;
	bool _gray;

	TextureManager();
	~TextureManager();

	vector<TextureInfo*> _textureInfoVec;
	TextureInfo* LoadPNG(string filePath, int readFrom);
	TextureInfo* LoadJPEG(string filePath, int readFrom);

	int GetNear2Power(float val);
	int Is2Power(float val);
	unsigned char* Get2PowerImgBuffer(unsigned char* imageRawData, TextureInfo* textureInfo);
	void MakeGrayBuffer(unsigned char* buf, int size, int bytesPP);

public:
	static TextureManager* GetInstance();
	static void DeleteInstance();

	void MakeGrayScale(bool gray);
	
	TextureInfo* CreateTexture(string filePath);
	TextureInfo* CreateTexture(string filePath, int readFrom);

	void DeleteTexture(TextureInfo* textureInfo);
	
	void DeleteAllTextures();
	void CreateAllTextures();

	void SetData(int val);
	int GetData();
};

#endif
