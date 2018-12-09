#include "Vector3.h"
	

CVector3::CVector3()
{
	x = y = z = 0.0f;
}

CVector3::CVector3( float x, float y )
{
	this->x = x;
	this->y = y;
		  z = 0;
}


CVector3::CVector3( float x, float y, float z )
{
	this->x = x;
	this->y = y;
	this->z = z;
}

CVector3::CVector3( const CVector3& vec )
{
	x = vec.x;
	y = vec.y;
	z = vec.z;
}

CVector3::~CVector3()
{
}

void CVector3::Set(float xx, float yy, float zz)
{
	x = xx;
	y = yy;
	z = zz;
}

CVector3& CVector3::operator=( const CVector3& vec )
{
	if( this == &vec )
		return *this;
	x = vec.x;
	y = vec.y;
	z = vec.z;
	
	return *this;
}

//CVector3 Addition
CVector3& CVector3::operator+=( const CVector3& vec )
{
	x += vec.x;
	y += vec.y;
	z += vec.z;
	
	return *this;
}

CVector3& CVector3::operator-=( const CVector3& vec )
{
	x -= vec.x;
	y -= vec.y;
	z -= vec.z;
	
	return *this;
}

const CVector3 CVector3::operator+( const CVector3& vec )
{
	return CVector3(*this) += vec;
}

const CVector3 CVector3::operator-( const CVector3& vec )
{
	return CVector3(*this) -= vec;
}

//Scalar Multplication
const CVector3 CVector3::operator*( float scalar )
{
	return CVector3( x*scalar, y*scalar, z*scalar );
}

const CVector3 CVector3::operator/( float scalar )
{
	const float recip = 1.0f/scalar; // for speed, one division
	return CVector3(x * recip, y * recip, z * recip);
}

CVector3& CVector3::operator*=( float scalar )
{
	x *= scalar;
	y *= scalar;
	z *= scalar;
	
	return *this;
}

CVector3& CVector3::operator/=( float scalar )
{
	const float recip = 1.0f/scalar; // for speed, one division
	x *= recip;
	y *= recip;
	z *= recip;
	
	return *this;
}

//Length
float CVector3::Length() //We also Call "Length or Magnitude" as "Norm" in Vectors
{
	return (float)sqrt( x*x + y*y + z*z );
}

void CVector3::SetLength(float len)
{
	Normalize();

	x *= len;
	y *= len;
	z *= len;
}

//Normalize
void CVector3::Normalize()
{
	const float length = Length();
	if( length == 0 ) 
		return;
	const float recip = 1.0f/length; // for speed, one division
	x *= recip;
	y *= recip;
	z *= recip;
}

const float CVector3::Dot( const CVector3 vec )
{	
	return (vec.x * x + vec.y * y + vec.z * z);
}

const float CVector3::Angle( CVector3 vec ) 
{
	const float dot = this->Dot( vec );
	const float len1 = this->Length();
	const float len2 = vec.Length();
	if( len1 == 0 || len2 == 0 )
		return 0;
	const float res = len1*len2;
	return acos( dot/res );
}

//CVector3 equivalence
const bool CVector3::operator==( const CVector3& vec )
{
	return (x==vec.x) && (y==vec.y) && (z==vec.z);
}

//CVector3 non-equivalence
const bool CVector3::operator!=( const CVector3& vec )
{
	return (x!=vec.x) || (y!=vec.y) || (z!=vec.z);
}

// CVector3 negation
const CVector3 CVector3::operator-()
{
	return CVector3(-x, -y, -z);
}

float CVector3::projLenAonB(CVector3 a, CVector3 b)
{
	float dotVal = a.Dot(b);

	return dotVal / b.Length();
}

CVector3 CVector3::GetReturnVec( CVector3 N )
{
	CVector3 Ri = *this;
	N.Normalize();
	float len = Ri.Dot(N);
	return Ri - N * (2*len);
}

const CVector3 CVector3::Cross( const CVector3 a )
{
	CVector3 vec;
	vec.x =  ( y*a.z - a.y*z );
	vec.y = -( x*a.z - a.x*z );
	vec.z =  ( x*a.y - a.x*y );
	return vec;
}

const float CVector3::Dist( float x, float y, float z )
{
	float X = this->x;
	float Y = this->y;
	float Z = this->z;

	return sqrtf( (X-x)*(X-x) + (Y-y)*(Y-y) + (Z-z)*(Z-z) );
}

const float CVector3::Dist( CVector3& vec )
{
	return Dist( vec.x, vec.y, vec.z );
}

CVector3 CVector3::Rotate( float x, float y, float angle )
{
	return CVector3( Rotate( CVector3( x, y, 0 ), angle ) );
}

CVector3 CVector3::Rotate( CVector3 rotatePt, float angle )
{
	float X =   rotatePt.x - x;
	float Y =   rotatePt.y - y;

	float _PI = 22.0f/7.0f;

	float newX = X * cosf(angle * _PI / 180.0f) - Y * sinf(angle * _PI / 180.0f) + x;
	float newY = X * sinf(angle * _PI / 180.0f) + Y * cosf(angle * _PI / 180.0f) + y;

	return CVector3( newX, newY, 0 );
}

const CVector3 CVector3::RotateX( CVector3 rotatePt, float angle )
{
	float yy =   rotatePt.y - y;
	float zz =   rotatePt.z - z;

	float theta = angle*(22.0f/7.0f/180.0f);

	float newY = (yy * cosf(theta) - zz * sinf(theta) )+ y;
	float newZ = (yy * sinf(theta) + zz * cosf(theta) )+ z;

	return CVector3( rotatePt.x, newY, newZ );
}

const CVector3 CVector3::RotateX( float x, float y, float z, float angle )
{
	return RotateX( CVector3( x, y, z), angle );
}

const CVector3 CVector3::RotateY( CVector3 rotatePt, float angle )
{
	float xx =   rotatePt.x - x;
	float zz =   rotatePt.z - z;

	float theta = angle*(22.0f/7.0f/180.0f);

	float newZ = (zz * cosf(theta) - xx * sinf(theta))+z;
	float newX = (zz * sinf(theta) + xx * cosf(theta))+x;

	return CVector3( newX, rotatePt.y, newZ );
}

const CVector3 CVector3::RotateY( float x, float y, float z, float angle )
{
	return RotateY( CVector3( x, y, z), angle );
}

const CVector3 CVector3::RotateZ( CVector3 rotatePt, float angle )
{
	float xx =   rotatePt.x - x;
	float yy =   rotatePt.y - y;
	
	float theta = angle*(22.0f/7.0f/180.0f);

	float newX = (xx * cosf(theta) - yy * sinf(theta)) + x;
	float newY = (xx * sinf(theta) + yy * cosf(theta)) + y;

	return CVector3( newX, newY, rotatePt.z );
}

const CVector3 CVector3::RotateZ( float x, float y, float z, float angle )
{
	return RotateZ( CVector3( x, y, z), angle );
}
