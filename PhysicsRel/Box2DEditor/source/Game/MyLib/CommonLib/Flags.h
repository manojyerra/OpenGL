#ifndef FLAGS_H
#define FLAGS_H

#include "MapVoid.h"
#include <string>
#include <map>
using namespace std;

class Flags
{
private:
	static Flags* _ref;

	Flags();
	~Flags();

	//map<std::string, void*> _map;
	MapVoid _map;

public:

	static Flags* GetInstance();
	static void DeleteInstance();
	void Draw(std::string countryName, int x, int y, int w, int h);
};

#endif