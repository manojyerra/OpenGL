#ifndef TestClass_H
#define TestClass_H

#include <stdlib.h>
#include "MemTrace.h"

//#ifdef _DEBUG
//   #define DEBUG_CLIENTBLOCK   new( _CLIENT_BLOCK, __FILE__, __LINE__)
//#else
//   #define DEBUG_CLIENTBLOCK
//#endif // _DEBUG
//
//#ifdef _DEBUG
//#define _CRTDBG_MAP_ALLOC
//#include <stdlib.h>
//#include <crtdbg.h>
//#endif
//
//#ifdef _DEBUG
//#define new DEBUG_CLIENTBLOCK
//#endif

class A
{
public:
	A(int a, int b, void* aa);
};

class TestClass
{
private:
	int a;
	int b;
	float x;
	float y;

	void* mem;
	A* _a;

public:
	TestClass(int a, int b, int c);
	~TestClass();
};

#endif