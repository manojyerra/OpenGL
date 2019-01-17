#include "FLMReader.h"
#include "IO/BinaryObject/BinaryObjectReader.h"

FLMReader::FLMReader(string filePath, FLMStruct* bin)
{
	BinaryObjectReader binObj((char*)filePath.c_str(), bin, sizeof(FLMStruct));
}

FLMReader::~FLMReader()
{
}