#include "BinaryObjectReader.h"
#include "BinaryIterator.h"

BinaryObjectReader::BinaryObjectReader(char* filePath, void* obj, int objSize)
{
	data = NULL;
	FILE* fp = fopen(filePath, "rb");
	
	if(fp)
	{
		fseek(fp, 0, SEEK_END);
		int fileSize = ftell(fp);
		fseek(fp, 0, SEEK_SET);

		data = (unsigned char*)malloc(fileSize);
		fread(data, fileSize, 1, fp);
		fclose(fp);

		BinaryIterator buf(data, fileSize);
		int sizeOfObj = buf.ReadInt();

		if(sizeOfObj == objSize)
		{
			unsigned char* objMem = buf.GetBuf(sizeOfObj);
			
			while(buf.EOB() == false)
			{
				int size = buf.ReadInt();
				if(size > 0)
				{
					int fieldNum = buf.ReadInt();
					WriteUInt(objMem, fieldNum*4, (unsigned int)buf.GetBuf(size));
				}
				else if(size < 0)		//float double dimentional array
				{
					int totBytes = buf.ReadInt();
					int fieldNum = buf.ReadInt();
					int rows  = buf.ReadInt();
					int cols = buf.ReadInt();
					
					unsigned char* fieldMem = buf.GetBuf(totBytes);
					void** fieldMemAsDDP = NULL;

					// Here DDP = Double-Dimentional pointer

					if(size == -1)
						fieldMemAsDDP = (void**)GetFloatDDPointer((float*)fieldMem, rows, cols);
					else if(size == -2)
						fieldMemAsDDP = (void**)GetIntDDPointer((int*)fieldMem, rows, cols);
					else if(size == -3)
						fieldMemAsDDP = (void**)GetShortDDPointer((short*)fieldMem, rows, cols);
					else if(size == -4)
						fieldMemAsDDP = (void**)GetCharDDPointer((char*)fieldMem, rows, cols);

					free(fieldMem);

					WriteUInt(objMem, fieldNum*4, (unsigned int)fieldMemAsDDP);
				}
			}
			memcpy(obj, objMem, sizeOfObj);

			if(objMem)
			{
				free(objMem);
				objMem = NULL;
			}
		}
	}	
}

void BinaryObjectReader::WriteUInt(unsigned char* mem, int writeAtIndex, unsigned int val)
{
	mem[writeAtIndex + 3] = (val >> 24) & 255;
	mem[writeAtIndex + 2] = (val >> 16) & 255;
	mem[writeAtIndex + 1] = (val >> 8) & 255;
	mem[writeAtIndex + 0] = (val ) & 255;
}

int** BinaryObjectReader::GetIntDDPointer(int* intArr, int rows, int cols)
{
	int** ddPointer = (int**)malloc(4*rows);
	for(int i=0; i<rows; i++)
		ddPointer[i] = (int*)malloc(sizeof(int)*cols);

	for(int i=0; i<rows; i++)
		for(int j=0;j<cols;j++)
			ddPointer[i][j] = intArr[i*cols + j];

	return ddPointer;
}

float** BinaryObjectReader::GetFloatDDPointer(float* floatArr, int rows, int cols)
{
	float** ddPointer = (float**)malloc(4*rows);//any pointer mem size is 4 bytes
	for(int i=0; i<rows; i++)
		ddPointer[i] = (float*)malloc(sizeof(float)*cols);

	for(int i=0; i<rows; i++)
		for(int j=0;j<cols;j++)
			ddPointer[i][j] = floatArr[i*cols + j];

	return ddPointer;
}

short** BinaryObjectReader::GetShortDDPointer(short* shortArr, int rows, int cols)
{
	short** ddPointer = (short**)malloc(4*rows);
	for(int i=0; i<rows; i++)
		ddPointer[i] = (short*)malloc(sizeof(short)*cols);

	for(int i=0; i<rows; i++)
		for(int j=0;j<cols;j++)
			ddPointer[i][j] = shortArr[i*cols + j];

	return ddPointer;
}

char** BinaryObjectReader::GetCharDDPointer(char* charArr, int rows, int cols)
{
	char** ddPointer = (char**)malloc(4*rows);
	for(int i=0; i<rows; i++)
		ddPointer[i] = (char*)malloc(sizeof(char)*cols);

	for(int i=0; i<rows; i++)
		for(int j=0;j<cols;j++)
			ddPointer[i][j] = charArr[i*cols + j];

	return ddPointer;
}

BinaryObjectReader::~BinaryObjectReader()
{
	if(data)
	{
		free(data);
		data = NULL;
	}
}