#include "FLModel.h"
#include "CFileReader.h"
#include "ImageBuffer.h"
#include "Util/GLUtil.h"
#include "Cam.h"

FLModel::FLModel(string folderPath)
{
	FILE* matFile = fopen( GetOrientationFilePath(folderPath).c_str(), "r" );

	GLMat mat;
	float* m = mat.m;

	if(matFile)
	{
		fscanf(matFile, "%f %f %f %f\n",&m[0] ,&m[1] ,&m[2] ,&m[3]);
		fscanf(matFile, "%f %f %f %f\n",&m[4] ,&m[5] ,&m[6] ,&m[7]);
		fscanf(matFile, "%f %f %f %f\n",&m[8] ,&m[9] ,&m[10] ,&m[11]);
		fscanf(matFile, "%f %f %f %f",&m[12] ,&m[13] ,&m[14] ,&m[15]);

		fclose(matFile);
	}

	Reset(folderPath, mat.m);
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

	_mat.Copy(mat);

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

	if(CFileReader::IsFileExists(folderPath+"/vertex.buf"))
	{
		CFileReader* fileReader = new CFileReader(folderPath+"/vertex.buf", "rb");
		_verticesPointer = (unsigned char*)malloc(fileReader->GetLength());
		memcpy(_verticesPointer, fileReader->GetData(), fileReader->GetLength());
		_numVertex = fileReader->GetLength() / 12;
		delete fileReader;
	}

	if(CFileReader::IsFileExists(folderPath+"/uv.buf"))
	{
		CFileReader* fileReader = new CFileReader(folderPath+"/uv.buf", "rb");
		_uvsPointer = (unsigned char*)malloc(fileReader->GetLength());
		memcpy(_uvsPointer, fileReader->GetData(), fileReader->GetLength());
		delete fileReader;
	}

	if(CFileReader::IsFileExists(folderPath+"/normal.buf"))
	{
		CFileReader* fileReader = new CFileReader(folderPath+"/normal.buf", "rb");
		_normalsPointer = (unsigned char*)malloc(fileReader->GetLength());
		memcpy(_normalsPointer, fileReader->GetData(), fileReader->GetLength());
		delete fileReader;
	}

	if(CFileReader::IsFileExists(folderPath+"/index.buf"))
	{
		CFileReader* fileReader = new CFileReader(folderPath+"/index.buf", "rb");
		int fileLen = fileReader->GetLength();
		_indicesPointer = (unsigned char*)malloc(fileLen);
		memcpy(_indicesPointer, fileReader->GetData(), fileLen);
		delete fileReader;

		int val = _indicesPointer[fileLen-4];

		if(val == 1)		_indicesType = GL_UNSIGNED_BYTE;
		else if(val == 2)	_indicesType = GL_UNSIGNED_SHORT;
		else if(val == 4)	_indicesType = GL_UNSIGNED_INT;

		_numIndices = (fileLen-4)/val;
	}

	ImageBuffer* imgBuf = new ImageBuffer(folderPath+"/texture.png");

	_textureID = GLUtil::GenerateGLTextureID(	imgBuf->GetWidth(), imgBuf->GetHeight(), 
											imgBuf->GetBytesPerPixel(), imgBuf->GetBuffer());

	delete imgBuf;


	FILE* bBoxFile = fopen( GetBBoxFilePath(_folderPath).c_str(), "r" );

	if(bBoxFile == NULL)
	{
		_bBox.CalcBBox((float*)_verticesPointer, _numVertex);
	}
	else
	{
		fscanf(bBoxFile, "%f %f %f %f %f %f",&_bBox.minX, &_bBox.minY, &_bBox.minZ, 
												&_bBox.maxX, &_bBox.maxY, &_bBox.maxZ);
		fflush(bBoxFile);
		fclose(bBoxFile);
	}
}

void FLModel::Save()
{
	Save(_folderPath);
}

void FLModel::Save(string folderPath)
{
	SaveOrientation(folderPath);
	SaveBBoxInfo(folderPath);
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

void FLModel::SaveOrientation(string folderPath)
{
	FILE* matFile = fopen(GetOrientationFilePath(folderPath).c_str(), "w");
	
	fprintf(matFile, "%f %f %f %f\n",_mat.m[0], _mat.m[1], _mat.m[2], _mat.m[3]);
	fprintf(matFile, "%f %f %f %f\n",_mat.m[4], _mat.m[5], _mat.m[6], _mat.m[7]);
	fprintf(matFile, "%f %f %f %f\n",_mat.m[8], _mat.m[9], _mat.m[10], _mat.m[11]);
	fprintf(matFile, "%f %f %f %f",	_mat.m[12], _mat.m[13], _mat.m[14], _mat.m[15]);

	fflush(matFile);
	fclose(matFile);
}

void FLModel::SaveBBoxInfo(string folderPath)
{
	FILE* bBoxFile = fopen( GetBBoxFilePath(folderPath).c_str(), "w" );
	
	fprintf(bBoxFile, "%f %f %f %f %f %f", _bBox.minX,_bBox.minY,_bBox.minZ,_bBox.maxX,_bBox.maxY,_bBox.maxZ);

	fflush(bBoxFile);
	fclose(bBoxFile);
}

string FLModel::GetOrientationFilePath(string folderPath)
{
	return folderPath+"/transformation.txt";
}

string FLModel::GetBBoxFilePath(string folderPath)
{
	return folderPath+"/bbox.txt";
}

void FLModel::SetTextureEnabled(bool enable)			{	_isTextureEnabled = enable;			}
bool FLModel::IsTextureEnabled()						{	return _isTextureEnabled;			}
void FLModel::SetWireFrameLinesEnabled(bool enable)		{	_wireFrameLinesEnabled = enable;	}
bool FLModel::IsWireFrameLinesEnabled()					{	return _wireFrameLinesEnabled;		}
void FLModel::SetWireFramePointsEnabled(bool enable)	{	_wireFramePointsEnabled = enable;	}
bool FLModel::IsWireFramePointsEnabled()				{	return _wireFramePointsEnabled;		}
void FLModel::SetBoundingBoxEnabled(bool enable)		{	_boundingBoxEnabled = enable;		}
bool FLModel::IsBoundingBoxEnabled()					{	return _boundingBoxEnabled;			}
void FLModel::SetBounding2DRectEnabled(bool enable)		{	_bounding2DRectEnabled = enable;	}
bool FLModel::IsBounding2DRectEnabled()					{	return _bounding2DRectEnabled;		}
void FLModel::SetLightingEnabled(bool enable)			{	_lightingEnabled = enable;			}
bool FLModel::IsLightingEnabled()						{	return _lightingEnabled;			}
void FLModel::ShowBoundingShapes(bool show)				{	_showBoundingShapes = show;			}
bool FLModel::IsShowingBoundingShapes()					{	return _showBoundingShapes;			}
void FLModel::ShowModel(bool show)						{	_showModel = show;					}
bool FLModel::IsShowingModel()							{	return _showModel;					}
void FLModel::ShowLocalAxis(bool show)					{	_drawLocalAxis = show;				}
bool FLModel::IsShowingLocalAxis()						{	return _drawLocalAxis;				}
void FLModel::SetMarked(bool mark)						{	_isMarked = mark;				}
bool FLModel::IsMarked()								{	return _isMarked;				}


void FLModel::SetMeterial(int lightParam, float r, float g, float b, float a)
{
	if(lightParam == GL_AMBIENT)		{	_ka[0] =r; _ka[1] =g; _ka[2] =b; _ka[3] =a;	}
	else if(lightParam == GL_DIFFUSE)	{	_kd[0] =r; _kd[1] =g; _kd[2] =b; _kd[3] =a;	}
	else if(lightParam == GL_SPECULAR)	{	_ks[0] =r; _ks[1] =g; _ks[2] =b; _ks[3] =a;	}
}

unsigned int FLModel::GetMeterial(int lightParam)
{
	if(lightParam == GL_AMBIENT)		
	{	
		return GLUtil::GetUInt(_ka[0], _ka[1], _ka[2], _ka[3]);
	}
	else if(lightParam == GL_DIFFUSE)	
	{	
		return GLUtil::GetUInt(_kd[0], _kd[1], _kd[2], _kd[3]);
	}
	else if(lightParam == GL_SPECULAR)	
	{	
		return GLUtil::GetUInt(_ks[0], _ks[1], _ks[2], _ks[3]);
	}

	return 0;
}

void FLModel::SetShininess(float val)
{
	_shininess = val;
}

float FLModel::GetShininess()
{
	return _shininess;
}

void FLModel::SetPos(float x, float y, float z)
{
	_mat.m[12] = x;
	_mat.m[13] = y;
	_mat.m[14] = z;
}

void FLModel::SetPos(CVector3 pos)
{
	_mat.m[12] = pos.x;
	_mat.m[13] = pos.y;
	_mat.m[14] = pos.z;
}

CVector3 FLModel::GetPos()
{
	return CVector3(_mat.m[12], _mat.m[13], _mat.m[14]);
}

void FLModel::AddTransInWorld(float x, float y, float z)
{
	_mat.m[12] += x;
	_mat.m[13] += y;
	_mat.m[14] += z;
}

void FLModel::AddRotateInWorld(char axis, float angle)
{
	GLMat newRot;
	
	if(axis == 'x' || axis == 'X')	newRot.glRotatef(angle, 1,0,0);
	if(axis == 'y' || axis == 'Y')	newRot.glRotatef(angle, 0,1,0);
	if(axis == 'z' || axis == 'Z')	newRot.glRotatef(angle, 0,0,1);

	newRot.glMultMatrixf( _mat.m );

	_mat.Copy(newRot.m);
}

void FLModel::AddTransInLocal(char axis, float move)
{
	CVector3 vec;

	if(axis == 'x')			vec = CVector3( _mat.m[0], _mat.m[1], _mat.m[2] );
	else if(axis == 'y')	vec = CVector3( _mat.m[4], _mat.m[5], _mat.m[6] );
	else if(axis == 'z')	vec = CVector3( _mat.m[8], _mat.m[9], _mat.m[10] );
	else
	{
		return;
	}
	
	vec.Normalize();
	vec *= move;

	_mat.m[12] += vec.x;
	_mat.m[13] += vec.y;
	_mat.m[14] += vec.z;
}

void FLModel::AddRotateInLocal(char axis, float angle)
{
	CVector3 scale = _mat.GetScale();

	_mat.RemoveScale();

	GLMat rotMat;
	
	if(axis == 'x' || axis == 'X')	rotMat.glRotatef(angle, 1,0,0);
	if(axis == 'y' || axis == 'Y')	rotMat.glRotatef(angle, 0,1,0);
	if(axis == 'z' || axis == 'Z')	rotMat.glRotatef(angle, 0,0,1);

	_mat.glMultMatrixf(rotMat.Get());

	_mat.SetScale( scale );
}

CVector3 FLModel::GetRotation()
{
	GLMat newMat;
	newMat.Copy(_mat.m);
	newMat.RemoveScale();

	return newMat.GetEulerXYZRot_In_Degrees();
}

void FLModel::SetRotation(CVector3 rot)
{
	CVector3 scale = _mat.GetScale();
	_mat.RemoveScale();

	GLMat newMat;
	
	newMat.glTranslatef( _mat.m[12], _mat.m[13], _mat.m[14] );
	newMat.glRotatef(rot.z, 0,0,1);
	newMat.glRotatef(rot.y, 0,1,0);
	newMat.glRotatef(rot.x, 1,0,0);
	newMat.glScalef(scale.x, scale.y, scale.z);

	_mat.Copy(newMat.m);
}

void FLModel::AddScale(CVector3 scale)
{
	_mat.glScalef(scale.x, scale.y, scale.z);
}

void FLModel::AddUniformScale(float scale)
{
	_mat.glScalef(scale, scale, scale);
}

vector<float> FLModel::GetVerticesOnRect(int x, int y, int w, int h)
{
	float* verArr = (float*)_verticesPointer;

	float xy[2];

	vector<float> vec;

	GLMat modelViewMatrix = GLUtil::GetModelViewMatrix();
	modelViewMatrix.glMultMatrixf(_mat.m);

	GLMat projMatrix = GLUtil::GetProjectionMatrix();

	for(unsigned int i=0; i<_numVertex*3; i+=3)
	{
		//Cam::GetInstance()->Get2DPosOnScreenFrom3DPos(&verArr[i], xy, modelViewMatrix.m);

		GLUtil::Get2DPosOnScreenFrom3DPos(&verArr[i], xy, modelViewMatrix.m, projMatrix.m);

		if( xy[0] >= x && xy[0] <= x+w && xy[1] >= y && xy[1] <= y+h )
		{
			vec.push_back(verArr[i+0]);
			vec.push_back(verArr[i+1]);
			vec.push_back(verArr[i+2]);
		}
	}

	return vec;
}

Shape* FLModel::AddBestBoudingShapeByVerticesOnRect(int x, int y, int w, int h)
{
	vector<float> verVec = GetVerticesOnRect(x, y, w, h);
	
	Shape* bShape = NULL;

	if(verVec.size() > 0)
	{
		bShape = Shape::GetBestFitBoundingShape(&verVec[0], verVec.size());

		_boundingShapes.push_back(bShape);
	}

	return bShape;
}

Shape* FLModel::AddBoudingShapeByVerticesOnRect(int x, int y, int w, int h, int boundingShapeID)
{
	vector<float> verVec = GetVerticesOnRect(x, y, w, h);
	
	Shape* bShape = NULL;

	if(verVec.size() > 0)
	{
		bShape = Shape::GetBoundingShape(&verVec[0], verVec.size(), boundingShapeID);

		if(bShape)
			_boundingShapes.push_back(bShape);
	}

	return bShape;
}

void FLModel::AddBoundingShape(Shape* shape)
{
	_boundingShapes.push_back(shape);
}

void FLModel::Draw()
{
	bool isLightOn = GLUtil::GLEnable(GL_LIGHTING, _lightingEnabled);

	glPushMatrix();
	glMultMatrixf(_mat.m);

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
		_bBox.Draw();

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

void FLModel::GetBounding2DRect(int* x, int* y, int* w, int* h, bool multWithLocalMat)
{
	float pos2D[8][2];

	float pos3D[8][3] = {	
							{_bBox.minX, _bBox.minY, _bBox.minZ},
							{_bBox.minX, _bBox.maxY, _bBox.minZ},
							{_bBox.maxX, _bBox.minY, _bBox.minZ},
							{_bBox.maxX, _bBox.maxY, _bBox.minZ},
							{_bBox.minX, _bBox.minY, _bBox.maxZ},
							{_bBox.minX, _bBox.maxY, _bBox.maxZ},
							{_bBox.maxX, _bBox.minY, _bBox.maxZ},
							{_bBox.maxX, _bBox.maxY, _bBox.maxZ}
						};

	GLMat mat = GLUtil::GetModelViewMatrix();
	GLMat projMat = GLUtil::GetProjectionMatrix();

	if(multWithLocalMat)
		mat.glMultMatrixf(_mat.m);
	
	for(int i=0; i<8; i++)
		GLUtil::Get2DPosOnScreenFrom3DPos(pos3D[i], pos2D[i], mat.m, projMat.m);

	float minX = pos2D[0][0];
	float maxX = pos2D[0][0];

	float minY = pos2D[0][1];
	float maxY = pos2D[0][1];

	for(int i=1; i<8; i++)
	{
		if(pos2D[i][0] < minX) minX = pos2D[i][0];
		if(pos2D[i][0] > maxX) maxX = pos2D[i][0];
		if(pos2D[i][1] < minY) minY = pos2D[i][1];
		if(pos2D[i][1] > maxY) maxY = pos2D[i][1];
	}

	minX--;
	minY--;
	maxX++;
	maxY++;

	if(minX < 0)	minX = 0;
	if(minY < 0)	minY = 0;
	if(maxX > GLUtil::GetWindowWidth() ) maxX = GLUtil::GetWindowWidth();
	if(maxY > GLUtil::GetWindowHeight() ) maxY = GLUtil::GetWindowHeight();

	if(maxX < 0)	maxX = 0;
	if(maxY < 0)	maxY = 0;
	if(minX > GLUtil::GetWindowWidth() ) minX = GLUtil::GetWindowWidth();
	if(minY > GLUtil::GetWindowHeight() ) minY = GLUtil::GetWindowHeight();

	float rectW = maxX - minX;
	float rectH = maxY - minY;

	x[0] = (int)minX;
	y[0] = (int)minY;
	w[0] = (int)rectW;
	h[0] = (int)rectH;
}

void FLModel::DrawBounding2DRect()
{
	int x,y,w,h;
	GetBounding2DRect(&x, &y, &w, &h, false);

	state2D.Begin(0xff0000ff, 2.0f, 1.0f, false, false);

	glBegin(GL_LINE_LOOP);
	glVertex2f(x+0, y+0);
	glVertex2f(x+w, y+0);
	glVertex2f(x+w, y+h);
	glVertex2f(x+0, y+h);
	
	glEnd();

	state2D.End();
}


void FLModel::CalcBorder()
{
	_borderVec.clear();

	GLfloat clearColor[4];
	GLUtil::GLClearColor(1,1,1,1, clearColor);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	GLboolean light = GLUtil::GLEnable(GL_LIGHTING, false);
	GLboolean blend = GLUtil::GLEnable(GL_BLEND, false);
	GLboolean depthTest = GLUtil::GLEnable(GL_DEPTH_TEST, true);
	unsigned int prevColor = GLUtil::GLColor(0x000000ff);

		bool bounding2DRect			= IsBounding2DRectEnabled();
		bool boundingBox			= IsBoundingBoxEnabled();
		bool boundingShapes			= IsShowingBoundingShapes();
		bool textureEnable			= IsTextureEnabled();
		bool wireFrameLinesEnable	= IsWireFrameLinesEnabled();
		bool wireFramePointsEnable	= IsWireFramePointsEnabled();
		bool lightingOn				= IsLightingEnabled();
		bool localAxis				= IsShowingLocalAxis();

		SetBounding2DRectEnabled(false);
		SetBoundingBoxEnabled(false);
		SetTextureEnabled(false);
		SetWireFrameLinesEnabled(false);
		SetWireFramePointsEnabled(false);
		ShowBoundingShapes(false);
		SetLightingEnabled(false);
		ShowLocalAxis(false);

		Draw();

		int x,y,w,h;
		GetBounding2DRect(&x, &y, &w, &h, true);

		GLubyte* data = (GLubyte*)malloc(w*h*4);
		memset(data, 255, w*h*4);

		GLUtil::GLReadPixelsFromTopLeft(x, y, w, h, GL_RGBA, GL_UNSIGNED_BYTE, data);

		SetBounding2DRectEnabled(bounding2DRect);
		SetBoundingBoxEnabled(boundingBox);
		SetTextureEnabled(textureEnable);
		SetWireFrameLinesEnabled(wireFrameLinesEnable);
		SetWireFramePointsEnabled(wireFramePointsEnable);
		ShowBoundingShapes(boundingShapes);
		SetLightingEnabled(lightingOn);
		ShowLocalAxis(localAxis);

	GLUtil::GLEnable(GL_LIGHTING, light);
	GLUtil::GLEnable(GL_BLEND, blend);
	GLUtil::GLEnable(GL_DEPTH_TEST, depthTest);
	GLUtil::GLColor(prevColor);


	int prevVal = 0;

	for(int j=0;j<h;j++)
	{
		for(int i=0;i<w;i++)
		{
			int pos = ((j*w) + i) * 4;

			if(i != 0 && data[pos] != prevVal)
			{
				_borderVec.push_back((float)(i + x));
				_borderVec.push_back((float)(h - j + y));
			}
			prevVal = data[pos];
		}
	}

	prevVal = 0;

	for(int i=0;i<w;i++)
	{
		for(int j=0;j<h;j++)
		{
			int pos = ((j*w) + i) * 4;

			if(j != 0 && data[pos] != prevVal)
			{
				_borderVec.push_back((float)(i + x));
				_borderVec.push_back((float)(h - j + y));
			}
			prevVal = data[pos];
		}
	}

	free(data);
}

void FLModel::DrawBorder()
{
	if(_borderVec.size() > 0)
	{
		state2D.Begin(0xff0000ff, 1.0f, 1.0f, false, false);

		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(2, GL_FLOAT, 0, &_borderVec[0]);
		glDrawArrays(GL_POINTS, 0, _borderVec.size()/2);
		glDisableClientState(GL_VERTEX_ARRAY);

		state2D.End();
	}

	_borderVec.clear();
}


FLModel::~FLModel()
{
	if(_verticesPointer)
		free(_verticesPointer);
	
	if(_uvsPointer)
		free(_uvsPointer);

	if(_normalsPointer)
		free(_normalsPointer);

	if(_indicesPointer)
		free(_indicesPointer);

	for(unsigned int i=0; i<_boundingShapes.size(); i++)
	{
		if(_boundingShapes[i])
		{
			delete _boundingShapes[i];
			_boundingShapes[i] = NULL;
		}
	}

	_boundingShapes.clear();
}