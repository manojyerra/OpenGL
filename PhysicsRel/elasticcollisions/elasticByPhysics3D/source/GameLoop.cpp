#ifdef _WINDOWS
#include <windows.h>
#endif

#include <gl/gl.h>
#include <gl/glu.h>
#include "GameLoop.h"
#include "CommonLib/UIFont.h"
#include "mathlib/Collisions.h"
#include <math.h>
#include "mathlib/Vector3.h"
#include "mathlib/Mtx33.h"
#include "mathlib/Plane.h"
#include "Renderer.h"

GameLoop::GameLoop(){}
GameLoop::~GameLoop(){}

void GameLoop::Init()
{
	InitGL();
	UIFont::GetInstance()->Init();

	float x = 0;
	float y = 1;
	float z = 0;

	MyVector givenVec(x, y, z);
	givenVec.Normalize();
	Point perPoint = GetPerpendicular( &Point( &givenVec.p) );
	float dotVal = ABS(givenVec.Dot(&MyVector(&perPoint)));

	if(dotVal < 0.0001)
		printf("k");

	p1 = new Point(20,20);
	p2 = new Point(900,20);
	p3 = new Point(900,500);
	p4 = new Point(20,500);

	Ball* ball;


	for(int xx = -20; xx<= 20; xx+=20)
	{
		for(int yy = -20; yy<= 20; yy+=20)
		{
			for(int zz = -20; zz<= 20; zz+=20)
			{
				ball = new Ball(&Point(xx, yy, zz), 1);
				_balls.push_back(ball);
			}
		}
	}


	//ball = new Ball(&Point(-3, 0, 0), 2);
	//ball->SetVelocity(1.0f, -1.0f, 1);
	//ball->SetR(2);
	//_balls.push_back(ball);

	//ball = new Ball(&Point(3, 0, 0), 2);
	//ball->SetVelocity(-1.0f, -1.0f, 1);
	//ball->SetR(2);
	//_balls.push_back(ball);


	_rightBorder = new Plane(&Point(1,0,0), &Point(25,0,0));
	_leftBorder = new Plane(&Point(1,0,0), &Point(-25,0,0));

	_upBorder = new Plane(&Point(0,1,0), &Point(0,25,0));
	_downBorder = new Plane(&Point(0,1,0), &Point(0,-25,0));

	_frontBorder = new Plane(&Point(0,0,1), &Point(0,0,25));
	_backBorder = new Plane(&Point(0,0,1), &Point(0,0,-25));
}

float GameLoop::ABS(float val)
{
	if(val < 0)
		return -val;
	return val;
}

void GameLoop::Update()
{
	for(int i=0; i<_balls.size();i++)
		CollisionWithBorder(_balls[i]);

	for(int i=0; i<_balls.size();i++)
		for(int j=i+1; j<_balls.size();j++)
			CheckBallCollision(_balls[i], _balls[j]);

	for(int i=0; i<_balls.size();i++)
		_balls[i]->Update();
}

void GameLoop::CheckBallCollision(Ball* ball1, Ball* ball2)
{
	Point c1(&ball1->GetCenter());
	Point c2(&ball2->GetCenter());

	float r1 = ball1->GetR();
	float r2 = ball2->GetR();

	float dist = sqrt( (c2.x-c1.x)*(c2.x-c1.x) + (c2.y-c1.y)*(c2.y-c1.y) );

	if(dist <= r1+r2)
	{
		ball1->Move();
		ball2->Move();

		Point cc1(&ball1->GetCenter());
		Point cc2(&ball2->GetCenter());

		float newDist = sqrt( (cc2.x-cc1.x)*(cc2.x-cc1.x) + (cc2.y-cc1.y)*(cc2.y-cc1.y) );

		ball1->UndoMove();
		ball2->UndoMove();

		if(newDist <= dist)
		{
			Ball* b1 = ball1;
			Ball* b2 = ball2;

			float b1x = c1.x;
			float b1y = c1.y;
			float b1z = c1.z;

			float b2x = c2.x;
			float b2y = c2.y;
			float b2z = c2.z;

			MyVector* ball1VelVec = b1->GetVelocity();
			MyVector* ball2VelVec = b2->GetVelocity();

			float b1vx = ball1VelVec->p.x;
			float b1vy = ball1VelVec->p.y;
			float b1vz = ball1VelVec->p.z;

			float b2vx = ball2VelVec->p.x;
			float b2vy = ball2VelVec->p.y;
			float b2vz = ball2VelVec->p.z;

			float b1m = b1->GetMass();
			float b2m = b2->GetMass();

			MyVector normVec(b2x-b1x, b2y-b1y, b2z-b1z);
			normVec.Normalize();
			MyVector tangVec1(&GetPerpendicular(&normVec.p));
			tangVec1.Normalize();
			MyVector tangVec2 = tangVec1.Cross(&normVec);
			tangVec2.Normalize();

			float ball1ProjOnNormal		= ball1VelVec->GetProjValue( &normVec );
			float ball2ProjOnNormal		= ball2VelVec->GetProjValue( &normVec );
			float ball1ProjOnTangent1	= ball1VelVec->GetProjValue( &tangVec1 );
			float ball2ProjOnTangent1	= ball2VelVec->GetProjValue( &tangVec1 );
			float ball1ProjOnTangent2	= ball1VelVec->GetProjValue( &tangVec2 );
			float ball2ProjOnTangent2	= ball2VelVec->GetProjValue( &tangVec2 );


			float ball1FinalVelOnNormal = (ball1ProjOnNormal * (b1m - b2m) + 2. * b2m * ball2ProjOnNormal) / (b1m + b2m);
			float ball2FinalVelOnNormal = (ball2ProjOnNormal * (b2m - b1m) + 2. * b1m * ball1ProjOnNormal) / (b1m + b2m);

			float ball1FinalVelOnTang1 = ball1ProjOnTangent1;
			float ball2FinalVelOnTang1 = ball2ProjOnTangent1;

			float ball1FinalVelOnTang2 = ball1ProjOnTangent2;
			float ball2FinalVelOnTang2 = ball2ProjOnTangent2;
			

			Point ball1VelOnNormal(normVec.p.x*ball1FinalVelOnNormal, normVec.p.y*ball1FinalVelOnNormal, normVec.p.z*ball1FinalVelOnNormal);
			Point ball2VelOnNormal(normVec.p.x*ball2FinalVelOnNormal, normVec.p.y*ball2FinalVelOnNormal, normVec.p.z*ball2FinalVelOnNormal);

			Point ball1VelOnTangent1(tangVec1.p.x*ball1FinalVelOnTang1, tangVec1.p.y*ball1FinalVelOnTang1, tangVec1.p.z*ball1FinalVelOnTang1);
			Point ball2VelOnTangent1(tangVec1.p.x*ball2FinalVelOnTang1, tangVec1.p.y*ball2FinalVelOnTang1, tangVec1.p.z*ball2FinalVelOnTang1);

			Point ball1VelOnTangent2(tangVec2.p.x*ball1FinalVelOnTang2, tangVec2.p.y*ball1FinalVelOnTang2, tangVec2.p.z*ball1FinalVelOnTang2);
			Point ball2VelOnTangent2(tangVec2.p.x*ball2FinalVelOnTang2, tangVec2.p.y*ball2FinalVelOnTang2, tangVec2.p.z*ball2FinalVelOnTang2);


			float b1xVel = ball1VelOnTangent1.x + ball1VelOnTangent2.x + ball1VelOnNormal.x;
			float b1yVel = ball1VelOnTangent1.y + ball1VelOnTangent2.y + ball1VelOnNormal.y;
			float b1zVel = ball1VelOnTangent1.z + ball1VelOnTangent2.z + ball1VelOnNormal.z;

			float b2xVel = ball2VelOnTangent1.x + ball2VelOnTangent2.x + ball2VelOnNormal.x;
			float b2yVel = ball2VelOnTangent1.y + ball2VelOnTangent2.y + ball2VelOnNormal.y;
			float b2zVel = ball2VelOnTangent1.z + ball2VelOnTangent2.z + ball2VelOnNormal.z;

			b1->SetVelocity(b1xVel, b1yVel, b1zVel);
			b2->SetVelocity(b2xVel, b2yVel, b2zVel);
		}
	}
}


Point GameLoop::GetPerpendicular(Point* p)
{
	//if( p->x == 0 )
	//   return Point(0, p->z, -p->y );
	//else if( p->y == 0 )
	//	return Point(-p->z, 0, p->x);
	//else if( p->z == 0 )
	//	return Point(p->y, -p->x, 0);
	//else
	{
		//return Point(-( (p->y * p->y) + (p->z * p->z) )/p->x, p->y, p->z );

		float zAngle = 0;

		MyVector myVec(p);
		zAngle = myVec.angleInZ();

		Point tempP(p);
		tempP.SetRotationZ(-zAngle);
		Point retP(-tempP.z, 0, tempP.x);
		retP.SetRotationZ(zAngle);

		return retP;
	}
}


float GameLoop::GetFinalVelocity1(float m1, float m2, float u1, float u2)
{
	float first = (m1-m2)*u1 / (m1+m2);
	float second = (2*m2)*u2 / (m1+m2);

	return ( first + second );
}


float GameLoop::GetFinalVelocity2(float m1, float m2, float u1, float u2)
{
	float first = (m2-m1)*u2 / (m1+m2);
	float second = (2*m1)*u1  / (m1+m2);

	return ( first + second );
}


void GameLoop::CollisionWithBorder(Ball* ball)
{
	MyVector* myVec = ball->GetVelocity();

	float rightDist = _rightBorder->PerpendicularDist(&ball->GetCenter());
	float leftDist = _leftBorder->PerpendicularDist(&ball->GetCenter());

	float upDist = _upBorder->PerpendicularDist(&ball->GetCenter());
	float downDist = _downBorder->PerpendicularDist(&ball->GetCenter());

	float frontDist = _frontBorder->PerpendicularDist(&ball->GetCenter());
	float backDist = _backBorder->PerpendicularDist(&ball->GetCenter());

	if(rightDist < ball->GetR())
	{
		//ball->Move();
		//float newRightDist = _rightBorder->PerpendicularDist(&ball->GetCenter());
		//ball->UndoMove();

		//if(newRightDist < rightDist)
		{
			myVec->p.x = -myVec->p.x;
			ball->Update();
		}
	}
	if(leftDist < ball->GetR())
	{
		//ball->Move();
		//float newLeftDist = _leftBorder->PerpendicularDist(&ball->GetCenter());
		//ball->UndoMove();

		//if(newLeftDist < leftDist)
		{
			myVec->p.x = -myVec->p.x;
			ball->Update();
		}
	}

	if(upDist < ball->GetR())
	{
		//ball->Move();
		//float newUpDist = _upBorder->PerpendicularDist(&ball->GetCenter());
		//ball->UndoMove();

		//if(newUpDist < upDist)
		{
			myVec->p.y = -myVec->p.y;
			ball->Update();
		}
	}
	if(downDist < ball->GetR())
	{
		//ball->Move();
		//float newDownDist = _downBorder->PerpendicularDist(&ball->GetCenter());
		//ball->UndoMove();

		//if(newDownDist < downDist)
		{
			myVec->p.y = -myVec->p.y;
			ball->Update();
		}
	}

	if(frontDist < ball->GetR())
	{
		//ball->Move();
		//float newFrontDist = _frontBorder->PerpendicularDist(&ball->GetCenter());
		//ball->UndoMove();

		//if(newFrontDist < frontDist)
		{
			myVec->p.z = -myVec->p.z;
			ball->Update();
		}
	}
	if(backDist < ball->GetR())
	{
		//ball->Move();
		//float newBackDist = _backBorder->PerpendicularDist(&ball->GetCenter());
		//ball->UndoMove();

		//if(newBackDist < backDist)
		{
			myVec->p.z = -myVec->p.z;
			ball->Update();
		}
	}
}


void GameLoop::Draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// clear screen and depth buffer
	glMatrixMode(GL_PROJECTION);			// set projection matrix current matrix
	glLoadIdentity();						// reset projection matrix
	gluPerspective(45.0f,(GLfloat)SCREEN_WIDTH/(GLfloat)SCREEN_HEIGHT,0.1f,10000.0f);

	gluLookAt(0,0,100, 0,0,0, 0,1,0);

	glMatrixMode(GL_MODELVIEW);				// set modelview matrix
	glLoadIdentity();						// reset modelview matrix

	for(int i=0; i<_balls.size();i++)
		_balls[i]->Draw();



	glColor4ub(255,0,0,255);
	Renderer::DrawWireRect3D(&Point(-25, 25, -25), &Point(25, 25, -25), 100, &Point(-25, 25, 25), &Point(25, 25, 25), 10);
	Renderer::DrawWireRect3D(&Point(-25, -25, -25), &Point(25, -25, -25), 100, &Point(-25, -25, 25), &Point(25, -25, 25), 10);

	glColor4ub(0,255,0,255);
	Renderer::DrawWireRect3D(&Point(-25, 25, -25), &Point(25, 25, -25), 100, &Point(-25, -25, -25), &Point(25, -25, -25), 10);
	Renderer::DrawWireRect3D(&Point(-25, 25, 25), &Point(25, 25, 25), 10, &Point(-25, -25, 25), &Point(25, -25, 25), 10);

	glColor4ub(0,0,255,255);
	Renderer::DrawWireRect3D(&Point(-25, -25, 25), &Point(-25, -25, -25), 100, &Point(-25, 25, 25), &Point(-25, 25, -25), 10);
	Renderer::DrawWireRect3D(&Point( 25, -25, 25), &Point( 25, -25, -25), 100, &Point( 25, 25, 25), &Point( 25, 25, -25), 10);

	//_upBorderLine->Draw();
	//_rightBorderLine->Draw();
	//_downBorderLine->Draw();
	//_leftBorderLine->Draw();
}

void GameLoop::DrawCircle( float x, float y, int r )
{
	glPointSize(2.5f);
	glBegin(GL_POINTS);
		glVertex2f(x, y);
	glEnd();


	glBegin(GL_LINE_LOOP);
	float deltaAngle = 20;

	for( int i=0; i<360; i+=deltaAngle )
	{
		float angle = i*(22.0f/7.0f)/180.0f;
		float xx = r * cosf(angle);
		float yy = r * sinf(angle);

		glVertex2f(x+xx, y+yy );
	}
	glEnd();
}


void GameLoop::Terminate()
{
}

void GameLoop::InitGL()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//gluOrtho2D( 0, GameLoop::SCREEN_WIDTH, GameLoop::SCREEN_HEIGHT,0 );

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glEnable( GL_BLEND );
	glEnable( GL_DEPTH_TEST );
}
