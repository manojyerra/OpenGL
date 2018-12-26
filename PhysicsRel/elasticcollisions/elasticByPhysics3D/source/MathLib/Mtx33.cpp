#include "Mtx33.h"
#include <math.h>

Mtx33::Mtx33()
{
	r[0][0] = 1;	r[0][1] = 0;	r[0][2] = 0;
	r[1][0] = 0;	r[1][1] = 1;	r[1][2] = 0;
	r[2][0] = 0;	r[2][1] = 0;	r[2][2] = 1;
}


void Mtx33::SetRotZ(float angleInDegrees)
{
	//if(angleInDegrees > 89.9 && angleInDegrees <= 90)
	//	angleInDegrees = 89.99;

	float rad = angleInDegrees * (22.0f/7.0f) / 180.0f;

	r[0][0] = cosf(rad);			r[0][1] = -sinf(rad);		r[0][2] = 0;
	r[1][0] = sinf(rad);			r[1][1] = cosf(rad);		r[1][2] = 0;
	r[2][0] = 0;				r[2][1] = 0;				r[2][2] = 1;
}


void Mtx33::SetRotX(float angleInDegrees)
{
	//if(angleInDegrees > 89.9 && angleInDegrees <= 90)
	//	angleInDegrees = 89.99;

	float rad = angleInDegrees * (22.0f/7.0f) / 180.0f;

	r[0][0] = 1;				r[0][1] = 0;				r[0][2] = 0;
	r[1][0] = 0;				r[1][1] = cosf(rad);			r[1][2] = -sinf(rad);
	r[2][0] = 0;				r[2][1] = sinf(rad);			r[2][2] = cosf(rad);
}


void Mtx33::SetRotY(float angleInDegrees)
{
	//if(angleInDegrees > 89.9 && angleInDegrees <= 90)
	//	angleInDegrees = 89.99;

	float rad = angleInDegrees * (22.0f/7.0f) / 180.0f;

	r[0][0] = cosf(rad);		r[0][1] = 0;			r[0][2] = sinf(rad);
	r[1][0] = 0;				r[1][1] = 1;			r[1][2] = 0;
	r[2][0] = -sinf(rad);		r[2][1] = 0;			r[2][2] = cosf(rad);
}


Mtx33 Mtx33::Multiply(Mtx33 a, Mtx33 b)
{
	Mtx33 res;

	res.r[0][0] = a.r[0][0]*b.r[0][0] + a.r[0][1]*b.r[1][0] + a.r[0][2]*b.r[2][0];
	res.r[0][1] = a.r[0][0]*b.r[0][1] + a.r[0][1]*b.r[1][1] + a.r[0][2]*b.r[2][1];
	res.r[0][2] = a.r[0][0]*b.r[0][2] + a.r[0][1]*b.r[1][2] + a.r[0][2]*b.r[2][2];

	res.r[1][0] = a.r[1][0]*b.r[0][0] + a.r[1][1]*b.r[1][0] + a.r[1][2]*b.r[2][0];
	res.r[1][1] = a.r[1][0]*b.r[0][1] + a.r[1][1]*b.r[1][1] + a.r[1][2]*b.r[2][1];
	res.r[1][2] = a.r[1][0]*b.r[0][2] + a.r[1][1]*b.r[1][2] + a.r[1][2]*b.r[2][2];

	res.r[2][0] = a.r[2][0]*b.r[0][0] + a.r[2][1]*b.r[1][0] + a.r[2][2]*b.r[2][0];
	res.r[2][1] = a.r[2][0]*b.r[0][1] + a.r[2][1]*b.r[1][1] + a.r[2][2]*b.r[2][1];
	res.r[2][2] = a.r[2][0]*b.r[0][2] + a.r[2][1]*b.r[1][2] + a.r[2][2]*b.r[2][2];

	return res;
}


Mtx33 Mtx33::RotationMatrixFromAxisAndAngle(float Vx, float Vy, float Vz, float angleInDegrees)
{
	//if(angleInDegrees > 89.9 && angleInDegrees <= 90)
	//	angleInDegrees = 89.99;

	Mtx33 res;

	float theta = angleInDegrees * 22.0f / (7.0f * 180.0f);

	res.r[0][0] = cos(theta) + Vx*Vx*(1-cos(theta));
	res.r[0][1] = Vx*Vy*(1-cos(theta)) - Vz*sin(theta);
	res.r[0][2] = Vx*Vz*(1-cos(theta)) + Vy*sin(theta);

	res.r[1][0] = Vy*Vx*(1-cos(theta)) + Vz*sin(theta);
	res.r[1][1] = cos(theta) + Vy*Vy*(1-cos(theta));
	res.r[1][2] = Vy*Vz*(1-cos(theta)) - Vx*sin(theta);

	res.r[2][0] = Vz*Vx*(1-cos(theta)) + Vy*sin(theta);
	res.r[2][1] = Vz*Vy*(1-cos(theta)) - Vx*sin(theta);
	res.r[2][2] = cos(theta) + Vz*Vz*(1-cos(theta));

	return res;
}
