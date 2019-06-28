#ifndef CharPtrArray_H
#define CharPtrArray_H
#pragma once

#include "DefinesAndIncludes.h"
#include "Vector3.h"

class CharPtrArray
{
private:
	unsigned int _capacity;
	int _size;
	unsigned int* _arr;
	
	void ReCreateMem();

public:
	CharPtrArray(unsigned int capacity);
	~CharPtrArray();

	void push_back(unsigned int val);
	
	unsigned int size();
	unsigned int capacity();

	const unsigned int* getArray();
};

#endif
