#ifndef ModelsManager_H
#define ModelsManager_H

#include "FLModel.h"
#include <vector>
#include <string>
using namespace std;

class ModelsManager
{
private:
	vector<FLModel*> _vec;

public:
	ModelsManager();
	~ModelsManager();

	FLModel* Add(string folderPath);
	FLModel* Add(string folderPath, float xPos, float yPos, float zPos);
	FLModel* Add(string folderPath, CVector3 pos);
	FLModel* Add(string folderPath, CVector3 pos, CVector3 rot);
	FLModel* Add(string folderPath, CVector3 pos, CVector3 rot, string rotOrder);
	FLModel* Add(string folderPath, float* mat);

	FLModel* Get(int index);
	int Size();
	void SetBoundingBoxEnabled(bool enable);
	void Draw();

	void DrawForSelection();
};

#endif