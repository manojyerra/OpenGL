#include "IO/Obj/ObjReader.h"
#include "IO/BinaryObject/BinaryObjectWriter.h"
#include "FLMStruct.h"

class FLMWriter
{
private:
	FLMStruct* _bin;

public:
	FLMWriter(FLMStruct* FLMStruct);
	~FLMWriter();
	void Write(string filePath);
};