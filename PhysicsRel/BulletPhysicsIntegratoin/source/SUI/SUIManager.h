#ifndef SUIManager_H
#define SUIManager_H

#include "SUIIncludes.h"
#include "SUIFrame.h"
#include "SUIButton.h"
#include "SUITextField.h"
#include "SUILabel.h"
#include "SUICheckBox.h"
#include "SUIRadioButton.h"
#include "SUISlider.h"

#include <vector>
using namespace std;

class SUIManager
{
private:
	float _windowWidth;
	float _windowHeight;

	static SUIManager* _ref;
	vector<SUIFrame*> _framesVec;
	SUIFrame* _activeFrame;
	SUIRect _statusBarRect;
	SUIComponent* _dialogCom;
	bool _newFrameAdded;
	static const int STATUS_BAR_H = 30;

	SUIManager();
	~SUIManager();
	void Update();
	void CheckForResetBounds();
	void Draw();

	void ActiveFrame(int frameIndex);
	void FireEvent(SUIComponent* com, SUIEvents &eventVec);

public:
	static SUIManager* GetInstance();
	static void DeleteInstance();

	void Setup(int windowWidth, int windowHeight); //int argc, char** argv);

	void Run();
	float GetFPS();
	unsigned int GetTimeInMilliSeconds();
	bool Contains(float mx, float my);

	float GetWindowWidth();
	float GetWindowHeight();
	float GetWindowHeightWithoutStatusBar();
	void AddFrame(SUIFrame* frame);
	void RemoveFrame(SUIFrame* frame);
	void SetFocusOn(SUIFrame* frame);
	void SetDialogCom(SUIComponent* dialogCom);
};

#endif
