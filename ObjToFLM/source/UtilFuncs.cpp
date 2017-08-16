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
