#include "GLFBO.h"
#include <assert.h>

GLFBO::GLFBO(int w, int h)
{
	_w = w;
	_h = h;

	glGenFramebuffersEXT(1, &_fboID);
	BindFBO(_fboID);
	_texID = GenerateEmptyTexture(_w, _h);
	AttachTexToFBO(_texID);
	_depthBufID = CreateDepthBuffer(_w, _h);
	AttachDepthBufferToFBO(_depthBufID);

	assert(isFBOCreated());
}

void GLFBO::BindFBO(int fboID)
{
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fboID);
}

void GLFBO::BindFBO()
{
	BindFBO(_fboID);
}

void GLFBO::UnBindFBO()
{
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}

unsigned int GLFBO::GetTextureID()
{
	return _texID;
}

unsigned int GLFBO::GenerateEmptyTexture(int w, int h)
{
	unsigned int texID;
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	return texID;
}

void GLFBO::AttachTexToFBO(unsigned int texID)
{
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, texID, 0);
}

unsigned int GLFBO::CreateDepthBuffer(int w, int h)
{
	unsigned int rbo;
	glGenRenderbuffersEXT(1, &rbo);
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, rbo);
	glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT24, w, h);
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);

	return rbo;
}

void GLFBO::AttachDepthBufferToFBO(unsigned int depthBufferID)
{
	glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, depthBufferID);
}

bool GLFBO::isFBOCreated()
{
	bool created = false;

	if (glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT) == GL_FRAMEBUFFER_COMPLETE_EXT)
		created = true;

	UnBindFBO();

	return created;
}

unsigned int GLFBO::GetW()
{
	return _w;
}

unsigned int GLFBO::GetH()
{
	return _h;
}

GLFBO::~GLFBO()
{
	//Delete resources
	glDeleteTextures(1, &_texID);
	glDeleteRenderbuffersEXT(1, &_depthBufID);
	//Bind 0, which means render to back buffer, as a result, fb is unbound
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	glDeleteFramebuffersEXT(1, &_fboID);
}
