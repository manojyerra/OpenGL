#ifndef FLMReader_H
#define FLMReader_H

#include "IO/BinaryObject/BinaryObjectWriter.h"
#include "FLMStruct.h"

class FLMReader
{
private:

public:
	FLMReader(string filePath, FLMStruct* FLMStruct);
	~FLMReader();
};

#endif