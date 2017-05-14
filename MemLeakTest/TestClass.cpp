#include "TestClass.h"

TestClass::TestClass(int a, int b, int c)
{
	mem = mallocTrace(1000);
	_a = (A*)newTrace(A(10, 20, NULL));
}

TestClass::~TestClass()
{
	mem = NULL;
}

A::A(int a, int b, void* aa)
{

}
