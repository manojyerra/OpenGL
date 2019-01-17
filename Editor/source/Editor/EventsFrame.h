#ifndef EventsFrame_H
#define EventsFrame_H

#include "Sui/SuiFrame.h"
#include "Sui/SuiBox.h"
#include "Sui/SuiActionListener.h"

#include <vector>
using namespace std;

class EventsFrame : SuiActionListener
{
private:
	SuiFrame* _frame;

	SuiBox* _objSelectionBox;
	SuiBox* _objSaveBox;
	SuiBox* _objDuplicateBox;
	SuiBox* _objDeleteBox;
	SuiBox* _objFocusBox;

	SuiBox* _physicsBox;
	SuiBox* _wireFrameBox;
	SuiBox* _cameraViewBox;
	SuiBox* _topBottomViewBox;
	SuiBox* _frontBackViewBox;
	SuiBox* _leftRightViewBox;
	SuiBox* _bestBoundingBox;

	SuiBox* GetEventBox(string name);
	SuiBox* GetKeyEventPair();

public:
	EventsFrame(int x, int y, int w, int h);
	~EventsFrame();
	
	vector<int> GetObjSelectionInput();
	vector<int> GetObjSaveInput();
	vector<int> GetObjDuplicateInput();
	vector<int> GetObjDeleteInput();
	vector<int> GetPhysicsInput();
	vector<int> GetWireFrameInput();
	vector<int> GetObjFocusInput();
	vector<int> GetCameraViewInput();
	vector<int> GetTopBottomViewInput();
	vector<int> GetFrontBackViewInput();
	vector<int> GetLeftRightViewInput();
	vector<int> GetBestBoundingInput();

	vector<int> GetInputVec(SuiBox* box);
	void SetInput(SuiBox* box, string str1, string str2, string str3, string str4);

	void actionPerformed(SuiActionEvent suiActionEvent);

	int GetKeyID(string name);
	int GetEventID(string name);

	bool ReadInputsFromFile();
	void WriteInputsToFile();

};

#endif