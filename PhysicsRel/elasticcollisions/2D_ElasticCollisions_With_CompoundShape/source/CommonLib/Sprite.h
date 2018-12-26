#ifndef SPRITE_H
#define SPRITE_H
#pragma once

#ifdef _WINDOWS
#include <windows.h>
#endif
#include <gl/gl.h>

#include "Ref.h"
//#include "Component.h"
#include "../MathLib/Posf.h"

class Sprite //: public Component
{
private:
	bool _visible;

	float _x;
	float _y;
	float _drawWidth;
	float _drawHeight;

	float _u;
	float _v;
	float _cropWidth;
	float _cropHeight;

	int _red;
	int _green;
	int	_blue;
	int	_alpha;
	unsigned int _color;

	bool _rotateBasedOnCenter;

	int _cX;
	int _cY;
	int _cW;
	int _cH;
	bool _isClipped;

	float _angle;
	float _centerX;
	float _centerY;

	int _cellWidth;
	int _cellHeight;

	void Reset(float x, float y, float w, float h, float u, float v, float cropWidth, float cropHeight);

	float _midX;
	float _midY;
	ImageInfo* _texture;

public:

	string FilePath;
	bool _flip;
	bool _horizontalFlip;
	float _minVertexX;
	float _maxVertexX;

	bool _actionPerformedForSelection;

	Sprite();
	~Sprite();

	void Init(float x, float y, float drawWidth, float drawHeight, int bpp);
	void Init(const char *filePath, float x, float y, float drawWidth, float drawHeight, bool filterNearest = false);

	ImageInfo* GetTexture();
	void SetTexture(ImageInfo* imageInfo);

	void Draw();
	void DrawFromCenter();
	void DrawInLocalAxis();
	void DrawImageRegion(float x, float y, float w, float h, float u, float v, float cropWidth, float cropHeight);

	bool ClipImage(int x, int y, int w, int h);
	bool Intersects(float rectX, float rectY, float rectWidth, float rectHeight);
	bool Contains(float x, float y, float addOffSet=0);
	void SetFlip(bool filp);
	void SetHorizantalFlip(bool horizantalFlip);

	bool HitTest(float hitX, float hitY);
	void GetPixelColor(int x, int y, int* colorsArr);

	void SetPixelColorFromBottom( int x, int y, int r, int g, int b, int a);
	void SetPixelColor(int x, int y, int r, int g, int b, int a);
	void SetPixelColorForRawData(int x, int y, int r, int g, int b, int a);
	void DeleteAndCreateWithRawData();

	void SetMidX(float midX);
	void SetMidY(float midY);
	float GetMidX();
	float GetMidY();

	void MoveXY(float dx, float dy);
	
	void SetX(float x);
	void SetY(float y);
	void SetPos(float x, float y);
	void SetWidth(float drawWidth);
	void SetHeight(float drawHeight);
	void SetSize(float drawWidth, float drawHeight);
	void SetBounds(float x, float y, float drawWidth, float drawHeight);

	float GetX();
	float GetY();
	float GetWidth();
	float GetHeight();

	float GetU_0To1();
	float GetV_0To1();
	float GetCropWidth_0To1();
	float GetCropHeight_0To1();

	int GetU_InPixels();
	int GetV_InPixels();
	int GetCropWidth_InPixels();
	int GetCropHeight_InPixels();

	void SetU_0To1(float u);
	void SetV_0To1(float v);
	void SetCropWidth_0To1(float cw);
	void SetCropHeight_0To1(float ch);
	void SetUVBounds_0To1(float u, float v, float cropWidth, float cropHeight);

	void SetU_InPixels(int u);
	void SetV_InPixels(int v);
	void SetCropWidth_InPixels(int cw);
	void SetCropHeight_InPixels(int ch);
	void SetUVBounds_InPixels(int u, int v, int cropWidth, int cropHeight);

	void SetVisible(bool visible);
	bool IsVisible();

	void SetColor(int red, int green, int blue, int alpha);
	void SetColor(unsigned int color);
	void SetRed(int red);
	void SetGreen(int green);
	void SetBlue(int blue);
	void SetAlpha(int alpha);
	void SetClipArea(int x, int y, int w, int h);
	void DisableClipArea();

	void SetRotation(float angle);
	void AddRotation(float angle, int pivotX, int pivotY);

	float GetRotation();

	int GetColor();
	int GetRed();
	int GetGreen();
	int GetBlue();
	int GetAlpha();

	Sprite* Clone();

	void Terminate();
};

#endif