#ifndef GLFBO_H
#define GLFBO_H

#include "Macros.h"

class GLFBO
{
private:
	unsigned int _fboID;
	unsigned int _texID;
	unsigned int _depthBufID;

	unsigned int _w;
	unsigned int _h;

	unsigned int GenerateEmptyTexture(int w, int h);
	void AttachTexToFBO(unsigned int texID);
	void BindFBO(int fboID);
	unsigned int CreateDepthBuffer(int w, int h);
	void AttachDepthBufferToFBO(unsigned int depthBufferID);

public:
	GLFBO(int w, int h);
	~GLFBO();

	void BindFBO();
	void UnBindFBO();
	unsigned int GetTextureID();
	bool isFBOCreated();

	unsigned int GetW();
	unsigned int GetH();
};

#endif
