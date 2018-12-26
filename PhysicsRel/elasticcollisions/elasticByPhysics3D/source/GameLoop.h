#ifndef GAME_LOOP
#define GAME_LOOP
#pragma once
#include "MathLib/point.h"
#include "MathLib/MyVector.h"
#include "Ball.h"
#include "mathlib/Line2D.h"
#include "mathlib/Plane.h"

#include <vector>
using namespace std;

class GameLoop
{
private:

public:
	static const int SCREEN_WIDTH = 800;
	static const int SCREEN_HEIGHT = 800;
	
	MyVector* lineVector;
	MyVector* rayVector;
	MyVector* normalVector;
	MyVector* refVector;

	Point* p1;
	Point* p2;
	Point* p3;
	Point* p4;

	Plane* _upBorder;
	Plane* _rightBorder;
	Plane* _downBorder;
	Plane* _leftBorder;
	Plane* _frontBorder;
	Plane* _backBorder;

	vector<Ball*> _balls;


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

	Point GetPerpendicular(Point* p);

	void Draw();
	void Terminate();

	float ABS(float val);
};

#endif
