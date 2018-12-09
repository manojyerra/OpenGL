#ifndef MAPVOID_H
#define MAPVOID_H

#include <string>
#include <vector>

using namespace std;

class MapVoid
{
private:
	vector<string> keyVec;
	vector<void*> valVec;

public:
	MapVoid();
	~MapVoid();

	int size();
	void add(string key, void* val);
	void set(string key, void* val);
	void* get(string key, bool* find);

	string getKeyByIndex(int index);
	void* getValByIndex(int index);

	int getKeyIndex(string key);
};

#endif