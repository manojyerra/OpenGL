#include "BinaryObjectWriter.h"
#include "Util/VectorByte.h"

BinaryObjectWriter::BinaryObjectWriter()
{
}

BinaryObjectWriter::~BinaryObjectWriter()
{
}

void BinaryObjectWriter::AddInt(int val)
{
	Field field(Field::INT_VAL);
	field.SetInt(val);
	_fieldVec.push_back(field);
}

void BinaryObjectWriter::AddFloat(float val)
{
	Field field(Field::FLOAT_VAL);
	field.SetFloat(val);
	_fieldVec.push_back(field);
}

void BinaryObjectWriter::AddBytePointer(byte* val, int length)
{
	Field field(Field::BYTE_POINTER);
	field.SetBytePointer(val, length);
	_fieldVec.push_back(field);
}

void BinaryObjectWriter::AddBytePointer(byte* val)
{
	if(val == NULL)
	{
		Field field(Field::BYTE_POINTER);
		field.SetBytePointer(NULL, 0);
		_fieldVec.push_back(field);
		return;
	}

	int index = 0;
	while(val[index] != '\0')
		index++;

	index++;

	Field field(Field::BYTE_POINTER);
	field.SetBytePointer(val, index);
	_fieldVec.push_back(field);
}

void BinaryObjectWriter::AddShortPointer(short* val, int length)
{
	Field field(Field::SHORT_POINTER);
	field.SetShortPointer(val, length);
	_fieldVec.push_back(field);
}

void BinaryObjectWriter::AddIntPointer(int* val, int length)
{
	Field field(Field::INT_POINTER);
	field.SetIntPointer(val, length);
	_fieldVec.push_back(field);
}

void BinaryObjectWriter::AddFloatPointer(float* val, int length)
{
	Field field(Field::FLOAT_POINTER);
	field.SetFloatPointer(val, length);
	_fieldVec.push_back(field);
}

void BinaryObjectWriter::AddByteDDPointer(byte** val, int rows, int cols)
{
	Field field(Field::BYTE_DDPOINTER);
	field.SetByteDDPointer(val, rows, cols);
	_fieldVec.push_back(field);
}

void BinaryObjectWriter::AddShortDDPointer(short** val, int rows, int cols)
{
	Field field(Field::SHORT_DDPOINTER);
	field.SetShortDDPointer(val, rows, cols);
	_fieldVec.push_back(field);
}

void BinaryObjectWriter::AddIntDDPointer(int** val, int rows, int cols)
{
	Field field(Field::INT_DDPOINTER);
	field.SetIntDDPointer(val, rows, cols);
	_fieldVec.push_back(field);
}

void BinaryObjectWriter::AddFloatDDPointer(float** val, int rows, int cols)
{
	Field field(Field::FLOAT_DDPOINTER);
	field.SetFloatDDPointer(val, rows, cols);
	_fieldVec.push_back(field);
}


void BinaryObjectWriter::Write(string filePath)
{
	FILE* file = fopen(filePath.c_str(), "wb");

	VectorByte vec;
	vec.addInt(_fieldVec.size()*4); // object instance variables memory in bytes.

	for(int i=0;i<_fieldVec.size(); i++)
	{
		if(_fieldVec[i].GetType() == Field::INT_VAL)
			vec.addInt(_fieldVec[i].intVal);
		else if(_fieldVec[i].GetType() == Field::FLOAT_VAL)
			vec.addFloat(_fieldVec[i].floatVal);
		else
			vec.addInt(0);
	}

	for(int i=0;i<_fieldVec.size(); i++)
	{
		Field f = _fieldVec[i];

		if(f.GetType() == Field::INT_POINTER && f.arrLen > 0)
		{
			vec.addInt(f.arrLen*4);
			vec.addInt(i);

			for(int j=0;j<f.arrLen;j++)
				vec.addInt(f.intPointer[j]);
		}
		else if(f.GetType() == Field::FLOAT_POINTER && f.arrLen > 0)
		{
			vec.addInt(f.arrLen*4);
			vec.addInt(i);

			for(int j=0;j<f.arrLen;j++)
				vec.addFloat(f.floatPointer[j]);
		}
		else if(f.GetType() == Field::SHORT_POINTER && f.arrLen > 0)
		{
			vec.addInt(f.arrLen*2);
			vec.addInt(i);

			for(int j=0;j<f.arrLen;j++)
				vec.addShort(f.shortPointer[j]);
		}
		else if(f.GetType() == Field::BYTE_POINTER && f.arrLen > 0)
		{
			vec.addInt(f.arrLen);
			vec.addInt(i);

			for(int j=0;j<f.arrLen;j++)
				vec.add(f.bytePointer[j]);
		}
		else if(f.GetType() == Field::FLOAT_DDPOINTER && f.rows > 0 && f.cols > 0)
		{
			int dataTypeID = -1;
			int dataTypeSize = 4;
			int totLen = f.rows * f.cols;

			vec.addInt(dataTypeID);
			vec.addInt(totLen*dataTypeSize);
			vec.addInt(i);
			vec.addInt(f.rows);
			vec.addInt(f.cols);

			for(int i=0;i<f.rows;i++)
				for(int j=0;j<f.cols;j++)
					vec.addFloat(f.floatDDPointer[i][j]);
		}
		else if(f.GetType() == Field::INT_DDPOINTER && f.rows > 0 && f.cols > 0)
		{
			int dataTypeID = -2;
			int dataTypeSize = 4;
			int totLen = f.rows * f.cols;

			vec.addInt(dataTypeID);
			vec.addInt(totLen*dataTypeSize);
			vec.addInt(i);
			vec.addInt(f.rows);
			vec.addInt(f.cols);

			for(int i=0;i<f.rows;i++)
				for(int j=0;j<f.cols;j++)
					vec.addInt(f.intDDPointer[i][j]);
		}
		else if(f.GetType() == Field::SHORT_DDPOINTER && f.rows > 0 && f.cols > 0)
		{
			int dataTypeID = -3;
			int dataTypeSize = 2;
			int totLen = f.rows * f.cols;

			vec.addInt(dataTypeID);
			vec.addInt(totLen*dataTypeSize);
			vec.addInt(i);
			vec.addInt(f.rows);
			vec.addInt(f.cols);

			for(int i=0;i<f.rows;i++)
				for(int j=0;j<f.cols;j++)
					vec.addShort(f.shortDDPointer[i][j]);
		}
		else if(f.GetType() == Field::BYTE_DDPOINTER && f.rows > 0 && f.cols > 0)
		{
			int dataTypeID = -4;
			int dataTypeSize = 1;
			int totLen = f.rows * f.cols;

			vec.addInt(dataTypeID);
			vec.addInt(totLen*dataTypeSize);
			vec.addInt(i);
			vec.addInt(f.rows);
			vec.addInt(f.cols);

			for(int i=0;i<f.rows;i++)
				for(int j=0;j<f.cols;j++)
					vec.add(f.byteDDPointer[i][j]);
		}
	}

	fwrite(vec.arr, 1, vec.size(), file);
	fclose(file);
}