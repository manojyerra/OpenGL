#ifndef SUI_MANAGER_H
#define SUI_MANAGER_H

#include "SuiIncludes.h"
#include "SuiFrame.h"
#include "SuiButton.h"
#include "SuiLabel.h"
#include "SuiCheckBox.h"
#include "SuiRadioButton.h"
#include "SuiSlider.h"

#include <vector>
using namespace std;

class SuiManager
{
private:
	float _windowWidth;
	float _windowHeight;

	static SuiManager* _ref;
	vector<SuiFrame*> _framesVec;
	SuiFrame* _activeFrame;
	SuiRect _statusBarRect;
	SuiComponent* _dialogCom;
	bool _newFrameAdded;
	static const int STATUS_BAR_H = 30;

	SuiManager();
	~SuiManager();
	void Update();
	void CheckForResetBounds();
	void Draw();

	void ActiveFrame(int frameIndex);
	void FireEvent(SuiComponent* com, SuiEvents &eventVec);

public:
	static SuiManager* GetInstance();
	static void DeleteInstance();

	void Setup(int windowWidth, int windowHeight); //int argc, char** argv);

	void Run();
	float GetFPS();
	unsigned int GetTimeInMilliSeconds();
	bool Contains(float mx, float my);

	float GetWindowWidth();
	float GetWindowHeight();
	float GetWindowHeightWithoutStatusBar();
	void AddFrame(SuiFrame* frame);
	void RemoveFrame(SuiFrame* frame);
	void SetFocusOn(SuiFrame* frame);
	void SetDialogCom(SuiComponent* dialogCom);
};

#endif
