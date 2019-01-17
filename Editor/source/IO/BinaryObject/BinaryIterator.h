#ifndef BinaryIterator_H
#define BinaryIterator_H

class BinaryIterator
{
private:
	unsigned char* _buf;
	int _pos;
	int _bufSize;

public:
	BinaryIterator(unsigned char* buf, int bufSize);
	int ReadInt();
	unsigned char* GetBuf(int size);
	bool EOB();
	void Stop();
};

#endif