#ifndef FloatArray_H
#define FloatArray_H
#pragma once

#include "DefinesAndIncludes.h"
#include "Vector3.h"

class FloatArray
{
private:
	unsigned int _capacity;
	int _size;
	float* _arr;

	void ReCreateMem();
	void ReCreateMem(unsigned int newCapacity);

public:
	FloatArray();
	FloatArray(unsigned int capacity);
	~FloatArray();

	void push_back(float val);
	void push_back_2(const CVector3& vec);
	void push_back_3(const CVector3& vec);
	
	unsigned int size();
	unsigned int capacity();

	void addCapacity(int addCapacity);

	const float* getArray();
};

#endif
