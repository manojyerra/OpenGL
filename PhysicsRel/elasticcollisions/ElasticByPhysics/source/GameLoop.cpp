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

	p1 = new Point(20,20);
	p2 = new Point(900,20);
	p3 = new Point(900,500);
	p4 = new Point(20,500);

	Ball* ball;

	for(int row = 70; row <= 470; row += 50)
	{
		for(int col = 100; col <= 700; col += 100)
		{
			ball = new Ball(col, row, 10);
			_balls.push_back(ball);
		}
	}

	//ball = new Ball(100, 400, 40);
	//ball->SetVelocity(1.0f, -1.0f);
	//ball->SetR(20);
	//ball->SetMass(20);
	//_balls.push_back(ball);

	//ball = new Ball(400, 400, 40);
	//ball->SetVelocity(-1.0f, -1.0f);
	//ball->SetR(20);
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
			Ball* b1 = ball1;
			Ball* b2 = ball2;

			float b1x = cx1;
			float b1y = cy1;
			float b2x = cx2;
			float b2y = cy2;

			MyVector* ball1VelVec = b1->GetVelocityVec();
			MyVector* ball2VelVec = b2->GetVelocityVec();

			float b1vx = ball1VelVec->p.x;
			float b1vy = ball1VelVec->p.y;
			float b2vx = ball2VelVec->p.x;
			float b2vy = ball2VelVec->p.y;

			float b1m = b1->GetMass();
			float b2m = b2->GetMass();

			MyVector normVec(&Point(b2x-b1x, b2y-b1y));
			normVec.Normalize();
			MyVector tangVec(&Point(normVec.p.y, -normVec.p.x));

			float ball1ProjOnNormal = ball1VelVec->GetProjValue( &normVec );
			float ball2ProjOnNormal = ball2VelVec->GetProjValue( &normVec );
			float ball1ProjOnTangnt = ball1VelVec->GetProjValue( &tangVec );
			float ball2ProjOnTangnt = ball2VelVec->GetProjValue( &tangVec );

			float ball1FinalVelOnNorm = (ball1ProjOnNormal * (b1m - b2m) + 2. * b2m * ball2ProjOnNormal) / (b1m + b2m);
			float ball2FinalVelOnNorm = (ball2ProjOnNormal * (b2m - b1m) + 2. * b1m * ball1ProjOnNormal) / (b1m + b2m);
			
			//float ball1FinalVelOnTang = (ball1ProjOnTangnt * (b1m - b2m) + 2. * b2m * ball2ProjOnTangnt) / (b1m + b2m);
			//float ball2FinalVelOnTang = (ball2ProjOnTangnt * (b2m - b1m) + 2. * b1m * ball1ProjOnTangnt) / (b1m + b2m);

			float ball1FinalVelOnTang = ball1ProjOnTangnt;
			float ball2FinalVelOnTang = ball2ProjOnTangnt;


			Point v_v1nPrime(normVec.p.x*ball1FinalVelOnNorm, normVec.p.y*ball1FinalVelOnNorm);
			Point v_v2nPrime(normVec.p.x*ball2FinalVelOnNorm, normVec.p.y*ball2FinalVelOnNorm);
			Point v_v1tPrime(tangVec.p.x*ball1FinalVelOnTang, tangVec.p.y*ball1FinalVelOnTang);
			Point v_v2tPrime(tangVec.p.x*ball2FinalVelOnTang, tangVec.p.y*ball2FinalVelOnTang);
			
			//update velocities.
			float nb1vx = v_v1nPrime.x + v_v1tPrime.x;
			float nb1vy = v_v1nPrime.y + v_v1tPrime.y;
			float nb2vx = v_v2nPrime.x + v_v2tPrime.x;
			float nb2vy = v_v2nPrime.y + v_v2tPrime.y; 

			b1->SetVelocity(nb1vx, nb1vy);
			b2->SetVelocity(nb2vx, nb2vy);
		}
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
	MyVector* myVec = ball->GetVelocityVec();

	float rightDist = _rightBorderLine->PerpendicularDist(ball->GetCX(), ball->GetCY());
	float upDist = _upBorderLine->PerpendicularDist(ball->GetCX(), ball->GetCY());
	float leftDist = _leftBorderLine->PerpendicularDist(ball->GetCX(), ball->GetCY());
	float downDist = _downBorderLine->PerpendicularDist(ball->GetCX(), ball->GetCY());

	if(rightDist < ball->GetR())
	{
		myVec->p.x = -myVec->p.x;
		ball->Update();
	}

	if(upDist < ball->GetR())
	{
		myVec->p.y = -myVec->p.y;
		ball->Update();
	}
		
	if(leftDist < ball->GetR())
	{
		myVec->p.x = -myVec->p.x;
		ball->Update();
	}

	if(downDist < ball->GetR())
	{
		myVec->p.y = -myVec->p.y;
		ball->Update();
	}
}


void GameLoop::Draw()
{
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
