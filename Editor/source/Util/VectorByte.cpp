#include "VectorByte.h"
#include "UtilFuncs.h"

VectorByte::VectorByte()
{
	arr = new byte[16];
	_size = 0;
	_capacity = 16;
	_increment = 32;
}

VectorByte::~VectorByte()
{
	delete[] arr;
}

void VectorByte::Clear()
{
	arr = 0;
	_size = 0;
	_capacity = 0;
}

void VectorByte::add(VectorByte* vecByte)
{
	int size = vecByte->size();

	for(int i=0;i<size;i++)
		add(vecByte->arr[i]);
}
    
void VectorByte::add(byte b)
{
	if(_size >= _capacity)
	{
		int newCapacity = _capacity + _increment;
		byte* newArr = new byte[newCapacity];

		for(int i=0;i<_capacity;i++)
			newArr[i] = arr[i];

		delete[] arr;

		arr = newArr;
		_capacity = newCapacity;
	}

	arr[_size] = b;
	_size++;
}

void VectorByte::addShort(short val)
{
    add( (byte)((val) & 255)         );
    add( (byte)((val >> 8) & 255)    );
}

void VectorByte::addInt(int val)
{
    add( (byte)((val) & 255)         );
    add( (byte)((val >> 8) & 255)    );
    add( (byte)((val >> 16) & 255)   );
    add( (byte)((val >> 24) & 255)   );
}

void VectorByte::addFloat(float val)
{
	int intVal = UtilFuncs::floatToIntBits( val );

    add( (byte)((intVal) & 255)         );
    add( (byte)((intVal >> 8) & 255)    );
    add( (byte)((intVal >> 16) & 255)   );
    add( (byte)((intVal >> 24) & 255)   );
}

void VectorByte::addVertex(Vertex* vertex)
{
	addFloat(vertex->x);
	addFloat(vertex->y);
	addFloat(vertex->z);    	
}
	
//*****************  Pre Add *********************//
    
void VectorByte::preAddFloat(float val)
{
	preAddInt(UtilFuncs::floatToIntBits(val));
}

void VectorByte::preAddInt(int val)
{
    int newCapacity = _capacity + 4;
    byte* newArr = new byte[newCapacity];

    for(int i=0;i<_capacity;i++)
        newArr[i+4] = arr[i];
    
    newArr[0] = (byte)((val) & 255);
    newArr[1] = (byte)((val >> 8) & 255);
    newArr[2] = (byte)((val >> 16) & 255);
    newArr[3] = (byte)((val >> 24) & 255);        
    
	delete[] arr;

    arr = newArr;
    _capacity = newCapacity;
    _size += 4;
}

int VectorByte::size()
{
    return _size;
}
