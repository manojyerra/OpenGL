#include "FLModel.h"
#include "CFileReader.h"
#include "ImageBuffer.h"
#include "UtilFuncs.h"

FLModel::FLModel(string folderPath)
{
	Init(folderPath, FLM_WITH_INDEX_SIZE);
}

FLModel::FLModel(string folderPath, int type)
{
	Init(folderPath, FLM_WITHOUT_INDEX_SIZE);
}

void FLModel::Init(string folderPath, int type)
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

		int fileLen = fileReader->GetLength();

		if(fileLen == _numVertex * 12)
		{
			_normalsPointer = (unsigned char*)malloc( fileLen );
			memcpy(_normalsPointer, fileReader->GetData(), fileLen);
			delete fileReader;
		}
		else
		{
			short* shortArr = (short*)malloc( fileLen );
			memcpy(shortArr, fileReader->GetData(), fileLen);
			delete fileReader;
			
			_normalsPointer = (unsigned char*)malloc( fileLen*2 );

			float* tempArr = (float*)_normalsPointer;

			for(int i=0; i<fileLen/2; i++)
			{
				tempArr[i] = (float)(shortArr[i] / 10000.0f);
			}
		}
	}

	if(CFileReader::IsFileExists(folderPath+"/index.buf"))
	{
		CFileReader* fileReader = new CFileReader(folderPath+"/index.buf", "rb");
		int fileLen = fileReader->GetLength();
		_indicesPointer = (unsigned char*)malloc(fileLen);
		memcpy(_indicesPointer, fileReader->GetData(), fileLen);
		delete fileReader;

		if (type == FLM_WITH_INDEX_SIZE)
		{
			int val = _indicesPointer[fileLen-4];

			if (val == 1)		_indicesType = GL_UNSIGNED_BYTE;
			else if (val == 2)	_indicesType = GL_UNSIGNED_SHORT;
			else if (val == 4)	_indicesType = GL_UNSIGNED_INT;

			_numIndices = (fileLen-4) / val;
		}
		else if (type == FLM_WITHOUT_INDEX_SIZE)
		{
			int val = 4;

			_indicesType = GL_UNSIGNED_INT;
			_numIndices = fileLen / val;
		}
		else
		{
			throw "Invaid FLM type";
		}
	}

	ImageBuffer* imgBuf = new ImageBuffer(folderPath+"/texture.png");
	_textureID = UtilFuncs::GenerateGLTextureID(imgBuf);
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
}
