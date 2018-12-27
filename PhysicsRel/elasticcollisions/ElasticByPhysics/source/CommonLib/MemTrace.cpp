#include "MemTrace.h"
#include "Ref.h"

vector<AddressInfo> MemTrace::_addressInfoVec;
int MemTrace::TOTAL_MEMORY = 0;


void MemTrace::Init()
{
	TOTAL_MEMORY = 0;
	_addressInfoVec.clear();
}


void* MemTrace::New(void* ptr) { return ptr; }
void* MemTrace::Malloc(size_t size) { return malloc(size); }
void MemTrace::Delete(void* ptr) { delete ptr; }
void MemTrace::Free(void* ptr)	{ free(ptr); }


void* MemTrace::New(void* address, size_t size, const char* file, unsigned int line)
{
	AddressInfo addressInfo;
	addressInfo.address = address;
	addressInfo.memSize = size;
	addressInfo.fileName = file;
	addressInfo.lineNum = (int)line;
	_addressInfoVec.push_back(addressInfo);

	TOTAL_MEMORY += size;

	return address;
}

void* MemTrace::Malloc(size_t size, const char* file, unsigned int line)
{
	void* address = malloc(size);

	AddressInfo addressInfo;
	addressInfo.address = address;
	addressInfo.memSize = size;
	addressInfo.fileName = file;
	addressInfo.lineNum = (int)line;
	_addressInfoVec.push_back(addressInfo);

	TOTAL_MEMORY += size;

	return address;
}



void MemTrace::Delete(void *ptr, const char* file, unsigned int line)
{
	bool find = false;
	for(int i=0;i<_addressInfoVec.size();i++)
	{
		if(_addressInfoVec[i].address == ptr)
		{
			delete ptr;
			TOTAL_MEMORY -= _addressInfoVec[i].memSize;
			_addressInfoVec.erase(_addressInfoVec.begin()+i);
			return;
		}
	}

	if(find == false)
	{
		char msg[512];

		if(ptr == NULL)
		{
			sprintf(msg, "Deleting a pointer from %s file %d line, which is NULL", file, line);
			//s3eDebugAssertShow(S3E_MESSAGE_CONTINUE_STOP, msg);
			int a=1,b=0,c=0; c = a/b;
		}
		else
		{
			sprintf(msg, "Deleting a pointer from %s file %d line, which is not yet initialized", file, line);
			//s3eDebugAssertShow(S3E_MESSAGE_CONTINUE_STOP, msg);
			int a=1,b=0,c=0; c = a/b;
		}
	}
}


void MemTrace::Free(void *ptr, const char* file, unsigned int line)
{
	bool find = false;
	for(int i=0;i<_addressInfoVec.size();i++)
	{
		if(_addressInfoVec[i].address == ptr)
		{
			free(ptr);
			TOTAL_MEMORY -= _addressInfoVec[i].memSize;
			_addressInfoVec.erase(_addressInfoVec.begin()+i);
			return;
		}
	}

	if(find == false)
	{
		char msg[512];

		if(ptr == NULL)
		{
			sprintf(msg, "Deleting a pointer from %s file %d line, which is NULL", file, line);
			//s3eDebugAssertShow(S3E_MESSAGE_CONTINUE_STOP, msg);
			int a=1,b=0,c=0; c = a/b;
		}
		else
		{
			sprintf(msg, "Deleting a pointer from %s file %d line, which is not yet initialized", file, line);
			//s3eDebugAssertShow(S3E_MESSAGE_CONTINUE_STOP, msg);
			int a=1,b=0,c=0; c = a/b;
		}
	}
}


int MemTrace::GetSize(void* ptr)
{
	for(int i=0;i<_addressInfoVec.size();i++)
		if(_addressInfoVec[i].address == ptr)
			return _addressInfoVec[i].memSize;

	return 0;
}


int MemTrace::GetTotalMemory()
{
	return TOTAL_MEMORY;
}

int MemTrace::ReportMemoryLeaks()
{
	char msg[512];

	sprintf(msg, "MEMORY LEAKS %d bytes or %.3f KB or %.3f MB , NUM OF REFERENCES %d",TOTAL_MEMORY, TOTAL_MEMORY*1.0f/1024.0f,
		TOTAL_MEMORY*1.0f/(1024.0f * 1024.0f),	_addressInfoVec.size());

	printf("\n\n\n\n");
	printf(msg);
	printf("\n\n");

	if(TOTAL_MEMORY > 0)
	{
		//s3eDebugAssertShow(S3E_MESSAGE_CONTINUE, msg);
	}

	bool showDialog = true;

	for(int i=0;i<_addressInfoVec.size();i++)
	{
		int lineNum = _addressInfoVec[i].lineNum;

		ImageInfo* imageInfoRef = dynamic_cast<ImageInfo*>((ImageInfo*)_addressInfoVec[i].address);

		if(imageInfoRef)
			sprintf(msg, "Not deleted ImageInfo object at FileName : %s, Line num = %d, ImageInfoObjectNum = %d", _addressInfoVec[i].fileName.c_str(), lineNum, imageInfoRef->objectNum);
		else
			sprintf(msg, "Not deleted memory at FileName : %s, Line num = %d", _addressInfoVec[i].fileName.c_str(), lineNum);

		if(showDialog)
		{
			//if(S3E_ERROR_SHOW_IGNORE == s3eDebugAssertShow(S3E_MESSAGE_CONTINUE_STOP_IGNORE, msg))
			//	showDialog = false;
		}

		printf("%s",msg);
	}

	_addressInfoVec.clear();

	printf("\n\n\n TOTAL MEMORY LEAKS %d bytes \n\n\n ", _addressInfoVec.size());

	return TOTAL_MEMORY;
}

void MemTrace::PrintMemoryStatus()
{
	printf("\nMEMORY allocated %d bytes or %.3f KB or %.3f MB , NUM OF REFERENCES %d",TOTAL_MEMORY, TOTAL_MEMORY*1.0f/1024.0f,
		TOTAL_MEMORY*1.0f/(1024.0f * 1024.0f),	_addressInfoVec.size());
}