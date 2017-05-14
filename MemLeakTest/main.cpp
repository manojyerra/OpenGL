#ifdef _DEBUG
   #define DEBUG_CLIENTBLOCK   new( _CLIENT_BLOCK, __FILE__, __LINE__)
#else
   #define DEBUG_CLIENTBLOCK
#endif // _DEBUG

#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

#ifdef _DEBUG
#define new DEBUG_CLIENTBLOCK
#endif

#include "MemTrace.h"
#include "TestClass.h"

int main()
{
	TestClass* testClass = (TestClass*)newTrace( TestClass(10,20,30) );
	
	deleteTrace( testClass );

	//delete testClass;

	//TestClass* testClass = new TestClass();
	//delete testClass;

	//int* arr = new int[100];
	//delete arr;

	int* arr1 = (int*)newTrace(int[100]);

	//deleteTrace( arr1 );

	MemTrace::ReportMemoryLeaks();

	//_CrtDumpMemoryLeaks();
	return 0;
}


//int* pBuffer = new int[100];
//void* leak = malloc(222);
