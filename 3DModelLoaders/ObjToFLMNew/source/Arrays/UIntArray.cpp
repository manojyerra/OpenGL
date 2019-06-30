#include "UIntArray.h"

UIntArray::UIntArray(unsigned int capacity)
{
	_capacity = capacity;
	_size = 0;

	_arr = new unsigned int[_capacity];
}

void UIntArray::ReCreateMem()
{
	int newCapacity = 3 * _capacity;
	unsigned int* newArr = new unsigned int[newCapacity];
	memcpy(newArr, _arr, _capacity*4);
	delete[] _arr;
	_arr = newArr;
	_capacity = newCapacity;
}

void UIntArray::push_back(unsigned int val)
{
	if (_size + 1 >= _capacity)
		ReCreateMem();

	_arr[_size] = val;

	_size++;
}

unsigned int UIntArray::size()
{
	return _size;
}

unsigned int UIntArray::capacity()
{
	return _capacity;
}

const unsigned int* UIntArray::getArray()
{
	return (unsigned int*)_arr;
}

UIntArray::~UIntArray()
{
	if(_arr)
	{
		delete[] _arr;
		_arr = NULL;
	}
}