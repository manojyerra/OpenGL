#include "UtilFuncs.h"
#include "DefinesAndIncludes.h"

short UtilFuncs::ConvFloatToShort(float val)
{
	int intVal = val * 100000;

	int sign = (intVal < 0) ? -1 : 1;

	if(sign == -1)
		intVal = -intVal;

	int intVal4 = (intVal / 10) + ((intVal % 10) > 5 ? 1 : 0);

	return (short)intVal4*sign;
}

float UtilFuncs::ConvShortToFloat(short val)
{
	return (float)(val / 10000.0f);
}

void UtilFuncs::PrintFloatBits(float val)
{
	vector<unsigned char> bits = GetFloatBits(val);

	writeConsole("\n[float value %f in binary : ",val);

	for(int i=0; i<bits.size(); i++)
	{
		writeConsole("%d", (int)bits[i]);
	}

	writeConsole("]\n");
}

vector<unsigned char> UtilFuncs::GetFloatBits(float floatVal)
{
	int numBits = sizeof(float) * 8;

	vector<unsigned char> bitsVec;

	unsigned int intVal = *(unsigned int*)&floatVal;

	for(int i=0; i<numBits; i++)
	{
		unsigned char bitVal = (unsigned char)((intVal << (31-i) ) >> 31);

		bitsVec.push_back( bitVal );
	}

	return bitsVec;
}

void UtilFuncs::Normalize(float* nx, float* ny, float* nz)
{
	float x = nx[0];
	float y = ny[0];
	float z = nz[0];

	float len = sqrt(x*x + y*y + z*z);

	nx[0] = x / len;
	ny[0] = y / len;
	nz[0] = z / len;
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
