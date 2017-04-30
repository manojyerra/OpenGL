#ifndef MainFrame_H
#define MainFrame_H

#include "SUI/SUIFrame.h"
#include "SUI/SUIActionListener.h"
#include "SUI/SUIRadioButton.h"
#include "SUI/SUICheckBox.h"
#include "SUI/SUIButton.h"
#include "SUI/SUILabel.h"
#include "SUI/SUIChoice.h"


class MainFrame
{
private:
	SUIFrame* _frame;
	SUIRadioButton* projRadio;

	SUICheckBox* showAxis;
	SUICheckBox* showGrid;
	SUICheckBox* showGridLines;

	SUICheckBox* enablePhysics;
	SUICheckBox* showObjs;
	SUICheckBox* showBounds;
	SUICheckBox* onBorder;

	SUIRadioButton* pivot;

	SUIChoice* meterialChoice;

	//SuiLabel* fpsLabel;

public:
	MainFrame(int x, int y, int w, int h, SUIActionListener* actionListener);
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