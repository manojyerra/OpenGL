#ifndef Editor_H
#define Editor_H

#include "Sui/SuiFrame.h"
#include "Sui/SuiButton.h"
#include "Sui/SuiBox.h"
#include "Sui/SuiActionListener.h"

#include "Model3D/Model3D.h"
#include "Cam.h"

#include "MainFrame.h"
#include "ModelPropsFrame.h"
#include "Pointers.h"
#include "Util/ModelBorder.h"
#include "Util/Rect.h"
#include "Sui/SuiFrame.h"
#include "EventsFrame.h"
#include <vector>
using namespace std;


class Editor : SuiActionListener
{
private:
	MainFrame* _mainFrame;
	ModelPropsFrame* _modelFrame;
	EventsFrame* _eventsFrame;
	Pointers* _pointers;

	vector<Model3D*> _modelVec;
	vector<Model3D*> _backUpModelVec;
	ModelBorder modelBorder;
	Rect rect;

	int _selIndex;
	bool _enablePhysics;
	bool _drawBoundingShapes;
	bool _drawObjects;
	bool _wireFrame;
	int _viewCounter;
	bool _drawOnlySelModel;

	void MoveByArrows();
	void SetPhysicsEnable(bool enable);
	void SelectModel();
	void DrawFloor();
	int GetBoundingShapeIndex(Model3D* model);
	int GetTriIndex(Model3D* model);
	void SaveObject();
	void DeleteObject();
	void DuplicateObject();
	void WireFrameObject();
	void FocusObject();

public:
	float _baseWindowW;
	float _baseWindowH;

	Editor(int windowWidth, int windowHeight);
	~Editor();

	void actionPerformed(SuiActionEvent suiActionEvent);
	void UpdateModelProps();
	void SetRotationMode(bool onRotationMode);
	bool IsSelected();
	Model3D* GetSelectedModel();
	void Update(float deltaTime);
	void UpdatePhysics(float deltaTime);
	void UpdateByKeys();
	void UpdateDrawRect();
	void Draw(float deltaTime);
};

#endif