#ifndef BOX_H
#define BOX_H

#include "SuiButton.h"
#include "SuiLabel.h"
#include "SuiCheckBox.h"
#include "SuiRadioButton.h"
#include "SuiSlider.h"
#include "SuiChoice.h"

#include "SuiComponent.h"
#include <vector>
using namespace std;

class SuiBox : public SuiComponent
{
private:
	int _alignment;
	vector<SuiComponent*> _elementVec;
	bool _isOn;
	bool _isOnOffEnable;
	
	unsigned char _titleBgR;
	unsigned char _titleBgG;
	unsigned char _titleBgB;
	unsigned char _titleBgA;

	void CommonInit(int alignment, float leftMargin, float rightMargin, float topMargin, float bottomMargin);
	
	static const int TITLE_H = 22;

public:
	SuiBox(int alignment);
	SuiBox(int alignment, float horMargin, float verMargin);
	SuiBox(int alignment, float leftMargin, float rightMargin, float topMargin, float bottomMargin);
	~SuiBox();

	void SetOn(bool on);
	void SetOnOffEnable(bool onOffEnable);
	vector<SuiComponent*> getComponentList();
	SuiComponent* getComponentAt(float x, float y);

	void AddBox(SuiBox* box);
	void AddButton(SuiButton* box);
	void AddLabel(SuiLabel* label);
	void AddCheckBox(SuiCheckBox* checkBox);
	void AddRadioButton(SuiRadioButton* radioButton);
	void AddSlider(SuiSlider* slider);
	void AddChoice(SuiChoice* choice);
	void AddList(SuiList* list);

	void ResetBounds();
	void Move(float dx, float dy);

	SuiEvents UpdateByInput();
	void Draw();
};

#endif