#ifndef ModelPropsFrame_H
#define ModelPropsFrame_H

#include "SUI/SUIFrame.h"
#include "SUI/SUIButton.h"
#include "SUI/SUICheckBox.h"
#include "SUI/SUIChoice.h"
#include "SUI/SUIActionListener.h"
//#include "Model3D/Model3D.h"
//#include "Util/Rect.h"

//#include "Bound/BoundingBox.h"
//#include "Bound/BoundingCylinder.h"
//#include "Bound/BoundingSphere.h"
//#include "Bound/BoundingCone.h"

class ModelPropsFrame : SUIActionListener
{
private:
	//Model3D* _model3D;
	//Rect _rect;

	SUIFrame* _frame;

	SUIButton* _transXBtn;
	SUIButton* _transYBtn;
	SUIButton* _transZBtn;

	SUIButton* _rotXBtn;
	SUIButton* _rotYBtn;
	SUIButton* _rotZBtn;

	SUIButton* _scaleXBtn;
	SUIButton* _scaleYBtn;
	SUIButton* _scaleZBtn;

	SUILabel* texPathLabel;
	SUIButton* texBrowse;

	SUILabel* massLabel;

	void AddMeterials(SUIChoice* choice);

public:
	ModelPropsFrame(int x, int y, int w, int h, SUIActionListener* actionListener);
	~ModelPropsFrame();
	void actionPerformed(SUIActionEvent SUIActionEvent);
	void SetTrans(float x, float y, float z);
	void SetRotation(float xRot, float yRot, float zRot);
	void SetScale(float xScale, float yScale, float zScale);
	//bool AddBoundingShape(string name, Model3D* model3D);
	void Update();
	//void UpdateModelProps(Model3D* model3D, Rect rect);
	void Draw();
};

#endif