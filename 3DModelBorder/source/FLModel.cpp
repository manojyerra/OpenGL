#include "FLModel.h"
#include "CFileReader.h"
#include "ImageBuffer.h"
#include "UtilFuncs.h"
#include "GLAndUtil.h"

FLModel::FLModel(string folderPath)
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

void FLModel::SetTextureEnable(bool enable)
{
	_isTextureEnabled = enable;
}

bool FLModel::IsTextureEnabled()
{
	return _isTextureEnabled;
}

void FLModel::Draw()
{
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


	////Begin : WireFrame

	//bool lighting = util::GLEnable(GL_LIGHTING, false);
	//bool lineSmooth = util::GLEnable(GL_LINE_SMOOTH, true);
	//GLfloat lineWidth = util::GLLineWidth(2);
	//unsigned int color = util::GLColor(0x000000ff);

	//glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	//glDepthFunc(GL_LEQUAL);
	//
	//glDrawElements(GL_TRIANGLES, _numIndices, _indicesType, _indicesPointer);

	//glDepthFunc(GL_LESS);
	//glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

	//util::GLColor(color);
	//util::GLLineWidth(lineWidth);
	//util::GLEnable(GL_LINE_SMOOTH, lineSmooth);
	//util::GLEnable(GL_LIGHTING, lighting);

	////End : WireFrame


	if(_verticesPointer)
		glDisableClientState(GL_VERTEX_ARRAY);


	//bool lighting1 = util::GLEnable(GL_LIGHTING, false);
	//_bBox.Draw();
	//DrawBounding2DRect();
	//util::GLEnable(GL_LIGHTING, lighting1);
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

	GLAndUtil::Begin2DDraw();
	
	GLfloat prevLineWidth = util::GLLineWidth(2);
	unsigned int prevColor = util::GLColor(0xff0000ff);

	glBegin(GL_LINE_LOOP);
	glVertex2f(minX, minY);
	glVertex2f(minX+rectW, minY);
	glVertex2f(minX+rectW, minY+rectH);
	glVertex2f(minX, minY+rectH);
	glEnd();

	util::GLLineWidth(prevLineWidth);
	util::GLColor(prevColor);
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
	SetTextureEnable(false);
	unsigned int color = util::GLColor(0x00000000);

	Draw();

    GLubyte* data = (GLubyte*)malloc(width*height*4);
	glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);

	glClearColor(clearColor[0], clearColor[1], clearColor[2], clearColor[3]);
	util::GLEnable(GL_DEPTH_TEST, glDepthTest);
	util::GLEnable(GL_LIGHTING, glLighting);
	util::GLEnable(GL_BLEND, glBlend);
	SetTextureEnable(true);
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
}
