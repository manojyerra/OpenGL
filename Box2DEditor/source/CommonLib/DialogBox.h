#ifndef DIALOG_BOX_H
#define DIALOG_BOX_H
#pragma once
#include "../Primitives/RectPrim.h"

#include <vector>
#include <string>
using namespace std;

#define Show(param)				DialogBox::ShowDialog(param, __FILE__, __LINE__)
#define ShowMsg(param)			DialogBox::ShowDialog(param, __FILE__, __LINE__)
#define ShowFloat(param)		DialogBox::ShowFloatDialog(param, __FILE__, __LINE__)
#define ShowInt(param)			DialogBox::ShowIntDialog(param, __FILE__, __LINE__)

class DialogBox
{
private:
	static RectPrim* _bg;
	static RectPrim* _break;
	static RectPrim* _continue;
	static RectPrim* _ignoreAll;

	static vector<string> _fileNameVec;
	static vector<int>	_lineNumVec;

	static void DrawImgs(char* msg);
	static void ShowAnim(char* msg);

public:
	static const int BREAK = 1;
	static const int CONTINUE = 2;
	static const int IGNORE_ALL = 3;

	static void Init();
	static int ShowDialog(char* msg, const char* file, unsigned int line);
	static int ShowFloatDialog(float number, const char* file, unsigned int line);
	static int ShowIntDialog(int number, const char* file, unsigned int line);
	static void Terminate();
};
#endif