#ifndef CVector3_H
#define CVector3_H

#include <math.h>

class CVector3
{
public:
	float x, y, z;

	inline CVector3();

	CVector3( float x, float y );

	CVector3( float x, float y, float z );

	CVector3( const CVector3& vec );

	~CVector3();

	void Set(float x, float y, float z);

	CVector3& operator=( const CVector3& vec );
	
	//CVector3 Addition
	CVector3& operator+=( const CVector3& vec );

	CVector3& operator-=( const CVector3& vec );

	const CVector3 operator+( const CVector3& vec );

	const CVector3 operator-( const CVector3& vec );

	//Scalar Multplication
	const CVector3 operator*( float scalar );

	const CVector3 operator/( float scalar );

	CVector3& operator*=( float scalar );

	CVector3& operator/=( float scalar );

	////Length
	float Length();	//We also Call "Length or Magnitude" as "Norm" in Vectors

	void SetLength(float len);

	////Normalize
	void Normalize();

	const float Dot( const CVector3 vec );
	const CVector3 Cross( const CVector3 a );

	const float Angle( const CVector3 vec );

	//CVector3 equivalence
	const bool operator==( const CVector3& vec );
	
	//CVector3 non-equivalence
	const bool operator!=( const CVector3& vec );
	
    // CVector3 negation
    const CVector3 operator-();

	static float projLenAonB(CVector3 a, CVector3 b);

	const float Dist( float x, float y, float z );
	const float Dist( CVector3& vec );

	const CVector3 RotateX( float x, float y, float z, float angle );
	const CVector3 RotateX( CVector3 rotatePt, float angle );

	const CVector3 RotateY( float x, float y, float z, float angle );
	const CVector3 RotateY( CVector3 rotatePt, float angle );

	const CVector3 RotateZ( float x, float y, float z, float angle );
	const CVector3 RotateZ( CVector3 rotatePt, float angle );

	CVector3 Rotate( CVector3 rotatePt, float angle );
	CVector3 Rotate( float x, float y, float angle );

	CVector3 GetReturnVec( CVector3 N );

};

#endif
