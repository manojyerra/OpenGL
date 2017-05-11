#include "FLModel.h"
#include "CFileReader.h"
#include "ImageBuffer.h"
#include "Util/GLUtil.h"
#include "Cam.h"

FLModel::FLModel(string folderPath)
{
	Reset(folderPath, NULL);
}

FLModel::FLModel(string folderPath, float xPos, float yPos, float zPos)
{
	GLMat mat;
	mat.glTranslatef(xPos, yPos, zPos);
	Reset(folderPath, mat.m);
}

FLModel::FLModel(string folderPath, CVector3 pos)
{
	GLMat mat;
	mat.glTranslatef(pos.x, pos.y, pos.z);
	Reset(folderPath, mat.m);
}

FLModel::FLModel(string folderPath, CVector3 pos, CVector3 rot)
{
	GLMat mat;
	mat.glTranslatef(pos.x, pos.y, pos.z);

	mat.glRotatef(rot.z, 0, 0, 1);
	mat.glRotatef(rot.y, 0, 1, 0);
	mat.glRotatef(rot.x, 1, 0, 0);

	Reset(folderPath, mat.m);
}

FLModel::FLModel(string folderPath, CVector3 pos, CVector3 rot, string rotOrder)
{
	GLMat mat;
	mat.glTranslatef(pos.x, pos.y, pos.z);

	char axis = rotOrder[2];
	
	if(axis == 'x')			mat.glRotatef(rot.z, 1, 0, 0);
	else if(axis == 'y')	mat.glRotatef(rot.z, 0, 1, 0);
	else if(axis == 'z')	mat.glRotatef(rot.z, 0, 0, 1);

	axis = rotOrder[1];
	
	if(axis == 'x')			mat.glRotatef(rot.y, 1, 0, 0);
	else if(axis == 'y')	mat.glRotatef(rot.y, 0, 1, 0);
	else if(axis == 'z')	mat.glRotatef(rot.y, 0, 0, 1);

	axis = rotOrder[0];
	
	if(axis == 'x')			mat.glRotatef(rot.x, 1, 0, 0);
	else if(axis == 'y')	mat.glRotatef(rot.x, 0, 1, 0);
	else if(axis == 'z')	mat.glRotatef(rot.x, 0, 0, 1);

	Reset(folderPath, mat.m);
}

FLModel::FLModel(string folderPath, float* mat)
{
	Reset(folderPath, mat);
}

void FLModel::Reset(string folderPath, float* mat)
{
	_folderPath = folderPath;
	_verticesPointer = NULL;
	_uvsPointer = NULL;
	_normalsPointer = NULL;
	_indicesPointer = NULL;
	_textureID = 0;
	_indicesType = 0;
	_numIndices = 0;
	_numVertex = 0;
	_isTextureEnabled = true;
	_wireFrameLinesEnabled = false;
	_wireFramePointsEnabled = false;
	_boundingBoxEnabled = false;
	_bounding2DRectEnabled = false;
	_lightingEnabled = true;
	_showBoundingShapes = false;
	_showModel = true;
	_drawLocalAxis = false;
	_isMarked = false;

	GLfloat ka[4] = { 0.6f, 0.6f, 0.6f, 1.0f};
	GLfloat kd[4] = { 0.6f, 0.6f, 0.6f, 1.0f};
	GLfloat ks[4] = { 0.9f, 0.9f, 0.9f, 1.0f};

	for(int i=0; i<4; i++)
	{
		_ka[i] = ka[i];
		_kd[i] = kd[i];
		_ks[i] = ks[i];
	}

	_shininess= 1.0f;

	_border = new FLModelBorder();

	_flmReaderWriter = new FLModelReaderWriter();
	_flmReaderWriter->Load(_folderPath);

	_verticesPointer	= _flmReaderWriter->GetVerticesPointer();
	_uvsPointer			= _flmReaderWriter->GetTexCoordsPointer();
	_normalsPointer		= _flmReaderWriter->GetNormalsPointer();
	_indicesPointer		= _flmReaderWriter->GetIndicesPointer();
	_indicesType		= _flmReaderWriter->GetIndicesType();

	_numVertex			= _flmReaderWriter->GetNumVertex();
	_numIndices			= _flmReaderWriter->GetNumIndices();

	_textureID			= _flmReaderWriter->GetTextureID();

	_aabb				= _flmReaderWriter->GetAABB();

	if(mat == NULL)
		_mat = _flmReaderWriter->GetMat();
	else
		_mat.Copy(mat);
}

float* FLModel::GetVerticesPointer()
{
	return (float*)_verticesPointer;
}

unsigned int FLModel::GetNumVertices()
{
	return _numVertex;
}

GLMat FLModel::GetMat()
{
	return _mat;
}

void FLModel::SetPos(float x, float y, float z)				{ _mat.SetPos(x, y, z);						}
void FLModel::SetPos(CVector3 pos)							{ _mat.SetPos(pos);							}
CVector3 FLModel::GetPos()									{ return _mat.GetPos();						}
void FLModel::AddTransInWorld(float x, float y, float z)	{ _mat.AddTransInWorld(x, y, z);			}
void FLModel::AddTransInLocal(char axis, float move)		{ _mat.AddTransInLocal( axis, move );		}
void FLModel::AddRotateInWorld(char axis, float angle)		{ _mat.AddRotateInWorld(axis, angle);		}
void FLModel::AddRotateInLocal(char axis, float angle)		{ _mat.AddRotateInLocal(axis, angle);		}
CVector3 FLModel::GetRotation()								{ return _mat.GetRotation();				}
void FLModel::SetRotation(CVector3 rot)						{ _mat.SetRotation(rot);					}
void FLModel::AddScale(CVector3 scale)						{ _mat.glScalef(scale.x, scale.y, scale.z);	}
void FLModel::AddUniformScale(float scale)					{ _mat.glScalef(scale, scale, scale);		}


void FLModel::SetTextureEnabled(bool enable)				{	_isTextureEnabled = enable;			}
bool FLModel::IsTextureEnabled()							{	return _isTextureEnabled;			}
void FLModel::SetWireFrameLinesEnabled(bool enable)			{	_wireFrameLinesEnabled = enable;	}
bool FLModel::IsWireFrameLinesEnabled()						{	return _wireFrameLinesEnabled;		}
void FLModel::SetWireFramePointsEnabled(bool enable)		{	_wireFramePointsEnabled = enable;	}
bool FLModel::IsWireFramePointsEnabled()					{	return _wireFramePointsEnabled;		}
void FLModel::SetBoundingBoxEnabled(bool enable)			{	_boundingBoxEnabled = enable;		}
bool FLModel::IsBoundingBoxEnabled()						{	return _boundingBoxEnabled;			}
void FLModel::SetBounding2DRectEnabled(bool enable)			{	_bounding2DRectEnabled = enable;	}
bool FLModel::IsBounding2DRectEnabled()						{	return _bounding2DRectEnabled;		}
void FLModel::SetLightingEnabled(bool enable)				{	_lightingEnabled = enable;			}
bool FLModel::IsLightingEnabled()							{	return _lightingEnabled;			}
void FLModel::ShowBoundingShapes(bool show)					{	_showBoundingShapes = show;			}
bool FLModel::IsShowingBoundingShapes()						{	return _showBoundingShapes;			}
void FLModel::ShowModel(bool show)							{	_showModel = show;					}
bool FLModel::IsShowingModel()								{	return _showModel;					}
void FLModel::ShowLocalAxis(bool show)						{	_drawLocalAxis = show;				}
bool FLModel::IsShowingLocalAxis()							{	return _drawLocalAxis;				}
void FLModel::SetMarked(bool mark)							{	_isMarked = mark;					}
bool FLModel::IsMarked()									{	return _isMarked;					}


void FLModel::SetShininess(float val)						{	_shininess = val;					}
float FLModel::GetShininess()								{	return _shininess;					}

void FLModel::SetMeterial(int lightParam, float r, float g, float b, float a)
{
	if(lightParam == GL_AMBIENT)		{	_ka[0] =r; _ka[1] =g; _ka[2] =b; _ka[3] =a;	}
	else if(lightParam == GL_DIFFUSE)	{	_kd[0] =r; _kd[1] =g; _kd[2] =b; _kd[3] =a;	}
	else if(lightParam == GL_SPECULAR)	{	_ks[0] =r; _ks[1] =g; _ks[2] =b; _ks[3] =a;	}
}

unsigned int FLModel::GetMeterial(int lightParam)
{
	if(lightParam == GL_AMBIENT)		return GLUtil::GetUInt(_ka[0], _ka[1], _ka[2], _ka[3]);
	else if(lightParam == GL_DIFFUSE)	return GLUtil::GetUInt(_kd[0], _kd[1], _kd[2], _kd[3]);
	else if(lightParam == GL_SPECULAR)	return GLUtil::GetUInt(_ks[0], _ks[1], _ks[2], _ks[3]);

	return 0;
}

void FLModel::AddBoundingShape(Shape* shape)
{
	_boundingShapes.push_back(shape);
}

Shape* FLModel::AddBestBoudingShapeByVerticesOnRect(Rect* rect)
{
	return AddBestBoudingShapeByVerticesOnRect(rect->x, rect->y, rect->w, rect->h);
}

Shape* FLModel::AddBoudingShapeByVerticesOnRect(Rect* rect, int boundingShapeID)
{
	return AddBoudingShapeByVerticesOnRect(rect->x, rect->y, rect->w, rect->h, boundingShapeID);
}

Shape* FLModel::AddBestBoudingShapeByVerticesOnRect(float x, float y, float w, float h)
{
	vector<float> verVec = GLUtil::GetVerticesOnRect((float*)_verticesPointer, _numVertex, _mat.m, x, y, w, h);
	
	Shape* bShape = NULL;

	if(verVec.size() > 0)
	{
		bShape = Shape::GetBestFitBoundingShape(&verVec[0], verVec.size());

		_boundingShapes.push_back(bShape);
	}

	return bShape;
}

Shape* FLModel::AddBoudingShapeByVerticesOnRect(float x, float y, float w, float h, int boundingShapeID)
{
	vector<float> verVec = GLUtil::GetVerticesOnRect((float*)_verticesPointer, _numVertex, _mat.m, x, y, w, h);
	
	Shape* bShape = NULL;

	if(verVec.size() > 0)
	{
		bShape = Shape::GetBoundingShape(&verVec[0], verVec.size(), boundingShapeID);

		if(bShape)
			_boundingShapes.push_back(bShape);
	}

	return bShape;
}

void FLModel::Draw()
{
	glPushMatrix();
	glMultMatrixf(_mat.m);

	GLboolean isLightOn = GLUtil::GLEnable(GL_LIGHTING, _lightingEnabled);

	bool enableNormals = glIsEnabled(GL_LIGHTING) && _normalsPointer;
	bool enableTexture = _textureID > 0 && _uvsPointer && _isTextureEnabled;

	if(enableTexture)
	{
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, _textureID);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer( 2, GL_FLOAT, 0, _uvsPointer );
	}

	if(enableNormals)
	{
		glEnableClientState(GL_NORMAL_ARRAY);
		glNormalPointer(GL_FLOAT, 0, _normalsPointer );

		glMaterialfv(GL_FRONT, GL_AMBIENT, _ka);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, _kd);
		glMaterialfv(GL_FRONT, GL_SPECULAR, _ks);
		glMaterialf(GL_FRONT, GL_SHININESS, _shininess);
	}

	if(_verticesPointer)
	{
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer( 3, GL_FLOAT, 0, _verticesPointer );
	}

	if(_showModel)
		glDrawElements(GL_TRIANGLES, _numIndices, _indicesType, _indicesPointer);

	if(enableNormals)
	{
		glDisableClientState(GL_NORMAL_ARRAY);
	}

	if(enableTexture)
	{
		glDisable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	}

	GLUtil::GLEnable(GL_LIGHTING, isLightOn);

	if(_wireFrameLinesEnabled || _wireFramePointsEnabled)
	{
		GLboolean lighting = GLUtil::GLEnable(GL_LIGHTING, false);
		GLboolean lineSmooth = GLUtil::GLEnable(GL_LINE_SMOOTH, true);
		GLboolean blend = GLUtil::GLEnable(GL_BLEND, false);
		unsigned int color = GLUtil::GLColor(0x0000ffff);
		unsigned int depthFunc = GLUtil::GLDepthFunc(GL_LEQUAL);
		
		if(_wireFrameLinesEnabled)
		{
			GLfloat lineWidth = GLUtil::GLLineWidth(0.75);
			glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );		
			glDrawElements(GL_TRIANGLES, _numIndices, _indicesType, _indicesPointer);
			glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
			GLUtil::GLLineWidth(lineWidth);
		}
		
		if(_wireFramePointsEnabled)
		{
			glColor(0xff0000ff);
			float prevPointSize = GLUtil::GLPointSize(4.0f);
			glDrawElements(GL_POINTS, _numIndices, _indicesType, _indicesPointer);
			GLUtil::GLPointSize(prevPointSize);
		}

		glDepthFunc(depthFunc);
		GLUtil::GLColor(color);
		GLUtil::GLEnable(GL_BLEND, blend);
		GLUtil::GLEnable(GL_LINE_SMOOTH, lineSmooth);
		GLUtil::GLEnable(GL_LIGHTING, lighting);
	}

	if(_verticesPointer)
		glDisableClientState(GL_VERTEX_ARRAY);

	if(_showBoundingShapes)
	{
		for(unsigned int i=0; i<_boundingShapes.size(); i++)
		{
			_boundingShapes[i]->Draw();
		}
	}

	GLboolean lighting1 = GLUtil::GLEnable(GL_LIGHTING, false);
	
	if(_boundingBoxEnabled)
		_aabb.Draw();

	if(_bounding2DRectEnabled)
		DrawBounding2DRect();
	
	GLUtil::GLEnable(GL_LIGHTING, lighting1);

	glPopMatrix();

	if(_drawLocalAxis)
		DrawLocalAxis();
}

vector<CVector3> FLModel::GetAxisLine3DPoints(float* mat, int triIndex)
{
	CVector3 center		( mat[12],	mat[13],	mat[14]);
	CVector3 sideVec	( mat[0],	mat[1],		mat[2] );
	CVector3 upVec		( mat[4],	mat[5],		mat[6] );
	CVector3 forwardVec	( mat[8],	mat[9],		mat[10]);

	sideVec.Normalize();
	upVec.Normalize();
	forwardVec.Normalize();

	CVector3 localAxisVec;

	if(triIndex == 0)		localAxisVec = sideVec;
	else if(triIndex == 1)	localAxisVec = upVec;
	else if(triIndex == 2)	localAxisVec = forwardVec;

	vector<CVector3> vec;
	vec.push_back( CVector3(center.x + localAxisVec.x,	center.y + localAxisVec.y,	center.z + localAxisVec.z) );
	vec.push_back( CVector3(center.x - localAxisVec.x,	center.y - localAxisVec.y,	center.z - localAxisVec.z) );

	return vec;
}

void FLModel::DrawLocalAxis()
{
	GLfloat lineWidth = GLUtil::GLLineWidth(1.0f);
	GLboolean lighting = GLUtil::GLEnable(GL_LIGHTING, false);

	glBegin(GL_LINES);
	
	for(int i=0; i<3; i++)
	{
		vector<CVector3> line3D = GetAxisLine3DPoints(_mat.m, i);

		if(i == 0)		glColor(0xff0000ff);
		else if(i == 1)	glColor(0x00ff00ff);
		else if(i == 2)	glColor(0x0000ffff);

		CVector3 axisVec1 (line3D[1] - line3D[0]);
		CVector3 axisVec2 = axisVec1;

		axisVec1.SetLength(500);
		axisVec2.SetLength(-500);

		axisVec1 += line3D[0];
		axisVec2 += line3D[0];

		glVertex3f(axisVec1.x, axisVec1.y, axisVec1.z);
		glVertex3f(axisVec2.x, axisVec2.y, axisVec2.z);
	}

	glEnd();

	GLUtil::GLEnable(GL_LIGHTING, lighting);
	GLUtil::GLLineWidth(lineWidth);
}

Rect FLModel::GetBounding2DRect(bool multWithLocalMat)
{
	vector<CVector3> pos3D = _aabb.GetAABBVertices();
	
	GLMat mat = GLUtil::GetModelViewMatrix();

	if(multWithLocalMat)
		mat.glMultMatrixf(_mat.m);
	
	vector<CVector3> pos2D = GLUtil::Get2DPosOnScreenFrom3DPos(&pos3D, mat.m);

	CVector3 min;
	CVector3 max;

	GLUtil::GetMinMaxPoints(&pos2D, &min, &max);

	Rect rect;
	rect.SetBoundsByPoints(--min.x, --min.y, ++max.x, ++max.y);
	rect.FitIntoBoundry(0, 0, GLUtil::GetWindowWidth(), GLUtil::GetWindowHeight());

	return rect;
}

void FLModel::DrawBounding2DRect()
{
	Rect rect = GetBounding2DRect(false);

	state2D.Begin(0xff0000ff, 2.0f, 1.0f, false, false);
	
	rect.DrawWithLines();

	state2D.End();
}

void FLModel::CalcBorder()
{
	Rect rect = GetBounding2DRect(true);

	_border->CalcBorder((float*)_verticesPointer, _numIndices, _indicesType, _indicesPointer, _mat.m, &rect);
}

void FLModel::DrawBorder()
{
	_border->DrawBorder();
}

FLModel::~FLModel()
{
	for(unsigned int i=0; i<_boundingShapes.size(); i++)
	{
		if(_boundingShapes[i])
		{
			delete _boundingShapes[i];
			_boundingShapes[i] = NULL;
		}
	}

	_boundingShapes.clear();

	if(_flmReaderWriter)
	{
		delete _flmReaderWriter;
		_flmReaderWriter = NULL;
	}

	if(_border)
	{
		delete _border;
		_border = NULL;
	}
}
