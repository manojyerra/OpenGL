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
	//f %d/%d/%d %d/%d/%d %d/%d/%d
	string line = "f 6579/7509/6663 16348/7620/6774 16347/7617/6771";
	str = (char*)line.c_str();

	int i = 2;
	int s = 2;

	while (str[++i] != '/');
	str[i] = '\0';
	v1[0] = atoi( &str[s] );

	s = i + 1;
	while (str[++i] != '/');
	str[i] = '\0';
	t1[0] = atoi(&str[s]);

	s = i + 1;
	while (str[++i] != ' ');
	str[i] = '\0';
	n1[0] = atoi(&str[s]);

}