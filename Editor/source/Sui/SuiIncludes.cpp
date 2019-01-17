#include "SuiIncludes.h"

//*************************** GL_ARB_vertex_buffer_object ******************************

PFNGLGENBUFFERSARBPROC __glGenBuffersARB = NULL;
PFNGLBINDBUFFERARBPROC __glBindBufferARB = NULL;
PFNGLBUFFERDATAARBPROC __glBufferDataARB = NULL;
PFNGLDELETEBUFFERSARBPROC __glDeleteBuffersARB = NULL;

bool MakeVBOExtensions()
{
	__glGenBuffersARB = (PFNGLGENBUFFERSARBPROC) wglGetProcAddress("glGenBuffersARB");
	__glBindBufferARB = (PFNGLBINDBUFFERARBPROC) wglGetProcAddress("glBindBufferARB");
	__glBufferDataARB = (PFNGLBUFFERDATAARBPROC) wglGetProcAddress("glBufferDataARB");
	__glDeleteBuffersARB = (PFNGLDELETEBUFFERSARBPROC) wglGetProcAddress("glDeleteBuffersARB");

	return (__glGenBuffersARB && __glBindBufferARB && __glBufferDataARB && __glDeleteBuffersARB);
}

void glGenBuffersARB(GLsizei n, GLuint *buffers)
{
	__glGenBuffersARB(n, buffers);
}

void glBindBufferARB(GLenum target, GLuint buffer)
{
	__glBindBufferARB(target, buffer);
}

void glBufferDataARB(GLenum target, int size, const GLvoid *data, GLenum usage)
{
	__glBufferDataARB(target, size, data, usage);
}

void glDeleteBuffersARB(GLsizei n, const GLuint *buffers)
{
	__glDeleteBuffersARB(n, buffers);
}


//*************************** GL_EXT_texture_compression_s3tc ****************************

PFNGLCOMPRESSEDTEXIMAGE2DARBPROC __glCompressedTexImage2DARB = NULL;

bool MakeTextureCompressionExtensions()
{
	__glCompressedTexImage2DARB = (PFNGLCOMPRESSEDTEXIMAGE2DARBPROC)wglGetProcAddress("glCompressedTexImage2DARB");
	return (__glCompressedTexImage2DARB != 0);
}

void glCompressedTexImage2DARB(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const GLvoid *data)
{
	__glCompressedTexImage2DARB(target, level, internalformat, width, height, border, imageSize, data);
}


//*************************** glMultiTexCoord2fARB ****************************

PFNGLMULTITEXCOORD2FARBPROC	__glMultiTexCoord2fARB = NULL;
PFNGLACTIVETEXTUREARBPROC __glActiveTextureARB = NULL;

bool MakeMultiTexCoordExtensions()
{
	__glActiveTextureARB	= (PFNGLACTIVETEXTUREARBPROC)		wglGetProcAddress("glActiveTextureARB");
	__glMultiTexCoord2fARB	= (PFNGLMULTITEXCOORD2FARBPROC)		wglGetProcAddress("glMultiTexCoord2fARB");

	return (__glActiveTextureARB != 0 && __glMultiTexCoord2fARB != 0);
}

void glActiveTextureARB(GLenum target)
{
	__glActiveTextureARB(target);
}

void glMultiTexCoord2fARB(GLenum target, GLfloat s, GLfloat t)
{
	__glMultiTexCoord2fARB(target, s, t);
}
