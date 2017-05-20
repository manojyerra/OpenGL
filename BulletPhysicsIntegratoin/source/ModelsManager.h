#ifndef ModelsManager_H
#define ModelsManager_H

#include "Model3D/FLModel.h"
#include <vector>
#include <string>
using namespace std;

class ModelsManager
{
private:
	vector<FLModel*> _vec;
	FLModel* _selModel;
	bool _showOnlySelObject;

public:
	ModelsManager();
	~ModelsManager();

	FLModel* Add(string folderPath);
	FLModel* Add(string folderPath, float xPos, float yPos, float zPos);
	FLModel* Add(string folderPath, CVector3 pos);
	FLModel* Add(string folderPath, CVector3 pos, CVector3 rot);
	FLModel* Add(string folderPath, CVector3 pos, CVector3 rot, string rotOrder);
	FLModel* Add(string folderPath, float* mat);

	unsigned int Size();
	FLModel* Get(unsigned int index);
	FLModel* GetSelectedModel();
	int GetModelIndexByMousePos(float x, float y);
	void SetSelectedModelIndex(int index);

	void ShowMarkedObjects(bool show);
	void ShowUnmarkedObjects(bool show);
	void MarkAllObjects();
	void UnmarkAllObjects();

	void ShowBoundingShapes(bool show);
	void SetBoundingBoxEnabled(bool enable);
	void SetAsPhysicsObjects(bool val);

	void ShowOnlySelectedObject(bool show);
	bool IsShowingOnlySelectedObject();

	void DrawForSelection();
	void Draw();
};

#endif
