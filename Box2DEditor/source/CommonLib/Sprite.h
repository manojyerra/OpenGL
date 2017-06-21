#ifndef SPRITE_H
#define SPRITE_H
#pragma once

#include "../MyDefines.h"
#include "Ref.h"
#include "../MathLib/Posf.h"
#include "../PhysicsLib/PhyBox.h"

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

	int _cX;
	int _cY;
	int _cW;
	int _cH;
	bool _isClipped;
	bool _flip;
	bool _horizontalFlip;

	float _angle;
	float _centerX;
	float _centerY;

	void Reset(float x, float y, float w, float h, int u, int v, int cropWidth, int cropHeight);

	bool _isWaveOn;
	float _waveSpeed;
	float _waveHeight;
	float _waveInitAngle;

	PhyBox* _phyBox;

public:
	ImageInfo* _texture;

	Sprite(const char *texture, float x, float y, float drawWidth, float drawHeight, bool filterNearest = false, bool holdImageData = false);
	Sprite(const char *texture, float x, float y, float drawWidth, float drawHeight, int u, int v, int cropWidth, int cropHeight, bool filterLinear = false, bool holdImageData = false);
	void SetAsPhysicsBody(bool val);
	void DrawImageRegion(float x, float y, float w, float h, int u, int v, int cropWidth, int cropHeight);

	ImageInfo* GetTexture();

	void Draw();
	void DrawFromCenter();
	void SetWaveEffect(bool isWaveOn);
	void SetWaveSpeed(float waveSpeed);
	void SetWaveHeight(float waveHeight);

	bool ClipImage(int x, int y, int w, int h);
	bool Intersects(float rectX, float rectY, float rectWidth, float rectHeight);
	bool Contains(float x, float y);
	void SetFlip(bool filp);
	void SetHorizantalFlip(bool horizantalFlip);

	void SetPixelColorFromBottom( int x, int y, int r, int g, int b, int a);
	void SetPixelColor(int x, int y, int r, int g, int b, int a);
	void SetPixelColorForRawData(int x, int y, int r, int g, int b, int a);
	void DeleteAndCreateWithRawData();

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

	int GetU();
	int GetV();

	void SetU(int u);
	void SetV(int v);

	void SetUVBounds(int u, int v, int cropWidth, int cropHeight);
	
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
	void SetRotation(float angle, int centerX, int centerY);

	float GetRotation();

	int GetColor();
	int GetRed();
	int GetGreen();
	int GetBlue();
	int GetAlpha();

	~Sprite();
};

#endif