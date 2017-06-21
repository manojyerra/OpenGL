#ifndef SPRITE_H
#define SPRITE_H
#pragma once

#include "Includes.h"
#include "Posf.h"
#include "TextureManager.h"

class Sprite
{
private:
	bool _visible;

	float _x;
	float _y;
	float _drawWidth;
	float _drawHeight;

	int _u;
	int _v;
	int _cropWidth;
	int _cropHeight;

	int _red;
	int _green;
	int	_blue;
	int	_alpha;
	unsigned int _color;

	bool _rotateBasedOnCenter;

	float _angle;
	float _centerX;
	float _centerY;

	float _horFlip;
	float _verFlip;

	bool _isWaveOn;
	float _waveSpeed;
	float _waveHeight;
	float _waveInitAngle;

	void Reset(float x, float y, float w, float h, int u, int v, int cropWidth, int cropHeight);

public:
	TextureInfo* _texture;

	Sprite(string texPath, float x, float y, float drawWidth, float drawHeight);

	Sprite(const char *texture, float x, float y);
	Sprite(const char *texture, float x, float y, float drawWidth, float drawHeight);
	Sprite(const char *texture, float x, float y, float drawWidth, float drawHeight, int readFrom);
	Sprite(const char *texture, float x, float y, float drawWidth, float drawHeight, float u, float v, float cw, float ch);

	//Sprite(const char *texture, float x, float y);
	//Sprite(const char *texture, float x, float y, float drawWidth, float drawHeight, bool filterNearest = false, bool holdImageData = false);
	//Sprite(const char *texture, float x, float y, float drawWidth, float drawHeight, int u, int v, int cropWidth, int cropHeight, bool filterLinear = false, bool holdImageData = false);

	//Sprite(TextureInfo* textureInfo, float x, float y, float drawWidth, float drawHeight);
	//Sprite(TextureInfo* textureInfo, float x, float y, float drawWidth, float drawHeight, int u, int v, int cropWidth, int cropHeight);
	
	TextureInfo* GetTextureInfo();

	void GetVertexAndUVs(float* ver, float* uvs);

	void Draw();
	void DrawFromCenter();
	void DrawImageRegion(float x, float y, float w, float h, int u, int v, int cropWidth, int cropHeight);

	void SetWaveEffect(bool isWaveOn);
	void SetWaveSpeed(float waveSpeed);
	void SetWaveHeight(float waveHeight);

	//bool CollidesWith(Sprite* img);
	bool Intersects(float rectX, float rectY, float rectWidth, float rectHeight);
	bool Contains(float x, float y);
	bool Contains(float X, float Y, float offSetX, float offSetY);
	bool ContainsByAlphaTest(float x, float y);

	void SetX(float x);
	void SetY(float y);
	void SetPos(float x, float y);
	void SetWidth(float drawWidth);
	void SetHeight(float drawHeight);
	void SetSize(float drawWidth, float drawHeight);
	void SetBounds(float x, float y, float drawWidth, float drawHeight);
	void SetMidPos(float midX, float midY);

	float GetX();
	float GetY();
	float GetWidth();
	float GetHeight();

	int GetU();
	int GetV();

	void SetU(int u);
	void SetV(int v);

	void SetUVBoundsInPixels(int u, int v, int cropWidth, int cropHeight);
	void SetUVBounds0To1(float u, float v, float cw, float ch);
	void SetUVToContent();
	
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

	void SetFlipHor(bool val);
	void SetFlipVer(bool val);

	bool GetFlipHor();
	bool GetFlipVer();

	void SetRotation(float angle);
	void SetRotation(float angle, int centerX, int centerY);
	float GetRotation();
	Posf GetCenterInRotation();

	int GetColor();
	int GetRed();
	int GetGreen();
	int GetBlue();
	int GetAlpha();

	void RotateUVsInClockWise();
	void RotateUVsInAntiClockWise();
	void SetColorOnRawData(float x1, float y1, float x2, float y2, int r, int g, int b, int a);

	~Sprite();
};

#endif
