#include "FLModel.h"
#include "CFileReader.h"
#include "ImageBuffer.h"
#include "UtilFuncs.h"
#include "GLAndUtil.h"

FLModel::FLModel(string folderPath)
{
	GLMat mat;
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
	_verticesPointer = NULL;
	_uvsPointer = NULL;
	_normalsPointer = NULL;
	_indicesPointer = NULL;
	_textureID = 0;
	_indicesType = 0;
	_numIndices = 0;
	_numVertex = 0;
	_isTextureEnabled = true;
	_wireFrameEnabled = false;
	_boundingBoxEnabled = false;
	_bounding2DRectEnabled = false;
	_mat.Copy(mat);

	GLfloat ka[4] = { 0.600000, 0.600000, 0.600000, 1.0};
	GLfloat kd[4] = { 0.600000, 0.600000, 0.600000, 1.0};
	GLfloat ks[4] = { 0.900000, 0.900000, 0.900000, 1.0};

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
	_textureID = UtilFuncs::GenerateGLTextureID(imgBuf);

	_bBox.CalcBBox((float*)_verticesPointer, _numVertex);
}

void FLModel::SetTextureEnabled(bool enable)			{	_isTextureEnabled = enable;		}
bool FLModel::IsTextureEnabled()						{	return _isTextureEnabled;		}
void FLModel::SetWireFrameEnabled(bool enable)			{	_wireFrameEnabled = enable;		}
bool FLModel::IsWireFrameEnabled()						{	return _wireFrameEnabled;		}
void FLModel::SetBoundingBoxEnabled(bool enable)		{	_boundingBoxEnabled = enable;	}
bool FLModel::IsBoundingBoxEnabled()					{	return _boundingBoxEnabled;		}
void FLModel::SetBounding2DRectEnabled(bool enable)		{	_bounding2DRectEnabled = enable;}
bool FLModel::IsBounding2DRectEnabled()					{	return _bounding2DRectEnabled;	}

void FLModel::SetMeterial(int lightParam, float r, float g, float b, float a)
{
	if(lightParam == GL_AMBIENT)		{	_ka[0] =r; _ka[1] =g; _ka[2] =b; _ka[3] =a;	}
	else if(lightParam == GL_DIFFUSE)	{	_kd[0] =r; _kd[1] =g; _kd[2] =b; _kd[3] =a;	}
	else if(lightParam == GL_SPECULAR)	{	_ks[0] =r; _ks[1] =g; _ks[2] =b; _ks[3] =a;	}
}

void FLModel::SetShininess(float val)
{
	_shininess = val;
}

void FLModel::SetPos(float x, float y, float z)
{
	_mat.m[12] = x;
	_mat.m[13] = y;
	_mat.m[14] = z;
}

void FLModel::SetPos(CVector3 pos)
{
	_mat.m[12] += pos.x;
	_mat.m[13] += pos.y;
	_mat.m[14] += pos.z;
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
	
	vec.Normalize();
	vec *= move;

	_mat.m[12] += vec.x;
	_mat.m[13] += vec.y;
	_mat.m[14] += vec.z;
}

void FLModel::AddRotateInLocal(char axis, float angle)
{
	GLMat rotMat;
	
	if(axis == 'x' || axis == 'X')	rotMat.glRotatef(angle, 1,0,0);
	if(axis == 'y' || axis == 'Y')	rotMat.glRotatef(angle, 0,1,0);
	if(axis == 'z' || axis == 'Z')	rotMat.glRotatef(angle, 0,0,1);

	_mat.glMultMatrixf(rotMat.Get());
}

void FLModel::Draw()
{
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


	if(_wireFrameEnabled)
	{
		bool lighting = util::GLEnable(GL_LIGHTING, false);
		bool lineSmooth = util::GLEnable(GL_LINE_SMOOTH, true);
		bool blend = util::GLEnable(GL_BLEND, false);
		GLfloat lineWidth = util::GLLineWidth(1);
		unsigned int color = util::GLColor(0x000000ff);
		unsigned int depthFunc = util::GLDepthFunc(GL_LEQUAL);
		glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
		
		glDrawElements(GL_TRIANGLES, _numIndices, _indicesType, _indicesPointer);

		glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
		glDepthFunc(depthFunc);
		util::GLColor(color);
		util::GLLineWidth(lineWidth);
		util::GLEnable(GL_BLEND, blend);
		util::GLEnable(GL_LINE_SMOOTH, lineSmooth);
		util::GLEnable(GL_LIGHTING, lighting);
	}

	if(_verticesPointer)
		glDisableClientState(GL_VERTEX_ARRAY);


	bool lighting1 = util::GLEnable(GL_LIGHTING, false);
	
	if(_boundingBoxEnabled)
		_bBox.Draw();

	if(_bounding2DRectEnabled)
		DrawBounding2DRect();
	
	util::GLEnable(GL_LIGHTING, lighting1);

	glPopMatrix();
}

void FLModel::DrawBounding2DRect()
{
	float pos2D[8][2];

	float pos3D[8][3] = {	{_bBox.minX, _bBox.minY, _bBox.minZ},
							{_bBox.minX, _bBox.maxY, _bBox.minZ},
							{_bBox.maxX, _bBox.minY, _bBox.minZ},
							{_bBox.maxX, _bBox.maxY, _bBox.minZ},
							{_bBox.minX, _bBox.minY, _bBox.maxZ},
							{_bBox.minX, _bBox.maxY, _bBox.maxZ},
							{_bBox.maxX, _bBox.minY, _bBox.maxZ},
							{_bBox.maxX, _bBox.maxY, _bBox.maxZ},
						};

	for(int i=0; i<8; i++)
		GLAndUtil::Get2DPosOnScreenFrom3DPos(pos3D[i], pos2D[i]);

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

	float rectW = maxX - minX;
	float rectH = maxY - minY;


	GLMat modelMat = util::GetModelViewMatrix();
	GLMat projMat = util::GetProjectionMatrix();
	bool depthTest = util::GLEnable(GL_DEPTH_TEST, false);
	bool lighting = util::GLEnable(GL_LIGHTING, false);
	bool blend = util::GLEnable(GL_BLEND, false);
	unsigned int prevColor = util::GLColor(0xff0000ff);
	GLfloat prevLineWidth = util::GLLineWidth(2);

	GLAndUtil::Begin2DDraw();

	glBegin(GL_LINE_LOOP);
	glVertex2f(minX, minY);
	glVertex2f(minX+rectW, minY);
	glVertex2f(minX+rectW, minY+rectH);
	glVertex2f(minX, minY+rectH);
	glEnd();


	util::GLLineWidth(prevLineWidth);
	util::GLColor(prevColor);
	util::GLEnable(GL_BLEND, blend);
	util::GLEnable(GL_LIGHTING, depthTest);
	util::SetProjectionMatrix(projMat);
	util::SetModelViewMatrix(modelMat);
	util::GLEnable(GL_DEPTH_TEST, depthTest);
}


void FLModel::CalcBorder()
{
	_borderVec.clear();

	int width = util::GetWindowWidth();
	int height = util::GetWindowHeight();

	GLfloat clearColor[4];
	util::GLClearColor(1,1,1,1, clearColor);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	bool glDepthTest = util::GLEnable(GL_DEPTH_TEST, false);
	bool glLighting = util::GLEnable(GL_LIGHTING, false);
	bool glBlend = util::GLEnable(GL_BLEND, false);
	SetTextureEnabled(false);
	unsigned int color = util::GLColor(0x00000000);

	Draw();

    GLubyte* data = (GLubyte*)malloc(width*height*4);
	glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);

	glClearColor(clearColor[0], clearColor[1], clearColor[2], clearColor[3]);
	util::GLEnable(GL_DEPTH_TEST, glDepthTest);
	util::GLEnable(GL_LIGHTING, glLighting);
	util::GLEnable(GL_BLEND, glBlend);
	SetTextureEnabled(true);
	util::GLColor(color);


	int prevVal = 0;

	for(int j=0;j<height;j++)
	{
		for(int i=0;i<width;i++)
		{
			int pos = ((j*width) + i) * 4;

			if(i != 0 && data[pos] != prevVal)
			{
				_borderVec.push_back(i);
				_borderVec.push_back(height-j);
			}
			prevVal = data[pos];
		}
	}

	prevVal = 0;

	for(int i=0;i<width;i++)
	{
		for(int j=0;j<height;j++)
		{
			int pos = ((j*width) + i) * 4;

			if(j != 0 && data[pos] != prevVal)
			{
				_borderVec.push_back(i);
				_borderVec.push_back(height-j);
			}
			prevVal = data[pos];
		}
	}

	free(data);
}

void FLModel::DrawBorder()
{
	GLMat modelMat = util::GetModelViewMatrix();
	GLMat projMat = util::GetProjectionMatrix();
	bool depthTest = util::GLEnable(GL_DEPTH_TEST, false);

	GLAndUtil::Begin2DDraw();
	
	bool glDepthTest = util::GLEnable(GL_DEPTH_TEST, false);
	bool glLighting = util::GLEnable(GL_LIGHTING, false);
	bool glBlend = util::GLEnable(GL_BLEND, false);
	unsigned int prevColor = util::GLColor(0xff0000ff);
	GLfloat pointSize = util::GLPointSize(1.0f);

	glBegin(GL_POINTS);

	for(int i=0; i<_borderVec.size(); i+=2)
		glVertex2f(_borderVec[i], _borderVec[i+1]);

	glEnd();

	util::GLEnable(GL_DEPTH_TEST, glDepthTest);
	util::GLEnable(GL_LIGHTING, glLighting);
	util::GLEnable(GL_BLEND, glBlend);
	util::GLColor(prevColor);
	util::GLPointSize(pointSize);

	util::GLEnable(GL_DEPTH_TEST, depthTest);
	util::SetProjectionMatrix(projMat);
	util::SetModelViewMatrix(modelMat);
}
