#ifndef MEM_TRACE_H
#define MEM_TRACE_H
#pragma once

#include <vector>
#include <string>
using namespace std;

#ifdef _DEBUG
	#define GLNew(param1, param2)	MemTrace::New(param1, param2, __FILE__, __LINE__)
	#define GLMalloc(param)			MemTrace::Malloc(param, __FILE__, __LINE__)
	#define GLDelete(param)			MemTrace::Delete(param, __FILE__, __LINE__)
	#define GLFree(param)			MemTrace::Free(param, __FILE__, __LINE__)
#else
	#define GLNew(param1, param2)	MemTrace::New(param1)
	#define GLMalloc(param)			MemTrace::Malloc(param)
	#define GLDelete(param)			MemTrace::Delete(param)
	#define GLFree(param)			MemTrace::Free(param)
#endif

class AddressInfo
{
public:
	void* address;
	int memSize;
	string fileName;
	int lineNum;
	
	AddressInfo()
	{
		address = 0;
		memSize = 0;
		fileName = "";
		lineNum = -1;
	}
};



class MemTrace
{
private:
public:
	static vector<AddressInfo> _addressInfoVec;
	static int TOTAL_MEMORY;

	static void Init();

	static void* New(void* address, size_t size, const char* file, unsigned int line);
	static void Delete(void *ptr, const char* file, unsigned int line);
	static void* Malloc(size_t size, const char* file, unsigned int line);
	static void Free(void *ptr, const char* file, unsigned int line);

	static void* New(void* address);
	static void Delete(void *ptr);
	static void* Malloc(size_t size);
	static void Free(void *ptr);

	static int GetSize(void* ptr);
	static int GetTotalMemory();
	static int ReportMemoryLeaks();
	static void PrintMemoryStatus();
};

#endif