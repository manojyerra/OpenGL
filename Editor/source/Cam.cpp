#include "Cam.h"
#include "Sui/Sui.h"
#include "Sui/SuiInput.h"
#include "MathLib/Point.h"
#include "MathLib/MatrixLib/GLMat.h"
#include "MathLib/MatrixLib/DXMat.h"
#include "Util/UtilFuncs.h"

Cam* Cam::_ref = NULL;

Cam::Cam()
{
}

Cam* Cam::GetInstance()
{
	if(_ref == NULL)
		_ref = new Cam();

	return _ref;
}

void Cam::DeleteInstance()
{
	if(_ref)
	{
		delete _ref;
		_ref = NULL;
	}
}

void Cam::Init(int windowWidth, int windowHeight)
{
	_windowWidth = windowWidth;
	_windowHeight = windowHeight;

	_angleX = 0;//10.8;
	_angleY = 0;//-391.6;

	_transX = 0.9;
	_transY = 8;
	_transZ = -13;

	_isOrtho = false;

	_origin = Point(0,0,0);

	_zNearHorHalf = 0.01f;
	_zNearVal = 0.02f;
	_zFarVal = 20000.0f;

	float ratio = (float)_windowWidth / (float)_windowHeight;

	_leftVal = -_zNearHorHalf * ratio;
	_rightVal = _zNearHorHalf * ratio;
	_bottomVal = -_zNearHorHalf;
	_topVal = _zNearHorHalf;

	_zNearW = _rightVal - _leftVal;
	_zNearH = _topVal - _bottomVal;

	memset(_modelViewMat, 0, sizeof(float)*16);
	memset(_projectionMat, 0, sizeof(float)*16);

	_modelViewMat[0] = _modelViewMat[5] = _modelViewMat[10] = _modelViewMat[15] = 1;
	_projectionMat[0] = _projectionMat[5] = _projectionMat[10] = _projectionMat[15] = 1;
}

void Cam::SetProjectionAsOrtho(bool isOrtho)
{
	_isOrtho = isOrtho;
}

bool Cam::IsOrthoProjection()
{
	return _isOrtho;
}

void Cam::SetAsOrigin(Point origin)
{
	_origin = origin;
}

void Cam::Update(float deltaTime)
{
	float dx = SuiInput::MX - SuiInput::PrevMX;
	float dy = SuiInput::MY - SuiInput::PrevMY;

	float coff = 1.0f;

	if(SuiInput::IsKeyPressed(VK_SPACE))
		coff = 20.0f;

	if(SuiInput::IsMiddleMousePressed())
	{
		if(SuiInput::IsKeyPressed(VK_SHIFT))
		{
			_transX += (dx/10.0f) / coff;
			_transY += (dy/10.0f) / coff;

			//_lookAtX += (dx/10.0f) / coff;
			//_lookAtY += (dy/10.0f) / coff;
		}
		else if(SuiInput::IsKeyPressed(VK_CONTROL) || SuiInput::IsKeyPressed(VK_SPACE))
		{
			_transZ += (-dy*0.3) / coff;
		}
		else
		{
			_angleY += dx * 180.0f / (_windowWidth*0.5);
			_angleX += dy * 180.0f / (_windowHeight*0.5);
		}
	}
	else if(SuiInput::IsScrollDown())
	{
		_transZ += -1.5 / coff;
	}
	else if(SuiInput::IsScrollUp())
	{
		_transZ += 1.5 / coff;
	}
}

void Cam::SetView(int windowWidth, int windowHeight)
{
	_windowWidth = windowWidth;
	_windowHeight = windowHeight;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	float ratio = (float)_windowWidth / (float)_windowHeight;

	if(_isOrtho)
	{
		float coff = -_transZ / _zNearVal;

		_leftVal = -_zNearHorHalf * ratio * coff;
		_rightVal = _zNearHorHalf * ratio * coff;
		_bottomVal = -_zNearHorHalf * coff;
		_topVal = _zNearHorHalf * coff;

		_zNearW = _rightVal - _leftVal;
		_zNearH = _topVal - _bottomVal;

		glOrtho(_leftVal, _rightVal, _bottomVal, _topVal, _zNearVal, _zFarVal);
	}
	else
	{
		_leftVal = -_zNearHorHalf * ratio;
		_rightVal = _zNearHorHalf * ratio;
		_bottomVal = -_zNearHorHalf;
		_topVal = _zNearHorHalf;

		_zNearW = _rightVal - _leftVal;
		_zNearH = _topVal - _bottomVal;

		glFrustum(_leftVal, _rightVal, _bottomVal, _topVal, _zNearVal, _zFarVal);
	}

	glGetFloatv(GL_PROJECTION_MATRIX, _projectionMat);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(_transX,-_transY,_transZ);
	glRotatef(_angleX,1,0,0);
	glRotatef(_angleY,0,1,0);
	glTranslatef(-_origin.x, -_origin.y, -_origin.z);

	//From Blender export as +Y Forward, +Z Up
	//and Rotate X and Z

	//glRotatef(-90, 1,0,0);
	//glRotatef(-90, 0,0,1);
	glGetFloatv(GL_MODELVIEW_MATRIX, _modelViewMat);

	float params[9];
	GetGluLookAtParameters(_modelViewMat, params);

	_camPos.setX(params[0]);
	_camPos.setY(params[1]);
	_camPos.setZ(params[2]);

	_camLookAt.setX(params[3]);
	_camLookAt.setY(params[4]);
	_camLookAt.setZ(params[5]);

	_upVector.setX(params[6]);
	_upVector.setY(params[7]);
	_upVector.setZ(params[8]);

	//glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();
	//gluLookAt
	//		(	_camPos.getX(),		_camPos.getY(),		_camPos.getZ(),
	//			_camLookAt.getX(),	_camLookAt.getY(),	_camLookAt.getZ(),
	//			_upVector.getX(),	_upVector.getY(),	_upVector.getZ()
	//		);

	//float fromGluLookAt[16];
	//glGetFloatv(GL_MODELVIEW_MATRIX, fromGluLookAt);
}

void Cam::SetViewType(int viewType)
{
	if(viewType == FRONT_VIEW)
	{
		_angleX = 0.0f;
		_angleY = 0.0f;			//no change in y
	}
	else if(viewType == BACK_VIEW)
	{
		_angleX = 0.0f;
		_angleY = 180.0f;			//need to add 180 to y otherwise just make it y value as 180.
	}
	else if(viewType == TOP_VIEW)
	{
		_angleX = 90.0f;
		//_angleY = 0.0f;			//no change in y
	}
	else if(viewType == BOTTOM_VIEW)
	{
		_angleX = -90.0f;
		//_angleY = 0.0f;			//no change in y
	}
	else if(viewType == RIGHT_VIEW)
	{
		_angleX = 0.0f;
		_angleY = -90.0f;
	}
	else if(viewType == LEFT_VIEW)
	{
		_angleX = 0.0f;
		_angleY = 90.0f;
	}

	_transX = 0;
	_transY = 0;
}

float* Cam::GetModelViewMatrix()
{
	return _modelViewMat;
}

float* Cam::GetProjectionMatrix()
{
	return _projectionMat;
}

void Cam::GetPosOnScreen(float* ver, int numVertex, float* onScreen)
{
	float* a = _modelViewMat;

	int xyCount = 0;
	for(int i=0; i<numVertex*3; i+=3)
	{
		float x = ver[i+0];
		float y = ver[i+1];
		float z = ver[i+2];

		float xWPos = a[0]*x + a[4]*y + a[8]*z + a[12];
		float yWPos = a[1]*x + a[5]*y + a[9]*z + a[13];
		float zWPos = a[2]*x + a[6]*y + a[10]*z + a[14];

		//if(zWPos > -1)
		//	zWPos = -1;

		float xOnZNear = -_zNearVal * xWPos / zWPos;
		float yOnZNear = -_zNearVal * yWPos / zWPos;

		if(_isOrtho)
		{
			xOnZNear = xWPos;
			yOnZNear = yWPos;
		}

		onScreen[xyCount+0] = (( xOnZNear - _leftVal ) / _zNearW) * _windowWidth;
		onScreen[xyCount+1] = _windowHeight - ((( yOnZNear - _bottomVal ) / _zNearH) * _windowHeight);

		xyCount += 2;
	}
}

void Cam::GetPosOnScreen(float* ver, int numVertex, float* objectOrientationMat, float* onScreen)
{
	float a[16];
	MultMat(_modelViewMat, objectOrientationMat, a);

	int xyCount = 0;
	for(int i=0; i<numVertex*3; i+=3)
	{
		float x = ver[i+0];
		float y = ver[i+1];
		float z = ver[i+2];

		float xWPos = a[0]*x + a[4]*y + a[8]*z + a[12];
		float yWPos = a[1]*x + a[5]*y + a[9]*z + a[13];
		float zWPos = a[2]*x + a[6]*y + a[10]*z + a[14];

		//if(zWPos > -1)
		//	zWPos = -1;

		float xOnZNear = -_zNearVal * xWPos / zWPos;
		float yOnZNear = -_zNearVal * yWPos / zWPos;

		if(_isOrtho)
		{
			xOnZNear = xWPos;
			yOnZNear = yWPos;
		}

		onScreen[xyCount+0] = (( xOnZNear - _leftVal ) / _zNearW) * _windowWidth;
		onScreen[xyCount+1] = _windowHeight - ((( yOnZNear - _bottomVal ) / _zNearH) * _windowHeight);

		xyCount += 2;
	}
}


btVector3 Cam::getCameraPosition()
{
	return _camPos;
}

btVector3 Cam::getCameraTargetPosition()
{
	return _camLookAt;
}

btVector3 Cam::getUpVector()
{
	return _upVector;
}

btVector3 Cam::getForwardVector()
{
	return _forwardVector;
}

btVector3 Cam::getSideVector()
{
	return _sideVector;
}


Cam::~Cam()
{
}

void Cam::MultMat(float* aa, float* bb, float* result)
{
	float a[16];
	float b[16];

	memcpy(a, aa, 16*sizeof(float));
	memcpy(b, bb, 16*sizeof(float));

	result[0] = a[0]*b[0] + a[4]*b[1] + a[8]*b[2] + a[12]*b[3];
	result[1] = a[1]*b[0] + a[5]*b[1] + a[9]*b[2] + a[13]*b[3];
	result[2] = a[2]*b[0] + a[6]*b[1] + a[10]*b[2] + a[14]*b[3];
	result[3] = a[3]*b[0] + a[7]*b[1] + a[11]*b[2] + a[15]*b[3];

	result[4] = a[0]*b[4] + a[4]*b[5] + a[8]*b[6] + a[12]*b[7];
	result[5] = a[1]*b[4] + a[5]*b[5] + a[9]*b[6] + a[13]*b[7];
	result[6] = a[2]*b[4] + a[6]*b[5] + a[10]*b[6] + a[14]*b[7];
	result[7] = a[3]*b[4] + a[7]*b[5] + a[11]*b[6] + a[15]*b[7];
	
	result[8] = a[0]*b[8] + a[4]*b[9] + a[8]*b[10] + a[12]*b[11];
	result[9] = a[1]*b[8] + a[5]*b[9] + a[9]*b[10] + a[13]*b[11];
	result[10] = a[2]*b[8] + a[6]*b[9] + a[10]*b[10] + a[14]*b[11];
	result[11] = a[3]*b[8] + a[7]*b[9] + a[11]*b[10] + a[15]*b[11];
	
	result[12] = a[0]*b[12] + a[4]*b[13] + a[8]*b[14] + a[12]*b[15];
	result[13] = a[1]*b[12] + a[5]*b[13] + a[9]*b[14] + a[13]*b[15];
	result[14] = a[2]*b[12] + a[6]*b[13] + a[10]*b[14] + a[14]*b[15];
	result[15] = a[3]*b[12] + a[7]*b[13] + a[11]*b[14] + a[15]*b[15];
}

void Cam::LookAt(btVector3& camPos, btVector3& camLookAt, btVector3& upVector, float* m)
{
    btVector3 forwardVec = camLookAt - camPos;
	forwardVec.normalize();

    btVector3 sideVec = forwardVec.cross(upVector);
	sideVec.normalize();
	
	upVector = sideVec.cross(forwardVec);

	memset(m, 0, 16*sizeof(GLfloat));
	m[0] = m[5] = m[10] = m[15] = 1.0f;

    m[0] = sideVec[0];
    m[4] = sideVec[1];
    m[8] = sideVec[2];

    m[1] = upVector[0];
    m[5] = upVector[1];
    m[9] = upVector[2];

    m[2] = -forwardVec[0];
    m[6] = -forwardVec[1];
    m[10] = -forwardVec[2];

	float transMat[16];
	memset(transMat, 0, 16*sizeof(float));
	transMat[0] = transMat[5] = transMat[10] = transMat[15] = 1.0f;
	transMat[12] = -camPos.getX();
	transMat[13] = -camPos.getY();
	transMat[14] = -camPos.getZ();

	MultMat(m, transMat, m);
}

void Cam::__gluMakeIdentityd(float m[16])
{
    m[0+4*0] = 1; m[0+4*1] = 0; m[0+4*2] = 0; m[0+4*3] = 0;
    m[1+4*0] = 0; m[1+4*1] = 1; m[1+4*2] = 0; m[1+4*3] = 0;
    m[2+4*0] = 0; m[2+4*1] = 0; m[2+4*2] = 1; m[2+4*3] = 0;
    m[3+4*0] = 0; m[3+4*1] = 0; m[3+4*2] = 0; m[3+4*3] = 1;
}

int Cam::__gluInvertMatrixd(const float src[16], float inverse[16])
{
    int i, j, k, swap;
    float t;
    GLfloat temp[4][4];

    for (i=0; i<4; i++)
		for (j=0; j<4; j++)
			temp[i][j] = src[i*4+j];

	for(int i=0;i<16;i++)
		inverse[i] = 0;
	inverse[0] = inverse[5] = inverse[10] = inverse[15] = 1.0f;

    for(i=0; i<4; i++)
	{
		swap = i;
		for (j = i + 1; j < 4; j++)
			if (fabs(temp[j][i]) > fabs(temp[i][i]))
				swap = j;

		if (swap != i) {
			//Swap rows.
			for (k = 0; k < 4; k++) {
				t = temp[i][k];
				temp[i][k] = temp[swap][k];
				temp[swap][k] = t;
				
				t = inverse[i*4+k];
				inverse[i*4+k] = inverse[swap*4+k];
				inverse[swap*4+k] = t;
			}
		}
		
		if (temp[i][i] == 0)
			return 0;

		t = temp[i][i];
		for (k = 0; k < 4; k++) {
			temp[i][k] /= t;
			inverse[i*4+k] /= t;
		}

		for (j = 0; j < 4; j++) {
			if (j != i) {
				t = temp[j][i];
				for (k = 0; k < 4; k++) {
					temp[j][k] -= temp[i][k]*t;
					inverse[j*4+k] -= inverse[i*4+k]*t;
				}
			}
		}
    }

    return 1;
}


void Cam::GetGluLookAtParameters(float* m, float* gluLookAtParams)
{
	Point sideVector(m[0], m[4], m[8]);
	Point upVector(m[1], m[5], m[9]);
	Point forwardVector(-m[2], -m[6], -m[10]);

	sideVector.Normalize();
	upVector.Normalize();
	forwardVector.Normalize();
	
	float rotMat[16];
	memcpy(rotMat, m, 16*sizeof(float));
	rotMat[12] = rotMat[13] = rotMat[14] = rotMat[3] = rotMat[7] = rotMat[11] = 0.0f;
	rotMat[15] = 1.0f;
	float rotInvert[16];
	__gluInvertMatrixd(rotMat, rotInvert);

	float transMat[16];
	memset(transMat, 0, 16*sizeof(float));
	transMat[0] = transMat[5] = transMat[10] = transMat[15] = 1.0f;
	MultMat(rotInvert, m, transMat);

	gluLookAtParams[0] = -transMat[12];
	gluLookAtParams[1] = -transMat[13];
	gluLookAtParams[2] = -transMat[14];

	gluLookAtParams[3] = -transMat[12] + forwardVector.x;
	gluLookAtParams[4] = -transMat[13] + forwardVector.y;
	gluLookAtParams[5] = -transMat[14] + forwardVector.z;
	
	gluLookAtParams[6] = upVector.x;
	gluLookAtParams[7] = upVector.y;
	gluLookAtParams[8] = upVector.z;
}