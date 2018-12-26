#ifndef MTX33_H
#define MTX33_H
#pragma once


class Mtx33
{
public:
	

public:
	float r[3][3];
	Mtx33();
	void SetRotZ(float angleInDegrees);
	void SetRotX(float angleInDegrees);
	void SetRotY(float angleInDegrees);

	static Mtx33 Multiply(Mtx33 a, Mtx33 b);
	static Mtx33 RotationMatrixFromAxisAndAngle(float Vx, float Vy, float Vz, float angleInDegrees);
};


#endif