#ifndef SUIBox_H
#define SUIBox_H

#include "SUIButton.h"
#include "SUILabel.h"
#include "SUICheckBox.h"
#include "SUIRadioButton.h"
#include "SUISlider.h"
#include "SUIChoice.h"
#include "SUITextField.h"

#include "SUIComponent.h"
#include <vector>
using namespace std;

class SUIBox : public SUIComponent
{
private:
	int _alignment;
	vector<SUIComponent*> _elementVec;
	bool _isOn;
	bool _isOnOffEnable;
	
	unsigned char _titleBgR;
	unsigned char _titleBgG;
	unsigned char _titleBgB;
	unsigned char _titleBgA;

	void CommonInit(int alignment, float leftMargin, float rightMargin, float topMargin, float bottomMargin);
	
	static const int TITLE_H = 22;

public:
	SUIBox(int alignment);
	SUIBox(int alignment, float horMargin, float verMargin);
	SUIBox(int alignment, float leftMargin, float rightMargin, float topMargin, float bottomMargin);
	~SUIBox();

	void SetOn(bool on);
	void SetOnOffEnable(bool onOffEnable);
	vector<SUIComponent*> getComponentList();
	SUIComponent* getComponentAt(float x, float y);

	void AddBox(SUIBox* box);
	void AddButton(SUIButton* box);
	void AddLabel(SUILabel* label);
	void AddCheckBox(SUICheckBox* checkBox);
	void AddRadioButton(SUIRadioButton* radioButton);
	void AddSlider(SUISlider* slider);
	void AddChoice(SUIChoice* choice);
	void AddList(SUIList* list);
	void AddTextField(SUITextField* textField);

	void ResetBounds();
	void Move(float dx, float dy);

	SUIEvents UpdateByInput();
	void Draw();
};

#endif
