#ifndef SUIComponent_H
#define SUIComponent_H

#include "SUIEvents.h"

#include <string>
#include <vector>
using namespace std;

class SUIComponent
{
private:
	int _id;

protected:
	float _x;
	float _y;
	float _w;
	float _h;

	string _name;
	int _nameAlignment;

	string _data;

	bool _visible;

	float _leftMargin;
	float _rightMargin;
	float _topMargin;
	float _bottomMargin;

	bool _isBgVisible;
	bool _isBgGradient;
	bool _isBorderVisible;

	unsigned char _bgR;
	unsigned char _bgG;
	unsigned char _bgB;
	unsigned char _bgA;

	unsigned char _borderR;
	unsigned char _borderG;
	unsigned char _borderB;
	unsigned char _borderA;

	float _borderWidth;

	unsigned int MixColor(unsigned int baseColor, unsigned int mixColor, float percent);

public:
	static int const CENTER = 0;
	static int const LEFT = 1;
	static int const RIGHT = 2;	

	static const int V_ALIGNMENT = 1;
	static const int H_ALIGNMENT = 2;

	static const int FRAME = 1;
	static const int BOX = 2;
	static const int BUTTON = 3;
	static const int LABEL = 4;
	static const int CHECKBOX = 5;
	static const int RADIO_BUTTON = 6;
	static const int SLIDER = 7;
	static const int SUI_CHOICE = 8;
	static const int SUI_LIST = 9;

	SUIComponent(int id);
	virtual ~SUIComponent();

	int GetComponentID();
	
	void SetName(string name, int nameAlignment);
	string GetName();

	void SetData(string data);
	string GetData();

	void SetVisible(bool visible);
	bool IsVisible();

	void SetMargin(float left, float right, float top, float bottom);

	void SetHorMarign(float horMargin);
	void SetVerMarign(float verMargin);

	float LeftMargin();
	float RightMargin();
	float TopMargin();
	float BottomMargin();

	void SetBgColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
	void SetBgColor(unsigned char* color);
	void SetBorderWidth(float borderWidth);
	void SetBgVisible(bool visible);
	void SetBgGradient(bool isBgGradient);

	void SetBorderColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
	void SetBorderVisible(bool visible);

	void SetPos(float x, float y);
	void SetWidth(float w);

	float GetX();
	float GetY();
	float GetW();
	float GetH();

	void DrawBackground(bool onPress = false);
	void DrawBorder();

	virtual bool Contains(float x, float y);
	virtual bool IsDialogMode(){ return false; }
	virtual void UpdateDialog(){}
	virtual void DrawDialog(){}

	virtual void ResetBounds() = 0;
	virtual void Draw() = 0;
	virtual SUIEvents UpdateByInput() = 0;
	virtual void Move(float dx, float dy) = 0;
};

#endif