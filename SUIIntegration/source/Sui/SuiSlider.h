#ifndef SUI_SLIDER_H
#define SUI_SLIDER_H

#include "SuiComponent.h"
#include "SuiActionListener.h"
#include "SuiMouseListener.h"
#include "SuiRect.h"

class SuiSlider : public SuiComponent
{
private:
	SuiActionListener* _actionListener;
	SuiMouseListener* _mouseListener;

	double _minValue;
	double _maxValue;
	double _currValue;
	
	bool _isIntegerValue;
	string _valueAsString;

	SuiRect _valueBoxRect;
	SuiRect _sliderBgRect;
	SuiRect _sliderBgLineRect;
	SuiRect _pointerRect;
	float _pointerX;
	bool _pointerActive;
	
	void CalcPointerPos();

public:
	SuiSlider(string name, float minValue, float maxValue, bool isIntegerValue);
	~SuiSlider();

	void SetMinValue(float minValue);
	void SetMaxValue(float maxValue);

	void SetValue(double value);
	double GetValue();
	void SetPointerAt(double percent);
	SuiEvents UpdateByInput();
	void Move(float dx, float dy);
	void ResetBounds();
	void Draw();

	//Listeners...
	void AddMouseListener(SuiMouseListener* mouseListener)		{ _mouseListener = mouseListener;	}
	void RemoveMouseListener()									{ _mouseListener = NULL;			}
	SuiMouseListener* GetMouseListener()						{ return _mouseListener;			}

	void AddActionListener(SuiActionListener* actionListener)	{ _actionListener = actionListener; }
	void RemoveActionListener()									{ _actionListener = NULL;			}
	SuiActionListener* GetActionListener()						{ return _actionListener;			}
};

#endif