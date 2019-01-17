#ifndef BinaryObjectWriter_H
#define BinaryObjectWriter_H

#include "Def.h"
#include <vector>
using namespace std;

class Field
{
private:
	int _type;

public:
	int intVal;
	float floatVal;

	byte* bytePointer;
	short* shortPointer;
	int* intPointer;
	float* floatPointer;

	byte** byteDDPointer;
	short** shortDDPointer;
	int** intDDPointer;
	float** floatDDPointer;

	int arrLen;
	int rows;
	int cols;

	static const int INT_VAL = 1;
	static const int FLOAT_VAL = 2;

	static const int BYTE_POINTER = 3;
	static const int SHORT_POINTER = 4;
	static const int INT_POINTER = 5;
	static const int FLOAT_POINTER = 6;

	static const int BYTE_DDPOINTER = 7;
	static const int SHORT_DDPOINTER = 8;
	static const int INT_DDPOINTER = 9;
	static const int FLOAT_DDPOINTER = 10;

	Field(int type)
	{
		_type = type;
		arrLen = 0;
		rows = 0;
		cols = 0;
	}

	int GetType()
	{
		return _type;
	}

	void SetInt(int val)
	{
		intVal = val;
	}
	void SetFloat(float val)
	{
		floatVal = val;
	}

	void SetBytePointer(byte* val, int length)
	{
		bytePointer = val;
		arrLen = length;
	}

	void SetShortPointer(short* val, int length)
	{
		shortPointer = val;
		arrLen = length;
	}

	void SetIntPointer(int* val, int length)
	{
		intPointer = val;
		arrLen = length;
	}

	void SetFloatPointer(float* val, int length)
	{
		floatPointer = val;
		arrLen = length;
	}

	// Double-Demination pointer related....

	void SetByteDDPointer(byte** val, int numRows, int numCols)
	{
		byteDDPointer = val;
		rows = numRows;
		cols = numCols;
	}

	void SetShortDDPointer(short** val, int numRows, int numCols)
	{
		shortDDPointer = val;
		rows = numRows;
		cols = numCols;
	}

	void SetIntDDPointer(int** val, int numRows, int numCols)
	{
		intDDPointer = val;
		rows = numRows;
		cols = numCols;
	}

	void SetFloatDDPointer(float** val, int numRows, int numCols)
	{
		floatDDPointer = val;
		rows = numRows;
		cols = numCols;
	}
};

class BinaryObjectWriter
{
private:
	vector<Field> _fieldVec;

public:
	BinaryObjectWriter();
	~BinaryObjectWriter();

	void AddInt(int val);
	void AddFloat(float val);

	void AddBytePointer(byte* val, int length);
	void AddBytePointer(byte* val);
	void AddShortPointer(short* val, int length);
	void AddIntPointer(int* val, int length);
	void AddFloatPointer(float* val, int length);

	void AddByteDDPointer(byte** val, int rows, int cols);
	void AddShortDDPointer(short** val, int rows, int cols);
	void AddIntDDPointer(int** val, int rows, int cols);
	void AddFloatDDPointer(float** val, int rows, int cols);

	void Write(string filePath);
};

#endif