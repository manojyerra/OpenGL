#ifndef ALL_DEMOS_H
#define ALL_DEMOS_H

#include "Editor/Editor.h"

class AllDemos
{
private:
	Editor* _process;

public:
	AllDemos(int windowWidth, int windowHeight);
	~AllDemos();
	void Update(float deltaTime);
	void Draw(float deltaTime);

	int GetFileSize(string filePath);
	void FontUVGen();
};

#endif