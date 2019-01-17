#include "BinaryIterator.h"
#include "string.h"
#include "stdlib.h"

BinaryIterator::BinaryIterator(unsigned char* buf, int bufSize)
{
	_buf = buf;
	_pos = 0;
	_bufSize = bufSize;
}

int BinaryIterator::ReadInt()
{
	int returnVal = 0;

	if(_pos + 4 - 1< _bufSize)
	{
		returnVal = (int)((_buf[_pos+3]<<24) + (_buf[_pos+2]<<16) + (_buf[_pos+1]<<8) + _buf[_pos+0]);
		_pos += 4;
	}
	else
		Stop();

	return returnVal;
}

unsigned char* BinaryIterator::GetBuf(int size)
{
	unsigned char* returnVal = 0;

	if(_pos + size - 1 < _bufSize)
	{
		//returnVal = &(_buf[_pos]);
		returnVal = (unsigned char*)malloc(size);
		memcpy(returnVal, &(_buf[_pos]), size);
		_pos += size;
	}
	else
		Stop();

	return returnVal;
}

bool BinaryIterator::EOB()
{
	return (_pos >= _bufSize);
}

void BinaryIterator::Stop()
{
	int a = 0;
	int c = 1/a;
}