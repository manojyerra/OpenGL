#include "UIDesign.h"
#include "SUI/SUIManager.h"
#include "SUI/SUIButton.h"

UIDesign::UIDesign(float windowWidth, float windowHeight, SUIActionListener* actionListener)
{
	_windowW = windowWidth;
	_windowH = windowHeight;

	///***************** List Creation ***********************//

	float frameHeight = 595;

	if(frameHeight > SUIManager::GetInstance()->GetWindowHeightWithoutStatusBar())
		frameHeight = SUIManager::GetInstance()->GetWindowHeightWithoutStatusBar()-1;

	_listFrame = new SUIFrame(windowWidth-200, 0, 200, frameHeight, SUIFrame::V_ALIGNMENT);

	_listFrame->SetName("BatchImageEditor", SUIFrame::LEFT);
	_listFrame->SetMargin(5,5,8,8);
	_listFrame->SetFocusOn();
	_listFrame->SetRemoveCloseOption(true);
	_listFrame->SetBgColor(200,200,200,255);

	int height = 130;
	int width = _listFrame->GetX();
	int y2 = SUIManager::GetInstance()->GetWindowHeightWithoutStatusBar();
	int y1 = y2-height;

	SUIButton* openBtn = new SUIButton("Open File");
	openBtn->AddActionListener(actionListener);

	SUIButton* saveBtn = new SUIButton("Save File");
	saveBtn->AddActionListener(actionListener);

	SUIButton* undoBtn = new SUIButton("Undo");
	undoBtn->AddActionListener(actionListener);

	_listFrame->Add(openBtn);
	_listFrame->Add(saveBtn);
	_listFrame->Add(undoBtn);

	_list = new SUIList(33);

	_list->Add("Colorize");
	_list->Add("Invert");
	_list->Add("InvertValue");
	_list->Add("InvertChannels");
	_list->Add("NegativePhoto");
	_list->Add("Sepia");
	_list->Add("Blur");
	_list->Add("Hue");
	_list->Add("GrayScale");
	_list->Add("BlackAndWhite");
	_list->Add("Sharpen");
	_list->Add("EdgeDetection");
	_list->Add("Bright");
	_list->Add("Dark");
	_list->Add("Pixelate");
	_list->Add("Contrast");
	_list->Add("Explosion");
	_list->Add("Emboss");
	_list->Add("Transparency");
	_list->Add("MotionBlur");
	_list->Add("FlipHorizantal");
	_list->Add("FilpVertical");
	_list->AddActionListener(actionListener);

	_list->SetNonSelectColor(150,150,150,255);
	_list->SetSelectColor(200,200,200,255);

	_listFrame->Add(_list);


	///*********************** Sliders Creation **************************/

	_sliderFrame = new SUIFrame(1,y1,width,height, SUIFrame::V_ALIGNMENT);
	_sliderFrame->SetBgColor(150,150,150,255);

	_slider1 = new SUISlider("", 0, 100, true);
	_slider1->SetMargin(0,0,3,0);
	_slider1->SetBorderVisible(false);
	_sliderFrame->Add(_slider1);
	_sliderFrame->SetVisible(false);

	_sliderR = new SUISlider("R ", 0, 128, true);
	_sliderR->SetMargin(0,0,10,5);
	_sliderR->SetValue(75);
	_sliderR->SetBorderVisible(false);

	_sliderFrame->Add(_sliderR);

	_sliderG = new SUISlider("G ", 0, 128, true);
	_sliderG->SetMargin(0,0,5,5);
	_sliderG->SetValue(0);
	_sliderG->SetBorderVisible(false);
	_sliderFrame->Add(_sliderG);

	_sliderB = new SUISlider("B ", 0, 128, true);
	_sliderB->SetMargin(0,0,5,10);
	_sliderB->SetValue(75);
	_sliderB->SetBorderVisible(false);
	_sliderFrame->Add(_sliderB);

	_slider1->AddActionListener(actionListener);
	_sliderR->AddActionListener(actionListener);
	_sliderG->AddActionListener(actionListener);
	_sliderB->AddActionListener(actionListener);

	_invertChannelBox = new SUIBox(SUIBox::H_ALIGNMENT);

	_invertRedChkBox = new SUICheckBox("Invert Red Channel", SUICheckBox::CENTER);
	_invertGreenChkBox = new SUICheckBox("Invert Green Channel", SUICheckBox::CENTER);
	_invertBlueChkBox = new SUICheckBox("Invert Blue Channel", SUICheckBox::CENTER);

	_invertRedChkBox->AddActionListener(actionListener);
	_invertGreenChkBox->AddActionListener(actionListener);
	_invertBlueChkBox->AddActionListener(actionListener);

	_invertChannelBox->AddCheckBox(_invertRedChkBox);
	_invertChannelBox->AddCheckBox(_invertGreenChkBox);
	_invertChannelBox->AddCheckBox(_invertBlueChkBox);

	_invertChannelBox->SetVisible(false);

	_sliderFrame->Add(_invertChannelBox);

	_blurValue = 1;
	_sharpValue = 50;
	_emBossValue = 10;
	_edgeDetectionValue = 30;
	_hueValue = 30;
	_blackAndWhiteValue = 50;
	_sepiaValue = 50;
	_darkValue = 50;
	_brightValue = 50;
	_contrastValue = 50;
	_pixelateValue = 4;
	_explosionValue = 3;
	_transValue = 50;
}

void UIDesign::ActionOnListSelection(ImageBuffer* imgBuf)
{
	string selectedStr = _list->GetItem(_list->GetSelectedIndex());

	if(selectedStr.compare("Blur") == 0)
	{
		_sliderFrame->SetVisible(true);
		_slider1->SetVisible(true);

		_slider1->SetName("BlurLevel  ", SUISlider::LEFT);
		_slider1->SetMinValue(1);
		_slider1->SetMaxValue(5);
		_slider1->SetValue(_blurValue);
		_slider1->ResetBounds();

		imgBuf->BlurImage(_blurValue);
	}
	else if(selectedStr.compare("Sharpen") == 0)
	{
		_sliderFrame->SetVisible(true);
		_slider1->SetVisible(true);

		_slider1->SetName("Sharpen Level  ", SUISlider::LEFT);
		_slider1->SetMinValue(1);
		_slider1->SetMaxValue(100);
		_slider1->SetValue(_sharpValue);
		_slider1->ResetBounds();

		imgBuf->Sharpen(_sharpValue);
	}
	else if(selectedStr.compare("Emboss") == 0)
	{
		_sliderFrame->SetVisible(true);
		_slider1->SetVisible(true);

		_slider1->SetName("Emboss Level  ", SUISlider::LEFT);
		_slider1->SetMinValue(1);
		_slider1->SetMaxValue(100);
		_slider1->SetValue(_emBossValue);
		_slider1->ResetBounds();

		imgBuf->Emboss(_emBossValue);
	}
	else if(selectedStr.compare("EdgeDetection") == 0)
	{
		_sliderFrame->SetVisible(true);
		_slider1->SetVisible(true);

		_slider1->SetName("Edge Detection Level  ", SUISlider::LEFT);
		_slider1->SetMinValue(1);
		_slider1->SetMaxValue(100);
		_slider1->SetValue(_edgeDetectionValue);
		_slider1->ResetBounds();

		imgBuf->EdgeDetection(_edgeDetectionValue);
	}
	else if(selectedStr.compare("Hue") == 0)
	{
		_sliderFrame->SetVisible(true);
		_slider1->SetVisible(true);

		_slider1->SetName("Hue Level  ", SUISlider::LEFT);
		_slider1->SetMinValue(1);
		_slider1->SetMaxValue(100);
		_slider1->SetValue(_hueValue);
		_slider1->ResetBounds();

		imgBuf->Hue(_hueValue);
	}
	else if(selectedStr.compare("BlackAndWhite") == 0)
	{
		_sliderFrame->SetVisible(true);
		_slider1->SetVisible(true);

		_slider1->SetName("Black And White Level  ", SUISlider::LEFT);
		_slider1->SetMinValue(1);
		_slider1->SetMaxValue(100);
		_slider1->SetValue(_blackAndWhiteValue);
		_slider1->ResetBounds();

		imgBuf->BlackAndWhite(_blackAndWhiteValue);
	}
	else if(selectedStr.compare("Sepia") == 0)
	{
		_sliderFrame->SetVisible(true);
		_slider1->SetVisible(true);

		_slider1->SetName("Sepia Level  ", SUISlider::LEFT);
		_slider1->SetMinValue(1);
		_slider1->SetMaxValue(100);
		_slider1->SetValue(_sepiaValue);
		_slider1->ResetBounds();

		imgBuf->Sepia(_sepiaValue);
	}
	else if(selectedStr.compare("Dark") == 0)
	{
		_sliderFrame->SetVisible(true);
		_slider1->SetVisible(true);

		_slider1->SetName("Dark Level  ", SUISlider::LEFT);
		_slider1->SetMinValue(1);
		_slider1->SetMaxValue(100);
		_slider1->SetValue(_darkValue);
		_slider1->ResetBounds();

		imgBuf->Dark(_darkValue);
	}
	else if(selectedStr.compare("Bright") == 0)
	{
		_sliderFrame->SetVisible(true);
		_slider1->SetVisible(true);

		_slider1->SetName("Bright Level  ", SUISlider::LEFT);
		_slider1->SetMinValue(1);
		_slider1->SetMaxValue(100);
		_slider1->SetValue(_brightValue);
		_slider1->ResetBounds();

		imgBuf->Bright(_brightValue);
	}
	else if(selectedStr.compare("Contrast") == 0)
	{
		_sliderFrame->SetVisible(true);
		_slider1->SetVisible(true);

		_slider1->SetName("Contrast Level  ", SUISlider::LEFT);
		_slider1->SetMinValue(1);
		_slider1->SetMaxValue(100);
		_slider1->SetValue(_contrastValue);
		_slider1->ResetBounds();

		imgBuf->Contrast(_contrastValue);
	}
	else if(selectedStr.compare("Pixelate") == 0)
	{
		_sliderFrame->SetVisible(true);
		_slider1->SetVisible(true);

		_slider1->SetName("Pixelate Level  ", SUISlider::LEFT);
		_slider1->SetMinValue(1);
		_slider1->SetMaxValue(100);
		_slider1->SetValue(_pixelateValue);
		_slider1->ResetBounds();

		imgBuf->PixelateNearest(_pixelateValue);
	}
	else if(selectedStr.compare("Explosion") == 0)
	{
		_sliderFrame->SetVisible(true);
		_slider1->SetVisible(true);

		_slider1->SetName("Explosion Level  ", SUISlider::LEFT);
		_slider1->SetMinValue(1);
		_slider1->SetMaxValue(100);
		_slider1->SetValue(_explosionValue);
		_slider1->ResetBounds();

		imgBuf->Explosion(_explosionValue);
	}
	else if(selectedStr.compare("Transparency") == 0)
	{
		_sliderFrame->SetVisible(true);
		_slider1->SetVisible(true);

		_slider1->SetName("Transparency Level  ", SUISlider::LEFT);
		_slider1->SetMinValue(1);
		_slider1->SetMaxValue(100);
		_slider1->SetValue(_transValue);
		_slider1->ResetBounds();

		imgBuf->SetTransparency(_transValue, true);
	}
	else if(selectedStr.compare("Colorize") == 0)
	{
		_sliderFrame->SetVisible(true);
		_sliderR->SetVisible(true);
		_sliderG->SetVisible(true);
		_sliderB->SetVisible(true);

		int r = _sliderR->GetValue();
		int g = _sliderG->GetValue();
		int b = _sliderB->GetValue();

		imgBuf->Colorize(r,g,b);
	}
	else if(selectedStr.compare("InvertChannels")==0)
	{
		_sliderFrame->SetVisible(true);
		_invertChannelBox->SetVisible(true);

		bool redInvert = _invertRedChkBox->IsSelected();
		bool blueInvert = _invertRedChkBox->IsSelected();
		bool greenInvert = _invertRedChkBox->IsSelected();

		imgBuf->InvertChannels(redInvert,greenInvert, blueInvert);
	}
	else if(selectedStr.compare("GrayScale") == 0)						imgBuf->GrayScale();
	else if(selectedStr.compare("ColorOverlay(0,0,255,50)") == 0)		imgBuf->ColorOverlay(0,0,255,50);
	else if(selectedStr.compare("FlipHorizantal") == 0)					imgBuf->FlipHorizantal();
	else if(selectedStr.compare("FilpVertical") == 0)					imgBuf->FilpVertical();
	else if(selectedStr.compare("Invert") == 0)							imgBuf->Invert();
	else if(selectedStr.compare("InvertValue") == 0)					imgBuf->InvertValue();
	else if(selectedStr.compare("NegativePhoto") == 0)					imgBuf->NegativePhoto();
	else if(selectedStr.compare("MotionBlur") == 0)						imgBuf->MotionBlur();
}


void UIDesign::ActionOnSliderMove(ImageBuffer* imgBuf)
{
	string selectedStr = _list->GetItem(_list->GetSelectedIndex());

	if(selectedStr.compare("Blur") == 0)
	{
		_blurValue = _slider1->GetValue();
		imgBuf->BlurImage(_blurValue);
	}
	else if(selectedStr.compare("Sharpen") == 0)
	{
		_sharpValue = _slider1->GetValue();
		imgBuf->Sharpen(_sharpValue);
	}
	else if(selectedStr.compare("Emboss") == 0)
	{
		_emBossValue = _slider1->GetValue();
		imgBuf->Emboss(_emBossValue);
	}
	else if(selectedStr.compare("EdgeDetection") == 0)
	{
		_edgeDetectionValue = _slider1->GetValue();
		imgBuf->EdgeDetection(_edgeDetectionValue);
	}
	else if(selectedStr.compare("Hue") == 0)
	{
		_hueValue = _slider1->GetValue();
		imgBuf->Hue(_hueValue);
	}
	else if(selectedStr.compare("BlackAndWhite") == 0)
	{
		_blackAndWhiteValue = _slider1->GetValue();
		imgBuf->BlackAndWhite(_blackAndWhiteValue);
	}
	else if(selectedStr.compare("Sepia") == 0)
	{
		_sepiaValue = _slider1->GetValue();
		imgBuf->Sepia(_sepiaValue);
	}
	else if(selectedStr.compare("Bright") == 0)
	{
		_brightValue = _slider1->GetValue();
		imgBuf->Bright(_brightValue);
	}
	else if(selectedStr.compare("Dark") == 0)
	{
		_darkValue = _slider1->GetValue();
		imgBuf->Dark(_darkValue);
	}
	else if(selectedStr.compare("Contrast") == 0)
	{
		_contrastValue = _slider1->GetValue();
		imgBuf->Contrast(_contrastValue);
	}
	else if(selectedStr.compare("Pixelate") == 0)
	{
		_contrastValue = _slider1->GetValue();
		imgBuf->PixelateNearest(_contrastValue);
	}
	else if(selectedStr.compare("Explosion") == 0)
	{
		_explosionValue = _slider1->GetValue();
		imgBuf->Explosion(_explosionValue);
	}
	else if(selectedStr.compare("Transparency") == 0)
	{
		_transValue = _slider1->GetValue();
		imgBuf->SetTransparency(_transValue, true);
	}
	else if(selectedStr.compare("Colorize") == 0)
	{
		int r = _sliderR->GetValue();
		int g = _sliderG->GetValue();
		int b = _sliderB->GetValue();

		imgBuf->Colorize(r,g,b);
	}
	else if(selectedStr.compare("InvertChannels") == 0)
	{
		bool redInvert = _invertRedChkBox->IsSelected();
		bool greenInvert = _invertGreenChkBox->IsSelected();
		bool blueInvert = _invertBlueChkBox->IsSelected();

		imgBuf->InvertChannels(redInvert,greenInvert, blueInvert);
	}
}

void UIDesign::InVisibleSliderFrameComponents()
{
	_sliderFrame->SetVisible(false);
	_slider1->SetVisible(false);
	_sliderR->SetVisible(false);
	_sliderG->SetVisible(false);
	_sliderB->SetVisible(false);
	_invertChannelBox->SetVisible(false);
}

bool UIDesign::IsSliderFrameComponent(SUIComponent* com)
{
	return (com == _slider1 || com == _sliderR || com == _sliderG || com == _sliderB || 
			com == _invertRedChkBox || com == _invertGreenChkBox || com == _invertBlueChkBox);
}

SUIFrame* UIDesign::GetListFrame()
{
	return _listFrame;
}

SUIFrame* UIDesign::GetSliderFrame()
{
	return _sliderFrame;
}

SUIList* UIDesign::GetOptionsList()
{
	return _list;
}

UIDesign::~UIDesign()
{
	if(_listFrame)
	{
		delete _listFrame;
		_listFrame = NULL;
	}

	if(_sliderFrame)
	{
		delete _sliderFrame;
		_sliderFrame = NULL;
	}
}