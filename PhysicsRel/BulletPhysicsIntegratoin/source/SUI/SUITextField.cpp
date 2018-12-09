#include "SUITextField.h"
#include "SUIFont.h"
#include "SUIInput.h"

SUITextField::SUITextField(string name, int inputType) : SUIComponent(SUIComponent::TEXTFIELD)
{
	Init(name, SUIComponent::CENTER, NULL, inputType);
}

SUITextField::SUITextField(string name, int nameAlignment, int inputType) : SUIComponent(SUIComponent::TEXTFIELD)
{
	Init(name, nameAlignment, NULL, inputType);
}

SUITextField::SUITextField(string name, SUIActionListener* actionListener, int inputType) : SUIComponent(SUIComponent::TEXTFIELD)
{
	Init(name, SUIComponent::CENTER, actionListener, inputType);
}

SUITextField::SUITextField(string name, int nameAlignment, SUIActionListener* actionListener, int inputType) : SUIComponent(SUIComponent::TEXTFIELD)
{
	Init(name, nameAlignment, actionListener, inputType);
}

void SUITextField::Init(string name, int nameAlignment, SUIActionListener* actionListener, int inputType)
{
	_name = name;
	_nameAlignment = nameAlignment;
	_actionListener = actionListener;
	_inputType = inputType;

	if(_inputType == INPUT_UNSIGNED_INT)
	{
		if(!IsValidUnsignedInt(_name))
			_name = "0";
	}
	else if(_inputType == INPUT_INT)
	{
		if(!IsValidInt(_name))
			_name = "0";
	}
	else if(_inputType == INPUT_DOUBLE)
	{
		if(!IsValidDouble(_name))
			_name = "0";
	}


	_h = 22;

	_isBgVisible = true;
	_isBorderVisible = true;
	_isBgGradient = false;

	_bgR = 171;
	_bgG = 171;
	_bgB = 171;
	_bgA = 255;

	_borderR = 57;
	_borderG = 57;
	_borderB = 57;
	_borderA = 255;

	_mouseListener = NULL;
}

SUITextField::~SUITextField()
{
}

void SUITextField::Move(float dx, float dy)
{
	_x += dx;
	_y += dy;
}

void SUITextField::ResetBounds()
{
}

void SUITextField::Exception(string info)
{
	int a = 1;
	int b = 0;
	int c = a/b;
	c++;
}

void SUITextField::SetText(string text)
{
	if(_inputType == INPUT_UNSIGNED_INT)
	{
		if(!IsValidUnsignedInt(_name))
			_name = "0";
	}
	else if(_inputType == INPUT_INT)
	{
		if(!IsValidInt(_name))
			_name = "0";
	}
	else if(_inputType == INPUT_DOUBLE)
	{
		if(!IsValidDouble(_name))
			_name = "0";
	}
	else if(_inputType == INPUT_STRING)
	{
		_name = text;
	}
}

string SUITextField::GetText()
{
	return _name;
}

void SUITextField::SetInt(int val)
{
	if(_inputType == INPUT_INT)
	{
		char arr[64];
		sprintf(arr, "%d",val);
		_name = string(arr);
	}
	else
	{
		Exception("Input type mismatch");
	}
}

int SUITextField::GetInt()
{
	if(_inputType == INPUT_INT)
	{
		return atoi(_name.c_str());
	}
	else
	{
		Exception("Input type mismatch");
	}
}

void SUITextField::SetUInt(unsigned int val)
{
	if(_inputType == INPUT_UNSIGNED_INT)
	{
		char arr[64];
		sprintf(arr, "%u",val);
		_name = string(arr);
	}
	else
	{
		Exception("Input type mismatch");
	}
}

unsigned int SUITextField::GetUInt()
{
	if(_inputType == INPUT_UNSIGNED_INT)
	{
		return (unsigned int)atol(_name.c_str());
	}
	else
	{
		Exception("Input type mismatch");
	}
}

void SUITextField::SetDouble(double val, int numDecimalsToShow)
{
	if(numDecimalsToShow < 0)		numDecimalsToShow = 0;
	else if(numDecimalsToShow > 6)	numDecimalsToShow = 6;

	if(_inputType == INPUT_DOUBLE)
	{
		char arr[64];

		if(numDecimalsToShow == 0)			sprintf(arr, "%lf",val);
		else if(numDecimalsToShow == 1)		sprintf(arr, "%.1lf",val);
		else if(numDecimalsToShow == 2)		sprintf(arr, "%.2lf",val);
		else if(numDecimalsToShow == 3)		sprintf(arr, "%.3lf",val);
		else if(numDecimalsToShow == 4)		sprintf(arr, "%.4lf",val);
		else if(numDecimalsToShow == 5)		sprintf(arr, "%.5lf",val);
		else if(numDecimalsToShow == 6)		sprintf(arr, "%.6lf",val);

		_name = string(arr);
	}
	else
	{
		Exception("Input type mismatch");
	}
}

double SUITextField::GetDouble()
{
	if(_inputType == INPUT_DOUBLE)
	{
		return atof(_name.c_str());
	}
	else
	{
		Exception("Input type mismatch");
	}
}

void SUITextField::Draw()
{
	if(_isBgVisible)
	{
		//if(SUIInput::IsMousePressed() && Contains((float)SUIInput::MX, (float)SUIInput::MY))
		//	DrawBackground(true);
		//else
			DrawBackground(false);
	}

	SUIFont::GetInstance()->SetColor(0xffffffff);
	SUIFont::GetInstance()->Begin();

	SUIFont::GetInstance()->HorBorder(_x, _x+_w);

	float horGap = -0.5;
	float fontSize = SUIFont::GetInstance()->GetFontSize();

	if(_nameAlignment == LEFT)			SUIFont::GetInstance()->DrawFromLeft(_name, _x+1, _y+_h/2, fontSize);
	else if(_nameAlignment == RIGHT)	SUIFont::GetInstance()->DrawFromRight(_name, _x+_w+1, _y+_h/2, fontSize);
	else								SUIFont::GetInstance()->DrawFromCenter(_name, _x+1+_w/2, _y+_h/2, fontSize);

	SUIFont::GetInstance()->End();

	if(_isBorderVisible)
		DrawBorder();
}

SUIEvents SUITextField::UpdateByInput()
{
	SUIEvents eventsVec;

	if(!Contains((float)SUIInput::MX, (float)SUIInput::MY))
		return eventsVec;

	int key = SUIInput::GetReleasedKey();

	if(_actionListener &&  key != 0)
	{
		if(key == VK_RETURN)
			eventsVec.ACTION_PERFORMED = true;
		else
			AppendChar(key);
	}

	return eventsVec;
}


void SUITextField::AppendChar(int key)
{
	if(key == VK_BACK)
	{
		if(_name.length() > 0)
			_name = _name.substr(0, _name.length()-1);
	}
	else if(key == VK_ESCAPE)
	{
		_name = "";
	}
	else
	{
		if(_inputType == INPUT_UNSIGNED_INT)
		{
			if(key >= '0' && key <= '9')
				_name += key;
		}
		else if(_inputType == INPUT_INT)
		{
			string tempStr = _name;
			tempStr += key;

			if(IsValidInt(tempStr))
				_name += key;
		}
		else if(_inputType == INPUT_DOUBLE)
		{
			string tempStr = _name;
			tempStr += key;

			if(IsValidDouble(tempStr))
				_name += key;
		}
		else
		{
			_name += key;
		}
	}
}

bool SUITextField::IsValidUnsignedInt(string str)
{
	for(int i=0; i<str.length(); i++)
	{
		if(!(str[i] >= '0' && str[i] <= '9'))
			return false;
	}

	return true;
}

bool SUITextField::IsValidInt(string str)
{
	for(int i=0; i<str.length(); i++)
	{
		if(i==0 && (str[i] == '-' || str[i] == '+'))
			continue;

		if(!(str[i] >= '0' && str[i] <= '9'))
			return false;
	}

	return true;
}

bool SUITextField::IsValidDouble(string str)
{
	int dotCount = 0;

	for(int i=0; i<str.length(); i++)
	{
		if(i==0 && (str[i] == '-' || str[i] == '+'))
			continue;

		if(str[i] == '.')
		{
			dotCount++;

			if(dotCount == 1)
				continue;
		}

		if(!(str[i] >= '0' && str[i] <= '9'))
			return false;
	}

	return true;
}
