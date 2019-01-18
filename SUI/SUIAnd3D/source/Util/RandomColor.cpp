#include "RandomColor.h"

bool RandomColor::init = false;
vector<unsigned int> RandomColor::colorVec;

RandomColor::RandomColor()
{
	if(init == false)
	{
		/*
		colorVec.push_back(0x9a4faaff);
		colorVec.push_back(0x587f0aff);
		colorVec.push_back(0x37ad77ff);
		colorVec.push_back(0xfe76e2ff);
		colorVec.push_back(0x06f53eff);
		colorVec.push_back(0xa5306dff);
		colorVec.push_back(0x70de33ff);
		colorVec.push_back(0xf8ec36ff);
		colorVec.push_back(0xda1903ff);
		colorVec.push_back(0x7d63e1ff);
		colorVec.push_back(0xb2bd31ff);
		colorVec.push_back(0xa7e2b6ff);
		colorVec.push_back(0xf7cc93ff);
		colorVec.push_back(0x85641cff);
		colorVec.push_back(0x65340eff);
		colorVec.push_back(0xcd0e69ff);
		colorVec.push_back(0x0ee4cbff);
		colorVec.push_back(0xae98b0ff);
		colorVec.push_back(0x4b337dff);
		colorVec.push_back(0x3ce37aff);
		colorVec.push_back(0xf77a9bff);
		colorVec.push_back(0x665e8cff);
		colorVec.push_back(0x6b01b7ff);
		colorVec.push_back(0x0baf55ff);
		colorVec.push_back(0x0cfe8bff);
		colorVec.push_back(0xbeaa01ff);
		colorVec.push_back(0x46bcbbff);
		colorVec.push_back(0x9b8c64ff);
		colorVec.push_back(0x16fb91ff);
		colorVec.push_back(0x4be9b2ff);
		colorVec.push_back(0x4c7ef8ff);
		colorVec.push_back(0xd6bb02ff);
		colorVec.push_back(0x305053ff);
		colorVec.push_back(0xd24ecdff);
		colorVec.push_back(0x01e9f9ff);
		colorVec.push_back(0xcad871ff);
		colorVec.push_back(0x506736ff);
		colorVec.push_back(0x8f413aff);
		colorVec.push_back(0x0bfb70ff);
		colorVec.push_back(0x941213ff);
		colorVec.push_back(0xfb3291ff);
		colorVec.push_back(0x157cb3ff);
		colorVec.push_back(0x0dd2e6ff);
		*/

		colorVec.push_back(0x9a4faaaa);
		colorVec.push_back(0x587f0aaa);
		colorVec.push_back(0x37ad77aa);
		colorVec.push_back(0xfe76e2aa);
		colorVec.push_back(0x06f53eaa);
		colorVec.push_back(0xa5306daa);
		colorVec.push_back(0x70de33aa);
		colorVec.push_back(0xf8ec36aa);
		colorVec.push_back(0xda1903aa);
		colorVec.push_back(0x7d63e1aa);
		colorVec.push_back(0xb2bd31aa);
		colorVec.push_back(0xa7e2b6aa);
		colorVec.push_back(0xf7cc93aa);
		colorVec.push_back(0x85641caa);
		colorVec.push_back(0x65340eaa);
		colorVec.push_back(0xcd0e69aa);
		colorVec.push_back(0x0ee4cbaa);
		colorVec.push_back(0xae98b0aa);
		colorVec.push_back(0x4b337daa);
		colorVec.push_back(0x3ce37aaa);
		colorVec.push_back(0xf77a9baa);
		colorVec.push_back(0x665e8caa);
		colorVec.push_back(0x6b01b7aa);
		colorVec.push_back(0x0baf55aa);
		colorVec.push_back(0x0cfe8baa);
		colorVec.push_back(0xbeaa01aa);
		colorVec.push_back(0x46bcbbaa);
		colorVec.push_back(0x9b8c64aa);
		colorVec.push_back(0x16fb91aa);
		colorVec.push_back(0x4be9b2aa);
		colorVec.push_back(0x4c7ef8aa);
		colorVec.push_back(0xd6bb02aa);
		colorVec.push_back(0x305053aa);
		colorVec.push_back(0xd24ecdaa);
		colorVec.push_back(0x01e9f9aa);
		colorVec.push_back(0xcad871aa);
		colorVec.push_back(0x506736aa);
		colorVec.push_back(0x8f413aaa);
		colorVec.push_back(0x0bfb70aa);
		colorVec.push_back(0x941213aa);
		colorVec.push_back(0xfb3291aa);
		colorVec.push_back(0x157cb3aa);
		colorVec.push_back(0x0dd2e6aa);

		init = true;
	}

	index = rand() % colorVec.size();
}

unsigned int RandomColor::NextColor()
{
	index++;

	if(index >= (int)colorVec.size())
		index = 0;

	return colorVec[index];
}

void RandomColor::Reset()
{
	index = 0;
}

RandomColor::~RandomColor()
{
}