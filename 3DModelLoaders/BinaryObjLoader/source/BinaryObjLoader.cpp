#include "BinaryObjLoader.h"
#include "CFileReader.h"
#include "ImageBuffer.h"
#include "UtilFuncs.h"
#include "Vector3.h"

BinaryObjLoader::BinaryObjLoader(string folderPath)
{
	_verticesPointer = NULL;
	_normalsPointer = NULL;
	_uvsPointer = NULL;

	ReadBinaryObjFile(folderPath);

	ImageBuffer* imgBuf = new ImageBuffer(folderPath +"/texture.png");
	_textureID = UtilFuncs::GenerateGLTextureID(imgBuf);
}

void BinaryObjLoader::ReadBinaryObjFile(string folderPath)
{
	if (CFileReader::IsFileExists(folderPath + "/vertex.buf"))
	{
		CFileReader* fileReader = new CFileReader(folderPath + "/vertex.buf", "rb");
		_verticesPointer = (unsigned char*)malloc(fileReader->GetLength());
		memcpy(_verticesPointer, fileReader->GetData(), fileReader->GetLength());
		_numVertex = fileReader->GetLength() / 12;
		delete fileReader;
	}

	if (CFileReader::IsFileExists(folderPath + "/normal.buf"))
	{
		CFileReader* fileReader = new CFileReader(folderPath + "/normal.buf", "rb");
		_normalsPointer = (unsigned char*)malloc(fileReader->GetLength());
		memcpy(_normalsPointer, fileReader->GetData(), fileReader->GetLength());
		delete fileReader;
	}

	if (CFileReader::IsFileExists(folderPath + "/uv.buf"))
	{
		CFileReader* fileReader = new CFileReader(folderPath + "/uv.buf", "rb");
		_uvsPointer = (unsigned char*)malloc(fileReader->GetLength());
		memcpy(_uvsPointer, fileReader->GetData(), fileReader->GetLength());
		delete fileReader;
	}

}

void BinaryObjLoader::Draw()
{
	glEnableClientState(GL_VERTEX_ARRAY);

	if(_textureID > 0 && _uvsPointer )
	{
		glEnable(GL_TEXTURE_2D);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer(2, GL_FLOAT, 0, _uvsPointer);
	}
	
	if(_normalsPointer)
	{
		glEnableClientState(GL_NORMAL_ARRAY);
		glNormalPointer(GL_FLOAT, 0, _normalsPointer);
	}

	glVertexPointer(3, GL_FLOAT, 0, _verticesPointer);
	glDrawArrays(GL_TRIANGLES, 0, _numVertex);

	if(_normalsPointer)
	{
		glDisableClientState(GL_NORMAL_ARRAY);
	}

	if(_textureID > 0 && _uvsPointer)
	{
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisable(GL_TEXTURE_2D);
	}

	glDisableClientState(GL_VERTEX_ARRAY);
}
