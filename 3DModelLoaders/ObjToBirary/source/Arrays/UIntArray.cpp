#include "CharPtrArray.h"

CharPtrArray::CharPtrArray(unsigned int capacity)
{
	_capacity = capacity;
	_size = 0;

	_arr = new unsigned int[_capacity];
}

void CharPtrArray::ReCreateMem()
{
	//int newCapacity = 3*_capacity;
	//float* newArr = new float[newCapacity];
	//memcpy(newArr,	_arr, _capacity);
	//delete[] _arr;
	//_arr = newArr;
	//_capacity = newCapacity;

	int newCapacity = 3 * _capacity;
	unsigned int* newArr = new unsigned int[newCapacity];
	memcpy(newArr, _arr, _capacity);
	delete[] _arr;
	_arr = newArr;
	_capacity = newCapacity;
}

void CharPtrArray::push_back(unsigned int val)
{
	if (_size + 1 >= _capacity)
		ReCreateMem();

	_arr[_size] = val;

	_size++;
}

unsigned int CharPtrArray::size()
{
	return _size;
}

unsigned int CharPtrArray::capacity()
{
	return _capacity;
}

const unsigned int* CharPtrArray::getArray()
{
	return (unsigned int*)_arr;
}

CharPtrArray::~CharPtrArray()
{
	if(_arr)
	{
		delete[] _arr;
		_arr = NULL;
	}
}