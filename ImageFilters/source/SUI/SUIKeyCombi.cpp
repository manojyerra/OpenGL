#include "SUIKeyCombi.h"

SUIKeyCombi::SUIKeyCombi()
{
	keys.clear();
}

SUIKeyCombi::SUIKeyCombi(int key1)
{
	keys.clear();
	keys.push_back(key1);
}

SUIKeyCombi::SUIKeyCombi(int key1, int key2)
{
	keys.clear();
	keys.push_back(key1);
	keys.push_back(key2);
}

SUIKeyCombi::SUIKeyCombi(int key1, int key2, int key3)
{
	keys.clear();
	keys.push_back(key1);
	keys.push_back(key2);
	keys.push_back(key3);
}

SUIKeyCombi::SUIKeyCombi(int key1, int key2, int key3, int key4)
{
	keys.clear();
	keys.push_back(key1);
	keys.push_back(key2);
	keys.push_back(key3);
	keys.push_back(key4);
}

bool SUIKeyCombi::IsEventFired()
{
	if(keys.size() == 0)
		return false;

	for(int i=0; i<keys.size(); i++)
	{
		if(i == keys.size()-1)
		{
			if(!SUIInput::IsKeyReleased(keys[i]))
				return false;
		}
		else
		{
			if(!SUIInput::IsKeyPressed(keys[i]))
				return false;
		}
	}

	return true;
}
