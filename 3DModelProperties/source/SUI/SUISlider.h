#ifndef SUISlider_H
#define SUISlider_H

#include "SUIComponent.h"
#include "SUIActionListener.h"
#include "SUIMouseListener.h"
#include "SUIRect.h"

class SUISlider : public SUIComponent
{
private:
	SUIActionListener* _actionListener;
	SUIMouseListener* _mouseListener;

	double _minValue;
	double _maxValue;
	double _currValue;
	
	bool _isIntegerValue;
	string _valueAsString;

	SUIRect _valueBoxRect;
	SUIRect _sliderBgRect;
	SUIRect _sliderBgLineRect;
	SUIRect _pointerRect;
	float _pointerX;
	bool _pointerActive;
	

	void CalcPointerPos();
	void Init(string name, float minValue, float maxValue, bool isIntegerValue, SUIActionListener* actionListener);

public:
	SUISlider(string name, float minValue, float maxValue, bool isIntegerValue);
	SUISlider(string name, float minValue, float maxValue, bool isIntegerValue, SUIActionListener* actionListener);

	~SUISlider();

	void SetMinValue(float minValue);
	void SetMaxValue(float maxValue);

	void SetValue(double value);
	double GetValue();
	void SetPointerAt(double percent);
	SUIEvents UpdateByInput();
	void Move(float dx, float dy);
	void ResetBounds();
	void Draw();

	//Listeners...
	void AddMouseListener(SUIMouseListener* mouseListener)		{ _mouseListener = mouseListener;	}
	void RemoveMouseListener()									{ _mouseListener = NULL;			}
	SUIMouseListener* GetMouseListener()						{ return _mouseListener;			}

	void AddActionListener(SUIActionListener* actionListener)	{ _actionListener = actionListener; }
	void RemoveActionListener()									{ _actionListener = NULL;			}
	SUIActionListener* GetActionListener()						{ return _actionListener;			}
};

#endif