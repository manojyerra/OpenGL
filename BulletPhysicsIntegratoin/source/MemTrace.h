#ifndef MEM_TRACE_H
#define MEM_TRACE_H

#include <vector>
#include <string>
using namespace std;

#ifdef _DEBUG
	
	#define newTrace(param1)		MemTrace::New((void*)(new param1), __FILE__, __LINE__)
	#define deleteTrace(param1)		{delete param1; MemTrace::Delete(param1, __FILE__, __LINE__);}

	#define mallocTrace(param)		MemTrace::Malloc(param, __FILE__, __LINE__)
	#define freeTrace(param)		MemTrace::Free(param, __FILE__, __LINE__)

#else

	#define newTrace(param)					new param
	#define deleteTrace(param)				delete param

	#define mallocTrace(param)				malloc(param)
	#define freeTrace(param)				free(param)

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
	static void* New(void* address, const char* file, unsigned int line);
	static void Delete(int emptyParam, void* ptr, const char* file, unsigned int line);
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

	static int Empty(void);
};

#endif


//#define _new(param1, param2)		MemTrace::New(param1, param2, __FILE__, __LINE__)
//#define _delete(param1, param2)		MemTrace::Delete(param2, __FILE__, __LINE__)
