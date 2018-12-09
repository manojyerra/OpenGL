#ifndef RandomColor_H
#define RandomColor_H

#include <vector>
using namespace std;

class RandomColor
{
private:
	static bool init;
	static vector<unsigned int> colorVec;
	int index;

public:
	RandomColor();
	unsigned int NextColor();
	void Reset();
	~RandomColor();
};

#endif
