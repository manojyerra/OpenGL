#include "MapVoid.h"

MapVoid::MapVoid()
{
}

MapVoid::~MapVoid()
{
}

void MapVoid::add(string key, void* val)
{
	keyVec.push_back(key);
	valVec.push_back(val);
}

void MapVoid::set(string key, void* val)
{
	int keyIndex = getKeyIndex(key);
	
	if(keyIndex != -1)
		valVec[keyIndex] = val;
}

void* MapVoid::get(string key, bool* find)
{
	int size = keyVec.size();
	int keyLen = key.length();

	for(int i=0;i<size;i++)
	{
		if(keyVec[i].length() == keyLen)
		{
			if(keyLen == 0)
				return valVec[i];

			if(keyVec[i][0] == key[0])
			{
				if(keyVec[i][keyLen-1] == key[keyLen-1])
				{
					if(keyVec[i].compare(key) == 0)
					{
						find[0] = true;
						return valVec[i];
					}
				}
			}
		}
	}

	find[0] = false;
	return NULL;
}

int MapVoid::getKeyIndex(string key)
{
	int size = keyVec.size();
	int keyLen = key.length();

	for(int i=0;i<size;i++)
	{
		if(keyVec[i].length() == keyLen)
		{
			if(keyLen == 0)
				return i;

			if(keyVec[i][0] == key[0])
			{
				if(keyVec[i][keyLen-1] == key[keyLen-1])
				{
					if(keyVec[i].compare(key) == 0)
					{
						return i;
					}
				}
			}
		}
	}

	return -1;
}

string MapVoid::getKeyByIndex(int index)
{
	return keyVec[index];
}

void* MapVoid::getValByIndex(int index)
{
	return valVec[index];
}

int MapVoid::size()
{
	return keyVec.size();
}
