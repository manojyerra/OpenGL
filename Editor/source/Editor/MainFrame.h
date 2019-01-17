#ifndef MainFrame_H
#define MainFrame_H

#include "Sui/SuiFrame.h"
#include "Sui/SuiActionListener.h"
#include "Sui/SuiRadioButton.h"
#include "Sui/SuiCheckBox.h"
#include "Sui/SuiButton.h"
#include "Sui/SuiLabel.h"
#include "Sui/SuiChoice.h"


class MainFrame
{
private:
	SuiFrame* _frame;
	SuiRadioButton* projRadio;

	SuiCheckBox* showAxis;
	SuiCheckBox* showGrid;
	SuiCheckBox* showGridLines;

	SuiCheckBox* enablePhysics;
	SuiCheckBox* showObjs;
	SuiCheckBox* showBounds;
	SuiCheckBox* onBorder;

	SuiRadioButton* pivot;

	SuiChoice* meterialChoice;

	//SuiLabel* fpsLabel;

public:
	MainFrame(int x, int y, int w, int h, SuiActionListener* actionListener);
	~MainFrame();

	void SetAsOrtho(bool ortho);

	bool IsOrtho();
	bool IsAxisEnable();
	bool IsGridEnable();
	bool IsGridLinesEnable();

	void SetFPS(float fps);

	bool IsPhysicsEnabled();
	bool CanShowObjects();
	bool CanShowBoundingShapes();
	bool CanShowBorder();

	bool IsOriginPivot();
	void SelectOriginAsModel();
	void PhysicsOn(bool on);
	void BorderOn(bool on);
};

#endif