#ifndef MainFrame_H
#define MainFrame_H

#include "SUI/SUIFrame.h"
#include "SUI/SUIActionListener.h"
#include "SUI/SUIRadioButton.h"
#include "SUI/SUICheckBox.h"
#include "SUI/SUIButton.h"
#include "SUI/SUILabel.h"
#include "SUI/SUIChoice.h"
#include "SUI/SUIBox.h"

#include "Cam.h"
#include "Floor.h"
#include "ModelsManager.h"
#include "ModelPropsFrame.h"

class MainFrame : public SUIActionListener
{
private:
	SUIFrame* _frame;
	
	SUIRadioButton* projRadio;
	SUIRadioButton* pivot;

	SUICheckBox* floorVisible;
	SUICheckBox* showAxis;
	SUICheckBox* showGrid;
	SUICheckBox* showGridLines;

	SUICheckBox* enablePhysics;
	SUICheckBox* showMarkedObjs;
	SUICheckBox* showUnmarkedObjs;
	SUICheckBox* showBoundShapes;
	SUICheckBox* showBoundBox;
	SUICheckBox* onBorder;
	
	Cam* _cam;
	Floor* _floor;
	ModelsManager* _modelsMgr;
	ModelPropsFrame* _modelProps;

	SUIBox* SetUpPivotBox();
	SUIBox* SetUpProjBox();
	SUIBox* SetUpCamViewBox();
	SUIBox* SetUpFloorBox();

public:
	MainFrame(int x, int y, int w, int h, Cam* cam, Floor* floor, ModelsManager* modelsMgr, ModelPropsFrame* modelProps);
	~MainFrame();

	void actionPerformed(SUIActionEvent e);

	void SetAsOrtho(bool ortho);
	void SelectOriginAsModel();
	void SetPhysicsOn(bool on);
	void SetBorderOn(bool on);

	bool IsOrtho();
	bool IsAxisEnable();
	bool IsGridEnable();
	bool IsGridLinesEnable();
	bool IsPhysicsEnabled();
	//bool IsShowingObjects();
	bool IsShowingBoundingShapes();
	bool IsShowingBorder();
	bool IsOriginAsPivot();
	bool IsSelectedObjectAsPivot();
};

#endif