#include "RectShape.h"

RectShape::RectShape(float x, float y)
{
	ball1 = new Ball(x, y, 40);
	ball1->SetR(20);
	ball1->SetMass(20);
	ball1->SetID(1);

	ball2 = new Ball(x+35, y, 40);
	ball2->SetVelocity(-1.0f, -1.0f);
	ball2->SetR(20);
	ball2->SetID(2);

	float cx1 = ball1->GetCX();
	float cy1 = ball1->GetCY();

	float cx2 = ball2->GetCX();
	float cy2 = ball2->GetCY();

	_initDist = sqrt((cx2-cx1)*(cx2-cx1) + (cy2-cy1)*(cy2-cy1));

}

void RectShape::ResetDist()
{
	float cx1 = ball1->GetCX();
	float cy1 = ball1->GetCY();

	float cx2 = ball2->GetCX();
	float cy2 = ball2->GetCY();

	MyVector myVec( &Point(cx2-cx1, cy2-cy1) );
	myVec.SetLength(_initDist);

	ball2->SetCenter(cx1+myVec.p.x, cy1+myVec.p.y);
}

void RectShape::Update()
{
	ball1->Update();
	ball2->Update();
}


void RectShape::Draw()
{
	ball1->Draw();
	ball2->Draw();
}