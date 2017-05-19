#ifndef ModelPropsFrame_H
#define ModelPropsFrame_H

#include "SUI/SUIFrame.h"
#include "SUI/SUIButton.h"
#include "SUI/SUICheckBox.h"
#include "SUI/SUIChoice.h"
#include "SUI/SUIActionListener.h"
#include "SUI/SUIBox.h"
#include "SUI/SUITextField.h"
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
	SUITextField* _transXTF;
	SUITextField* _transYTF;
	SUITextField* _transZTF;

	SUIButton* _transXZeroBtn;
	SUIButton* _transYZeroBtn;
	SUIButton* _transZZeroBtn;

	SUIButton* _copyTransBtn;
	SUIButton* _pasteTransBtn;
	//End : Translation box related ui


	//Begin : Rotation box related ui
	SUITextField* _rotXTF;
	SUITextField* _rotYTF;
	SUITextField* _rotZTF;

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


	//Begin : Save box related ui
	SUIBox*	_saveBox;
	SUIButton* _saveBoundingShapes;
	SUIButton* _saveAABB;
	SUIButton* _saveTransformation;
	SUIButton* _saveAll;

	//End : Save box related ui


	//Begin : Bouding shapes box

	vector<SUICheckBox*> _bShapesChKBoxVec;
	vector<Shape*> _bShapesVec;
	SUIButton* _deleteBoudingShape;
	SUIButton* _selectAllBoundingShapes;
	SUIButton* _deSelectAllBoundingShapes;

	//End : Bouding shapes box


	//Begin : ClipBoard values

	static const int NONE = 0;
	static const int ROT = 0;
	static const int TRANS = 0;

	int _copyType;
	CVector3 _rotCopy;
	CVector3 _transCopy;

	//End : ClipBoard values


	SUIBox* SetUpLightingBox();
	SUIBox* SetUpTransBox();
	SUIBox* SetUpRotationBox();
	SUIBox* SetUpSaveBox();
	

	SUIBox* _bShapesBox;

	void AddUIForBoundingShapes(FLModel* model);
	void AddCheckBoxesForBoundingShapes(FLModel* model, SUIBox* box);

	bool CheckLightBoxUI(SUIComponent* com, FLModel* selModel);
	bool CheckBoudingShapesBoxUI(SUIComponent* com, FLModel* selModel);

public:
	ModelPropsFrame(int x, int y, int w, int h, ModelsManager* modelsMgr);
	~ModelPropsFrame();

	void ResetBounds();

	void SetVisible(bool visible);
	bool IsVisible();

	void SetVisibleSaveBox(bool visible);
	void SetVisibleBShapesBox(bool visible);

	void actionPerformed(SUIActionEvent SUIActionEvent);

	void SetUIValuesFromModel(FLModel* model);
	void ShowBoundingShapes(bool show);

	void UpdateTransInfo(FLModel* model);
	void UpdateRotationInfo(FLModel* model);
};

#endif
