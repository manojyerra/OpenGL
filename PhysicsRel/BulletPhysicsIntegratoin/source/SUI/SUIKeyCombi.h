#ifndef SUIKeyCombi_H
#define SUIKeyCombi_H

#include "SUIInput.h"
#include <vector>
using namespace std;

class SUIKeyCombi
{
private:
	vector<int> keys;

public:
	SUIKeyCombi();
	SUIKeyCombi(int key1);
	SUIKeyCombi(int key1, int key2);
	SUIKeyCombi(int key1, int key2, int key3);
	SUIKeyCombi(int key1, int key2, int key3, int key4);

	bool IsEventFired();
};

#endif
