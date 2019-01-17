#include "Model3D_Helper.h"
#include "Model3D.h"

Model3D_Helper::Model3D_Helper()
{
}

Model3D_Helper::~Model3D_Helper()
{
}

void Model3D_Helper::CreateBufferObects(FLMStruct* bin, unsigned int* vertexBufID, unsigned int* uvBufID, unsigned int* normalBufID, unsigned int* indicesBufID)
{
	unsigned int _vertexBufID = 0;
	unsigned int _uvBufID = 0;
	unsigned int _normalBufID = 0;
	unsigned int _indicesBufID = 0;

	if(bin->vertexBuf)
	{
		glGenBuffersARB(1, &_vertexBufID);
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, _vertexBufID);
		glBufferDataARB(GL_ARRAY_BUFFER_ARB, bin->numVertex*3*4, bin->vertexBuf, GL_STATIC_DRAW_ARB);
	}

	if(bin->uvBuf)
	{
		glGenBuffersARB(1, &_uvBufID);
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, _uvBufID);
		glBufferDataARB(GL_ARRAY_BUFFER_ARB, bin->numUVs*2*4, bin->uvBuf, GL_STATIC_DRAW_ARB);
	}

	if(bin->normalBuf)
	{
		glGenBuffersARB(1, &_normalBufID);
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, _normalBufID);
		glBufferDataARB(GL_ARRAY_BUFFER_ARB, bin->numNormals*3*4, bin->normalBuf, GL_STATIC_DRAW_ARB);
	}

	if(bin->indicesBuf)
	{
		glGenBuffersARB(1, &_indicesBufID);
		glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, _indicesBufID);

		if(bin->indicesFormat == GL_UNSIGNED_BYTE)
			glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB, bin->numIndices, bin->indicesBuf, GL_STATIC_DRAW_ARB);
		else if(bin->indicesFormat == GL_UNSIGNED_SHORT)
			glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB, bin->numIndices*2, bin->indicesBuf, GL_STATIC_DRAW_ARB);
		else if(bin->indicesFormat == GL_UNSIGNED_INT)
			glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB, bin->numIndices*4, bin->indicesBuf, GL_STATIC_DRAW_ARB);
	}

	glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, 0);

	*vertexBufID = _vertexBufID;
	*uvBufID = _uvBufID;
	*normalBufID = _normalBufID;
	*indicesBufID = _indicesBufID;
}

int Model3D_Helper::CreateCallList(FLMStruct* bin, unsigned int textureID)
{
	glDisable( GL_TEXTURE_2D );
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);

	if(textureID && bin->uvBuf)
	{
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, textureID);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer(2, GL_FLOAT, 0, bin->uvBuf);
	}

	if(bin->normalBuf)
	{
		glEnableClientState(GL_NORMAL_ARRAY);
		glNormalPointer(GL_FLOAT, 0, bin->normalBuf);
	}

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, bin->vertexBuf);

	int displayListID = glGenLists(1);
	glNewList(displayListID, GL_COMPILE);
	glDrawElements(GL_TRIANGLES, bin->numIndices, bin->indicesFormat, bin->indicesBuf);
	glEndList();

	return displayListID;
}