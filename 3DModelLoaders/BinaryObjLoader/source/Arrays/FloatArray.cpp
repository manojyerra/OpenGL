#include "FloatArray.h"

FloatArray::FloatArray()
{
	_capacity = 512;
	_size = 0;

	_arr = new float[_capacity];
}

FloatArray::FloatArray(unsigned int capacity)
{
	_capacity = capacity;
	_size = 0;

	_arr = new float[_capacity];
}

void FloatArray::ReCreateMem()
{
	ReCreateMem(2 * _capacity);
}

void FloatArray::ReCreateMem(unsigned int newCapacity)
{
	float* newArr = new float[newCapacity];
	memcpy(newArr, _arr, _capacity * 4);
	delete[] _arr;
	_arr = newArr;
	_capacity = newCapacity;
}

void FloatArray::push_back(float val)
{
	if (_size + 1 >= _capacity)
	{
		ReCreateMem();
	}

	_arr[_size] = val;
	_size++;
}

void FloatArray::push_back_2(const CVector3& vec)
{
	if (_size + 2 >= _capacity)
	{
		ReCreateMem();
	}

	_arr[_size + 0] = vec.x;
	_arr[_size + 1] = vec.y;

	_size += 2;
}

void FloatArray::push_back_3(const CVector3& vec)
{
	if (_size + 3 >= _capacity)
	{
		ReCreateMem();
	}

	_arr[_size + 0] = vec.x;
	_arr[_size + 1] = vec.y;
	_arr[_size + 2] = vec.z;

	_size += 3;
}

unsigned int FloatArray::size()
{
	return _size;
}

unsigned int FloatArray::capacity()
{
	return _capacity;
}

void FloatArray::addCapacity(int addCapacity)
{
	ReCreateMem(_capacity + addCapacity);
}

const float* FloatArray::getArray()
{
	return _arr;
}

FloatArray::~FloatArray()
{
	if(_arr)
	{
		delete[] _arr;
		_arr = NULL;
	}
}