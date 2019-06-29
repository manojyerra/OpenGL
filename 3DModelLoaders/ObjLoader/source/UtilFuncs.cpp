#include "UtilFuncs.h"
#include "math.h"
#include <windows.h>
#include <gl/gl.h>

unsigned int UtilFuncs::GenerateGLTextureID(ImageBuffer* imgBuf)
{
	void* buffer = imgBuf->GetBuffer();
	
	if(buffer == NULL)
		return 0;

	int width = imgBuf->GetWidth();
	int height = imgBuf->GetHeight();
	int bytesPerPixel = imgBuf->GetBytesPerPixel();
	

	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	if(bytesPerPixel == 4)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
	else if(bytesPerPixel == 3)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, buffer);
	else if(bytesPerPixel == 1)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, width, height, 0, GL_RGB8, GL_UNSIGNED_BYTE, buffer);
	else
		return 0;

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	return textureID;
}

unsigned int UtilFuncs::GenerateGLTextureID(int width, int height, bool hasAlpha, unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	int bytesPerPixel = hasAlpha ? 4 : 3;
	int size = width * height * bytesPerPixel;
	unsigned char* buffer = (unsigned char*)malloc(size);

	for(int i=0; i<size; i+=bytesPerPixel)
	{
		buffer[i+0] = r;
		buffer[i+1] = g;
		buffer[i+2] = b;

		if(bytesPerPixel == 4)
			buffer[i+3] = a;
	}

	unsigned int textureID = 0;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	if(bytesPerPixel == 4)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
	else if(bytesPerPixel == 3)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, buffer);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	free(buffer);

	return textureID;
}


void UtilFuncs::scanFace_VTN(char* str,
	int* v1, int* t1, int* n1,
	int* v2, int* t2, int* n2,
	int* v3, int* t3, int* n3)
{
	//string line = "f 6579/7509/6663 16348/7620/6774 16347/7617/6771";
	//str = (char*)line.c_str();

	int i = 2;
	int s = i;

	while (str[++i] != '/');
	str[i] = '\0';
	v1[0] = atoi_naive(&str[s]);

	s = i + 1;
	while (str[++i] != '/');
	str[i] = '\0';
	t1[0] = atoi_naive(&str[s]);

	s = i + 1;
	while (str[++i] != ' ');
	str[i] = '\0';
	n1[0] = atoi_naive(&str[s]);

	//////////////////

	s = i + 1;
	while (str[++i] != '/');
	str[i] = '\0';
	v2[0] = atoi_naive(&str[s]);

	s = i + 1;
	while (str[++i] != '/');
	str[i] = '\0';
	t2[0] = atoi_naive(&str[s]);

	s = i + 1;
	while (str[++i] != ' ');
	str[i] = '\0';
	n2[0] = atoi_naive(&str[s]);

	//////////////////

	s = i + 1;
	while (str[++i] != '/');
	str[i] = '\0';
	v3[0] = atoi_naive(&str[s]);

	s = i + 1;
	while (str[++i] != '/');
	str[i] = '\0';
	t3[0] = atoi_naive(&str[s]);

	s = i + 1;
	do { i++; } while (str[i] != '\0' && str[i] != '\r');
	str[i] = '\0';
	n3[0] = atoi_naive(&str[s]);
}

void UtilFuncs::scanFace_VN(char* str,
	int* v1, int* n1,
	int* v2, int* n2,
	int* v3, int* n3)
{
	//string line = "f 6579//6663 16348//6774 16347//6771";
	//str = (char*)line.c_str();

	int i = 2;
	int s = i;

	while (str[++i] != '/');
	str[i] = '\0';
	v1[0] = atoi_naive(&str[s]);

	s = i + 2;
	while (str[++i] != ' ');
	str[i] = '\0';
	n1[0] = atoi_naive(&str[s]);

	//////////////////

	s = i + 1;
	while (str[++i] != '/');
	str[i] = '\0';
	v2[0] = atoi_naive(&str[s]);

	s = i + 2;
	while (str[++i] != ' ');
	str[i] = '\0';
	n2[0] = atoi_naive(&str[s]);

	//////////////////

	s = i + 1;
	while (str[++i] != '/');
	str[i] = '\0';
	v3[0] = atoi_naive(&str[s]);

	s = i + 2;
	do { i++; } while (str[i] != '\0' && str[i] != '\r');
	str[i] = '\0';
	n3[0] = atoi_naive(&str[s]);
}

void UtilFuncs::scanFace_VT(char* str,
	int* v1, int* t1,
	int* v2, int* t2,
	int* v3, int* t3)
{
	//string line = "f 6579/6663 16348/6774 16347/6771";
	//str = (char*)line.c_str();

	int i = 2;
	int s = i;

	while (str[++i] != '/');
	str[i] = '\0';
	v1[0] = atoi_naive(&str[s]);

	s = i + 1;
	while (str[++i] != ' ');
	str[i] = '\0';
	t1[0] = atoi_naive(&str[s]);

	//////////////////

	s = i + 1;
	while (str[++i] != '/');
	str[i] = '\0';
	v2[0] = atoi_naive(&str[s]);

	s = i + 1;
	while (str[++i] != ' ');
	str[i] = '\0';
	t2[0] = atoi_naive(&str[s]);

	//////////////////

	s = i + 1;
	while (str[++i] != '/');
	str[i] = '\0';
	v3[0] = atoi_naive(&str[s]);

	s = i + 1;
	do { i++; } while (str[i] != '\0' && str[i] != '\r');
	str[i] = '\0';
	t3[0] = atoi_naive(&str[s]);
}

void UtilFuncs::scanFace_V(char* str, int* x, int* y, int* z)
{
	//string line = "v -2.227210 3.102950 -6.477872";
	//str = (char*)line.c_str();

	int i = 2;
	int s = i;

	while (str[++i] != ' ');
	str[i] = '\0';
	x[0] = atoi_naive(&str[s]);

	s = i + 1;
	while (str[++i] != ' ');
	str[i] = '\0';
	y[0] = atoi_naive(&str[s]);

	s = i + 1;
	do { i++; } while (str[i] != '\0' && str[i] != '\r');
	str[i] = '\0';
	z[0] = atoi_naive(&str[s]);
}

void UtilFuncs::scan_vertex(char* str, float* x, float* y, float* z)
{
	//string line = "v -2.227210 3.102950 -6.477872";
	//str = (char*)line.c_str();

	int i = 2;
	int s = i;

	while (str[++i] != ' ');
	str[i] = '\0';
	x[0] = atof_naive(&str[s]);

	s = i + 1;
	while (str[++i] != ' ');
	str[i] = '\0';
	y[0] = atof_naive(&str[s]);

	s = i + 1;
	do { i++; } while (str[i] != '\0' && str[i] != '\r');
	str[i] = '\0';
	z[0] = atof_naive(&str[s]);
}

void UtilFuncs::scan_uv(char* str, float* x, float* y)
{
	//string line = "vt -2.227210 3.102950";
	//str = (char*)line.c_str();

	int i = 3;
	int s = i;

	while (str[++i] != ' ');
	str[i] = '\0';
	x[0] = atof_naive(&str[s]);

	s = i + 1;
	do { i++; } while (str[i] != '\0' && str[i] != '\r');
	str[i] = '\0';
	y[0] = atof_naive(&str[s]);
}

void UtilFuncs::scan_normal(char* str, float* x, float* y, float* z)
{
	//string line = "vn -2.227210 3.102950 -6.477872";
	//str = (char*)line.c_str();

	int i = 3;
	int s = i;

	while (str[++i] != ' ');
	str[i] = '\0';
	x[0] = atof_naive(&str[s]);

	s = i + 1;
	while (str[++i] != ' ');
	str[i] = '\0';
	y[0] = atof_naive(&str[s]);

	s = i + 1;
	do { i++; } while (str[i] != '\0' && str[i] != '\r');
	str[i] = '\0';
	z[0] = atof_naive(&str[s]);
}

double UtilFuncs::atof_naive(const char *p) {
	double r = 0.0;
	bool neg = false;
	if (*p == '-') {
		neg = true;
		++p;
	}
	while (*p >= '0' && *p <= '9') {
		r = (r*10.0) + (*p - '0');
		++p;
	}
	if (*p == '.') {
		double f = 0.0;
		int n = 0;
		++p;
		while (*p >= '0' && *p <= '9') {
			f = (f*10.0) + (*p - '0');
			++p;
			++n;
		}
		r += f / std::pow(10.0, n);
	}
	if (neg) {
		r = -r;
	}

	return r;
}

unsigned int UtilFuncs::atoi_naive(const char * p)
{
	unsigned int x = 0;
	while (*p) {
		x = x * 10 + (*p++ - '0');
	}
	return x;
}
