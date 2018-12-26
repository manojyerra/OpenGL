#ifndef GAME_LOOP
#define GAME_LOOP
#pragma once
#include "MathLib/point.h"
#include "MathLib/MyVector.h"
#include "Ball.h"
#include "mathlib/Line2D.h"

#include <vector>
using namespace std;

class GameLoop
{
private:

public:
	static const int SCREEN_WIDTH = 1024;
	static const int SCREEN_HEIGHT = 600;
	
	MyVector* lineVector;
	MyVector* rayVector;
	MyVector* normalVector;
	MyVector* refVector;

	Point* p1;
	Point* p2;
	Point* p3;
	Point* p4;

	Line2D* _upBorderLine;
	Line2D* _rightBorderLine;
	Line2D* _downBorderLine;
	Line2D* _leftBorderLine;

	vector<Ball*> _balls;

	Sprite* _boardImg;

	//for compound shape

	float _initDist;

	GameLoop();
	~GameLoop();

	void DrawCircle( float x, float y, int r );
	Point ReflectionPt( Point p1, Point p2, Point cp, Point p);

	void Init();
	void InitGL();
	void Update();
	void CollisionWithBorder(Ball* ball);
	void CheckBallCollision(Ball* ball1, Ball* ball2);

	float GetFinalVelocity1(float m1, float m2, float u1, float u2);
	float GetFinalVelocity2(float m1, float m2, float u1, float u2);

	void Draw();
	void Terminate();
};

#endif
