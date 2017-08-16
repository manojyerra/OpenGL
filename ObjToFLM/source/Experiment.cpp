#include "Experiment.h"
#include "DefinesAndIncludes.h"

Experiment::Experiment()
{
}

Experiment::~Experiment()
{
}

short Experiment::ConvFloatToShort(float val)
{
	return (short)(val * 10000);
}

float Experiment::ConvShortToFloat(short val)
{
	return (float)(val / 10000.0f);
}

void Experiment::PrintFloatBits(float val)
{
	vector<unsigned char> bits = GetFloatBits(val);

	writeConsole("\n[float value %f in binary : ",val);

	for(int i=0; i<bits.size(); i++)
	{
		writeConsole("%d", (int)bits[i]);
	}

	writeConsole("]\n");
}

vector<unsigned char> Experiment::GetFloatBits(float floatVal)
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