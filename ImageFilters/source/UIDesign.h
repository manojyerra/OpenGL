#ifndef UIDesign_H
#define UIDesign_H

#include "SUI/SUIFrame.h"
#include "SUI/SUIList.h"
#include "SUI/SUISlider.h"
#include "SUI/SUICheckBox.h"
#include "SUI/SUIBox.h"
#include "SUI/SUIActionListener.h"
#include "ImageBuffer.h"

class UIDesign
{
private:
	SUIFrame* _listFrame;
	SUIList* _list;

	SUIFrame* _sliderFrame;

	SUISlider* _slider1;
	SUISlider* _sliderR;
	SUISlider* _sliderG;
	SUISlider* _sliderB;

	SUICheckBox* _invertRedChkBox;
	SUICheckBox* _invertGreenChkBox;
	SUICheckBox* _invertBlueChkBox;
	SUIBox* _invertChannelBox;

	int _blurValue;
	int _sharpValue;
	int _emBossValue;
	int _edgeDetectionValue;
	int _hueValue;
	int _blackAndWhiteValue;
	int _sepiaValue;
	int _darkValue;
	int _brightValue;
	int _contrastValue;
	int _pixelateValue;
	int _explosionValue;
	int _transValue;

	float _windowW;
	float _windowH;

public:
	UIDesign(float windowWidth, float windowHeight, SUIActionListener* SUIActionListener);
	~UIDesign();

	void ActionOnListSelection(ImageBuffer* SUIBuff);
	void ActionOnSliderMove(ImageBuffer* SUIBuff);
	void InVisibleSliderFrameComponents();
	SUIFrame* GetListFrame();
	SUIFrame* GetSliderFrame();
	SUIList* GetOptionsList();
	bool IsSliderFrameComponent(SUIComponent* com);
};

#endif


