#ifndef Model3D_Helper_H
#define Model3D_Helper_H

#include "Def.h"
#include "IO/FLM/FLMStruct.h"

class Model3D_Helper
{
private:

public:
	Model3D_Helper();
	~Model3D_Helper();

	void CreateBufferObects(FLMStruct* bin, unsigned int* vertexBufID, unsigned int* uvBufID, unsigned int* normalBufID, unsigned int* indicesBufID);
	int CreateCallList(FLMStruct* bin, unsigned int textureID);
};

#endif
