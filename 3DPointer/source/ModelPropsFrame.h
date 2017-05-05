#ifndef ModelPropsFrame_H
#define ModelPropsFrame_H

#include "SUI/SUIFrame.h"
#include "SUI/SUIButton.h"
#include "SUI/SUICheckBox.h"
#include "SUI/SUIChoice.h"
#include "SUI/SUIActionListener.h"
#include "SUI/SUIBox.h"
#include "ModelsManager.h"

class ModelPropsFrame : SUIActionListener
{
private:
	SUIFrame* _frame;
	ModelsManager* _modelsMgr;
		
	SUICheckBox* _texureChkBox;
	SUICheckBox* _boundingBoxChkBox;
	SUICheckBox* _bounding2DRectChkBox;
	SUICheckBox* _showBoundingShapesChkBox;
	SUICheckBox* _wireFrameLinesChkBox;
	SUICheckBox* _wireFramePointsChkBox;
	SUICheckBox* _showModelChkBox;
	SUICheckBox* _showLocalAxis;
	SUICheckBox* _markObject;

	//Begin : Light box related ui

	SUISlider* _ambientR;
	SUISlider* _ambientG;
	SUISlider* _ambientB;

	SUISlider* _diffuseR;
	SUISlider* _diffuseG;
	SUISlider* _diffuseB;

	SUISlider* _specularR;
	SUISlider* _specularG;
	SUISlider* _specularB;

	SUICheckBox* _lightingChkBox;
	SUISlider* _shininessSlider;

	//End : Light box related ui
	
	SUIBox* CreateLightingUI();
	bool CheckLightBoxUI(SUIComponent* com, FLModel* selModel);

public:
	ModelPropsFrame(int x, int y, int w, int h, ModelsManager* modelsMgr);
	~ModelPropsFrame();

	void actionPerformed(SUIActionEvent SUIActionEvent);

	void SetUIValuesFromModel(FLModel* model);
	void ShowBoundingShapes(bool show);
};

#endif