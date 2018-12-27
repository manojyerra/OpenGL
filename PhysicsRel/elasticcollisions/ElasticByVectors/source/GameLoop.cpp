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

GameLoop::GameLoop(){}
GameLoop::~GameLoop(){}

void GameLoop::Init()
{
	InitGL();
	UIFont::GetInstance()->Init();

	//Point p1(400,380);
	//Point p2(250,450);

	//lineVector = new MyVector(&p1, &p2);

	//Point rayP1(250, 250);
	//Point rayP2(250, 300);

	//rayVector = new MyVector(&rayP1, &rayP2);

	//MyVector tempNormVec = rayVector->GetNormalVector( &p1, &p2);
	//normalVector = new MyVector(&tempNormVec.Tail, &tempNormVec.Head);

	//MyVector tempRefVec = rayVector->GetReflectionVector( &p1, &p2);
	//refVector = new MyVector(&tempRefVec.Tail, &tempRefVec.Head);

	p1 = new Point(50,50);
	p2 = new Point(900,0);
	p3 = new Point(1024,500);
	p4 = new Point(20,500);

	Ball* ball;

	for(int row = 70; row <= 470; row += 50)
	{
		for(int col = 100; col <= 700; col += 100)
		{
			ball = new Ball(col, row, 10);
			ball->SetDirection(&Point(0,1,0));
			_balls.push_back(ball);
		}
	}

	//ball = new Ball(100, 100, 10);
	//ball->SetDirection(&Point(1,0.01,0));
	//_balls.push_back(ball);

	//ball = new Ball(100, 130, 10);
	//ball->SetDirection(&Point(1,-0.01,0));
	//_balls.push_back(ball);


	//ball = new Ball(100, 130, 10);
	//ball->SetDirection(&Point(1,0,0));
	//MyVector* myVec = ball->GetDirectionVec();
	//myVec->Reflect(&Point(0,0),  &Point(1,0));

	//_balls.push_back(ball);


	_upBorderLine = new Line2D( p1, p2 );
	_rightBorderLine = new Line2D( p2, p3 );
	_downBorderLine = new Line2D( p3, p4 );
	_leftBorderLine = new Line2D( p4, p1 );
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
	float cx1 = ball1->GetCX();
	float cy1 = ball1->GetCY();
	float cx2 = ball2->GetCX();
	float cy2 = ball2->GetCY();

	float r1 = ball1->GetR();
	float r2 = ball2->GetR();

	float dist = sqrt( (cx2-cx1)*(cx2-cx1) + (cy2-cy1)*(cy2-cy1) );

	if(dist <= r1+r2)
	{
		ball1->Move();
		ball2->Move();

		cx1 = ball1->GetCX();
		cy1 = ball1->GetCY();
		cx2 = ball2->GetCX();
		cy2 = ball2->GetCY();

		float newDist = sqrt( (cx2-cx1)*(cx2-cx1) + (cy2-cy1)*(cy2-cy1) );

		ball1->UndoMove();
		ball2->UndoMove();

		if(newDist < dist)
		{
			float A = cy2-cy1;
			float B = cx1-cx2;

			ball1->GetDirectionVec()->Reflect(&Point(0,0),  &Point(A,B));
			ball2->GetDirectionVec()->Reflect(&Point(0,0),  &Point(A,B));

			ball1->Update();
			ball2->Update();
		}

		//// to check, if it is still colliding...
		//cx1 = ball1->GetCX();
		//cy1 = ball1->GetCY();
		//cx2 = ball2->GetCX();
		//cy2 = ball2->GetCY();

		//r1 = ball1->GetR();
		//r2 = ball2->GetR();

		//float dist = sqrt( (cx2-cx1)*(cx2-cx1) + (cy2-cy1)*(cy2-cy1) );

		//if(dist <= r1+r2)
		//{
		//	ball1->Update();
		//	ball2->Update();
		//}

		// up to here...


		//// x - axis
		//float vx1 = GetFinalVelocity1(ball1->GetMass(), ball2->GetMass(), ball1->GetXVelocity(), ball2->GetXVelocity());
		//float vx2 = GetFinalVelocity2(ball1->GetMass(), ball2->GetMass(), ball1->GetXVelocity(), ball2->GetXVelocity());

		//// y - axis
		//float vy1 = GetFinalVelocity1(ball1->GetMass(), ball2->GetMass(), ball1->GetYVelocity(), ball2->GetYVelocity());
		//float vy2 = GetFinalVelocity2(ball1->GetMass(), ball2->GetMass(), ball1->GetYVelocity(), ball2->GetYVelocity());

		//ball1->SetVelocity( vx1, vy1 );
		//ball2->SetVelocity( vx2, vy2 );
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
	MyVector* myVec = ball->GetDirectionVec();

	float rightDist = _rightBorderLine->PerpendicularDist(ball->GetCX(), ball->GetCY());
	float upDist = _upBorderLine->PerpendicularDist(ball->GetCX(), ball->GetCY());
	float leftDist = _leftBorderLine->PerpendicularDist(ball->GetCX(), ball->GetCY());
	float downDist = _downBorderLine->PerpendicularDist(ball->GetCX(), ball->GetCY());

	if(rightDist < ball->GetR())
	{
		myVec->Reflect(&_rightBorderLine->P1, &_rightBorderLine->P2);
		ball->Update();
	}

	if(upDist < ball->GetR())
	{
		myVec->Reflect(&_upBorderLine->P1, &_upBorderLine->P2);
		ball->Update();
	}
		
	if(leftDist < ball->GetR())
	{
		myVec->Reflect(&_leftBorderLine->P1, &_leftBorderLine->P2);
		ball->Update();
	}

	if(downDist < ball->GetR())
	{
		myVec->Reflect(&_downBorderLine->P1, &_downBorderLine->P2);
		ball->Update();
	}
}


void GameLoop::Draw()
{
	//float r1 = 50;
	//float r2 = 90;

	//DrawCircle( 60, 200, r1 );
	//DrawCircle( 200, 200, r2 );

	//CVector3 v(200-60, 200-200);
	//CVector3 v1(60, 200);

	//v.Normalize();
	//v*=r1;
	//v+=v1;

	//glPointSize(4.5f);
	//glColor4ub( 255, 0,0, 255);
	//glBegin(GL_POINTS);
	//	glVertex2f(v.x, v.y);
	//glEnd();
	//glColor4ub( 255, 255,255, 255);

	//CVector3 dir = v1 - v;
	//dir.Normalize();

	//CVector3 roi = CVector3(60, 200-r1)- v;
	//float len = dir.Dot(roi);
	//dir *= len;
	//v += dir;

	//dir = v-CVector3(60, 200-r1);
	//dir.Normalize();
	//len = dir.Length();

	//CVector3 pt = CVector3(60, 200-r1);
	//CVector3 ror = pt+(dir*len);

	//glBegin(GL_POINTS);
	//glVertex2f(ror.x, ror.y);
	//glEnd();

	//lineVector->Draw();
	//rayVector->Draw();
	//refVector->Draw();
	//normalVector->Draw();

	for(int i=0; i<_balls.size();i++)
		_balls[i]->Draw();

	_upBorderLine->Draw();
	_rightBorderLine->Draw();
	_downBorderLine->Draw();
	_leftBorderLine->Draw();
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
	gluOrtho2D( 0, GameLoop::SCREEN_WIDTH, GameLoop::SCREEN_HEIGHT,0 );

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glEnable( GL_BLEND );
}
