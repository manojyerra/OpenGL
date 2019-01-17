#ifndef ModelPropsFrame_H
#define ModelPropsFrame_H

#include "Sui/SuiFrame.h"
#include "Sui/SuiButton.h"
#include "Sui/SuiCheckBox.h"
#include "Sui/SuiChoice.h"
#include "Sui/SuiActionListener.h"
#include "Model3D/Model3D.h"
#include "Util/Rect.h"

#include "Bound/BoundingBox.h"
#include "Bound/BoundingCylinder.h"
#include "Bound/BoundingSphere.h"
#include "Bound/BoundingCone.h"

class ModelPropsFrame : SuiActionListener
{
private:
	Model3D* _model3D;
	Rect _rect;

	SuiFrame* _frame;

	SuiButton* _transXBtn;
	SuiButton* _transYBtn;
	SuiButton* _transZBtn;

	SuiButton* _rotXBtn;
	SuiButton* _rotYBtn;
	SuiButton* _rotZBtn;

	SuiButton* _scaleXBtn;
	SuiButton* _scaleYBtn;
	SuiButton* _scaleZBtn;

	SuiLabel* texPathLabel;
	SuiButton* texBrowse;

	SuiLabel* massLabel;

	void AddMeterials(SuiChoice* choice);

public:
	ModelPropsFrame(int x, int y, int w, int h, SuiActionListener* actionListener);
	~ModelPropsFrame();
	void actionPerformed(SuiActionEvent suiActionEvent);
	void SetTrans(float x, float y, float z);
	void SetRotation(float xRot, float yRot, float zRot);
	void SetScale(float xScale, float yScale, float zScale);
	bool AddBoundingShape(string name, Model3D* model3D);
	void Update();
	void UpdateModelProps(Model3D* model3D, Rect rect);
	void Draw();
};

#endif