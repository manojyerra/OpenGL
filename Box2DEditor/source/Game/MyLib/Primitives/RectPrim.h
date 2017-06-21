#ifndef RECT_PRIM_H
#define RECT_PRIM_H

#include "BasePrim.h"
#include "AddData.h"

class RectPrim : public BasePrim
{
private:

public:
	float x;
	float y;
	float w;
	float h;

	RectPrim();
	RectPrim(float x, float y, float width, float height, float angle, unsigned int color);
	~RectPrim();

	bool Contains(float xPos, float yPos);
	bool CollidesWithRect(float xPos, float yPos, float width, float height);
	Point GetCenter();
	float GetCX();
	float GetCY();
	void Draw();

	////////////////////////////////////

	void SetRotation(float angle);
	float GetRotation();

	void SetPos(float xPos, float yPos);
	void SetSize(float width, float heiht);
	void SetBounds(float xPos, float yPos, float width, float height);	

	static void Draw(float x, float y, float w, float h, float angle, unsigned int color, AddData* addData = NULL);
	static void DrawBorder(float x, float y, float w, float h, float lineWidth, unsigned int color);
};
#endif



//static void DrawBatch(float x, float y, float w, float h, AddData* addData);
//static void Draw(int x, int y, int w, int h, int r1, int g1, int b1, int a1, int r2, int g2, int b2, int a2);
//void Draw(AddData* addData);
//static void GetVertex(float x, float y, float w, float h, float* ver);
