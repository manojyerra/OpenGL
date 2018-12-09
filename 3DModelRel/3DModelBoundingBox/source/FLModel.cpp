#include "FLModel.h"
#include "CFileReader.h"
#include "ImageBuffer.h"
#include "UtilFuncs.h"

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

	float* vertexArr = (float*)_verticesPointer;
	_minX = vertexArr[0];
	_maxX = vertexArr[0];
	_minY = vertexArr[1];
	_maxY = vertexArr[1];
	_minZ = vertexArr[2];
	_maxZ = vertexArr[2];

	for(int i=3; i<_numVertex*3; i+=3)
	{
		float x = vertexArr[i+0];
		float y = vertexArr[i+1];
		float z = vertexArr[i+2];

		if(x < _minX) _minX = x;
		if(x > _maxX) _maxX = x;

		if(y < _minY) _minY = y;
		if(y > _maxY) _maxY = y;
		
		if(z < _minZ) _minZ = z;
		if(z > _maxZ) _maxZ = z;
	}
}


void FLModel::Draw()
{
	if(_textureID > 0 && _uvsPointer)
	{
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, _textureID);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer( 2, GL_FLOAT, 0, _uvsPointer );
	}
	
	if(_verticesPointer)
	{
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer( 3, GL_FLOAT, 0, _verticesPointer );
	}

	if(_normalsPointer)
	{
		glEnableClientState(GL_NORMAL_ARRAY);
		glNormalPointer(GL_FLOAT, 0, _normalsPointer );
	}

	glDrawElements(GL_TRIANGLES, _numIndices, _indicesType, _indicesPointer);

	if(_normalsPointer)
		glDisableClientState(GL_NORMAL_ARRAY);

	if(_textureID > 0 && _uvsPointer)
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	if(_verticesPointer)
		glDisableClientState(GL_VERTEX_ARRAY);


	//Drawing bounding box...

	bool lightEnabled = glIsEnabled( GL_LIGHTING );

	glDisable(GL_LIGHTING);
	DrawBoundingBox();

	if(lightEnabled)	glEnable(GL_LIGHTING);	else	glDisable(GL_LIGHTING);
}

void FLModel::DrawBoundingBox()
{
	float cx = ( _minX + _maxX ) / 2.0f;
	float cy = ( _minY + _maxY ) / 2.0f;
	float cz = ( _minZ + _maxZ ) / 2.0f;

	float w = ( _maxX - _minX ) / 2.0f;
	float h = ( _maxY - _minY ) / 2.0f;
	float d = ( _maxZ - _minZ ) / 2.0f;

	glColor4f(0, 1.0f, 0, 0.5);

	glBegin(GL_QUADS);

	// Front Face
	glVertex3f(cx-w, cy-h,  cz+d);  // Bottom Left Of The Texture and Quad
	glVertex3f(cx+w, cy-h,  cz+d);  // Bottom Right Of The Texture and Quad
	glVertex3f(cx+w, cy+h,  cz+d);  // Top Right Of The Texture and Quad
	glVertex3f(cx-w, cy+h,  cz+d);  // Top Left Of The Texture and Quad

	// Back Face
	glVertex3f(cx-w, cy-h, cz-d);  // Bottom Right Of The Texture and Quad
	glVertex3f(cx-w, cy+h, cz-d);  // Top Right Of The Texture and Quad
	glVertex3f(cx+w, cy+h, cz-d);  // Top Left Of The Texture and Quad
	glVertex3f(cx+w, cy-h, cz-d);  // Bottom Left Of The Texture and Quad

	// Top Face
	glVertex3f(cx-w, cy+h, cz-d);  // Top Left Of The Texture and Quad
	glVertex3f(cx-w, cy+h, cz+d);  // Bottom Left Of The Texture and Quad
	glVertex3f(cx+w, cy+h, cz+d);  // Bottom Right Of The Texture and Quad
	glVertex3f(cx+w, cy+h, cz-d);  // Top Right Of The Texture and Quad
	
	// Bottom Face
	glVertex3f(cx-w, cy-h, cz-d);  // Top Right Of The Texture and Quad
	glVertex3f(cx+w, cy-h, cz-d);  // Top Left Of The Texture and Quad
	glVertex3f(cx+w, cy-h, cz+d);  // Bottom Left Of The Texture and Quad
	glVertex3f(cx-w, cy-h, cz+d);  // Bottom Right Of The Texture and Quad

	// Right face
	glVertex3f(cx+w, cy-h, cz-d);  // Bottom Right Of The Texture and Quad
	glVertex3f(cx+w, cy+h, cz-d);  // Top Right Of The Texture and Quad
	glVertex3f(cx+w, cy+h, cz+d);  // Top Left Of The Texture and Quad
	glVertex3f(cx+w, cy-h, cz+d);  // Bottom Left Of The Texture and Quad

	// Left Face
	glVertex3f(cx-w, cy-h, cz-d);  // Bottom Left Of The Texture and Quad
	glVertex3f(cx-w, cy-h, cz+d);  // Bottom Right Of The Texture and Quad
	glVertex3f(cx-w, cy+h, cz+d);  // Top Right Of The Texture and Quad
	glVertex3f(cx-w, cy+h, cz-d);  // Top Left Of The Texture and Quad

	glEnd();
}
