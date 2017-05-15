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


	//Begin : Translation box related ui

	SUILabel* _transXLabel;
	SUILabel* _transYLabel;
	SUILabel* _transZLabel;

	SUIButton* _transXBtn;
	SUIButton* _transYBtn;
	SUIButton* _transZBtn;

	//End : Translation box related ui


	//Begin : Rotation box related ui

	SUILabel* _rotXLabel;
	SUILabel* _rotYLabel;
	SUILabel* _rotZLabel;

	SUIButton* _rx1;
	SUIButton* _ry1;
	SUIButton* _rz1;

	SUIButton* _rx2;
	SUIButton* _ry2;
	SUIButton* _rz2;

	SUIButton* _lx1;
	SUIButton* _ly1;
	SUIButton* _lz1;

	SUIButton* _lx2;
	SUIButton* _ly2;
	SUIButton* _lz2;

	SUIButton* _rotZeroBtn;
	SUIButton* _copyRotBtn;
	SUIButton* _pasteRotBtn;
	//End : Rotation box related ui


	//Begin : ClipBoard values

	static const int NONE = 0;
	static const int COPY = 0;
	static const int TRANS = 0;

	int _copyType;
	CVector3 _rotCopy;

	//End : ClipBoard values


	SUIBox* SetUpLightingBox();
	SUIBox* SetUpTransBox();
	SUIBox* SetUpRotationBox();

	bool CheckLightBoxUI(SUIComponent* com, FLModel* selModel);

public:
	ModelPropsFrame(int x, int y, int w, int h, ModelsManager* modelsMgr);
	~ModelPropsFrame();

	void actionPerformed(SUIActionEvent SUIActionEvent);

	void SetUIValuesFromModel(FLModel* model);
	void ShowBoundingShapes(bool show);

	void UpdateTransInfo(FLModel* model);
	void UpdateRotationInfo(FLModel* model);
};

#endif
