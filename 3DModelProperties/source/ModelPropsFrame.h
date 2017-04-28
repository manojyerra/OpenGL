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
	
	SUISlider* shininessSlider;
	
	SUIBox* CreateLightingUI();

public:
	ModelPropsFrame(int x, int y, int w, int h, ModelsManager* modelsMgr);
	~ModelPropsFrame();

	void actionPerformed(SUIActionEvent SUIActionEvent);
};

#endif