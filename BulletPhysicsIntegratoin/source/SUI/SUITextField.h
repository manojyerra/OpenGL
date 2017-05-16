#ifndef SUITextField_H
#define SUITextField_H

#include "SUIComponent.h"
#include "SUIActionListener.h"
#include "SUIMouseListener.h"

class SUITextField : public SUIComponent
{
private:
	int _inputType;

	SUIActionListener* _actionListener;
	SUIMouseListener* _mouseListener;

	void Init(string name, int nameAlignment, SUIActionListener* actionListener, int inputType);
	void AppendChar(int key);

	bool IsValidUnsignedInt(string str);
	bool IsValidInt(string str);
	bool IsValidDouble(string str);

	void Exception(string info);

public:
	static const int INPUT_UNSIGNED_INT = 1;
	static const int INPUT_INT = 2;
	static const int INPUT_DOUBLE = 3;
	static const int INPUT_STRING = 4;

	SUITextField(string name, int inputType);
	SUITextField(string name, int nameAlignment, int inputType);
	SUITextField(string name, SUIActionListener* actionListener, int inputType);
	SUITextField(string name, int nameAlignment, SUIActionListener* actionListener, int inputType);

	~SUITextField();

	SUIEvents UpdateByInput();
	void Move(float dx, float dy);
	void ResetBounds();
	void Draw();

	string GetText();
	int GetInt();
	unsigned int GetUInt();
	double GetDouble();

	void SetText(string text);
	void SetInt(int val);
	void SetUInt(unsigned int val);
	void SetDouble(double val, int numDecimalsToShow=0);

	//Listeners...
	void AddMouseListener(SUIMouseListener* mouseListener)		{ _mouseListener = mouseListener;	}
	void RemoveMouseListener()									{ _mouseListener = NULL;			}
	SUIMouseListener* GetMouseListener()						{ return _mouseListener;			}

	void AddActionListener(SUIActionListener* actionListener)	{ _actionListener = actionListener; }
	void RemoveActionListener()									{ _actionListener = NULL;			}
	SUIActionListener* GetActionListener()						{ return _actionListener;			}
};

#endif
