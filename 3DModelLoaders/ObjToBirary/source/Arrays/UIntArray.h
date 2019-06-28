#ifndef UIntArray_H
#define UIntArray_H
#pragma once

#include "DefinesAndIncludes.h"
#include "Vector3.h"

class UIntArray
{
private:
	unsigned int _capacity;
	int _size;
	unsigned int* _arr;
	
	void ReCreateMem();

public:
	UIntArray(unsigned int capacity);
	~UIntArray();

	void push_back(unsigned int val);
	
	unsigned int size();
	unsigned int capacity();

	const unsigned int* getArray();
};

#endif
