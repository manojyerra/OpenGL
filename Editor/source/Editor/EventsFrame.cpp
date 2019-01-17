#include "EventsFrame.h"
#include "Sui/SuiInput.h"
#include "windows.h"
#include "IO/File/FileReader.h"

EventsFrame::EventsFrame(int x, int y, int w, int h)
{
	_frame = new SuiFrame(x, y, w, h, SuiFrame::V_ALIGNMENT);
	_frame->SetName("Events", SuiFrame::CENTER);
	_frame->SetMaximized(true);
	_frame->SetBounds( _frame->GetX(), _frame->GetY(), _frame->GetW(), _frame->GetH());

	SuiLabel* tempLabel1 = new SuiLabel("", SuiLabel::CENTER);
	tempLabel1->SetBgColor(0,0,0,0);
	tempLabel1->SetBgGradient(false);
	_frame->Add(tempLabel1);

	_frame->Add(new SuiButton("Save", this));
	
	SuiLabel* tempLabel2 = new SuiLabel("", SuiLabel::CENTER);
	tempLabel2->SetBgColor(0,0,0,0);
	tempLabel2->SetBgGradient(false);
	_frame->Add(tempLabel2);

	//_frame->Add( GetEventBox("Camera Zoom In") );
	//_frame->Add( GetEventBox("Camera Zoom Out") );
	//_frame->Add( GetEventBox("Camera Move") );
	//_frame->Add( GetEventBox("Camera Rotate") );

	_frame->Add( _objSelectionBox = GetEventBox("Select Object") );
	_frame->Add( _objFocusBox = GetEventBox("Focus Object On/Off") );
	_frame->Add( _objSaveBox = GetEventBox("Save Selected Object") );
	_frame->Add( _objDuplicateBox = GetEventBox("Duplicate Selected Object") );
	_frame->Add( _objDeleteBox = GetEventBox("Delete Selected Object") );

	_frame->Add( _physicsBox = GetEventBox("Physics On/Off") );
	_frame->Add( _wireFrameBox = GetEventBox("WireFrame On/Off") );
	_frame->Add( _cameraViewBox = GetEventBox("Camera Ortho / Perspective") );

	_frame->Add( _topBottomViewBox = GetEventBox("Top View / Bottom View") );
	_frame->Add( _frontBackViewBox = GetEventBox("Front View / Back View") );
	_frame->Add( _leftRightViewBox = GetEventBox("Left View / Right View") );
	_frame->Add( _bestBoundingBox = GetEventBox("Create Bounding Shape") );

	_frame->SetMaximized(true);
	_frame->SetMinimized(true);

	if(ReadInputsFromFile() == false)
	{
		SetInput(_objSelectionBox,		"Left Mouse",	"Mouse Click",			"Select Key",		"Select Event");
		SetInput(_objFocusBox,			"Left Mouse",	"Mouse Double Click",	"Select Key",		"Select Event");
		SetInput(_objSaveBox,			"Control",		"Key Press",			"S",				"Key Release");
		SetInput(_objDuplicateBox,		"Control",		"Key Press",			"D",				"Key Release");
		SetInput(_objDeleteBox,			"Control",		"Key Press",			"X",				"Key Release");
		SetInput(_physicsBox,			"P",			"Key Release",			"Select Key",		"Select Event");
		SetInput(_wireFrameBox,			"W",			"Key Release",			"Select Key",		"Select Event");
		SetInput(_cameraViewBox,		"C",			"Key Release",			"Select Key",		"Select Event");
		SetInput(_topBottomViewBox,		"T",			"Key Release",			"Select Key",		"Select Event");
		SetInput(_frontBackViewBox,		"F",			"Key Release",			"Select Key",		"Select Event");
		SetInput(_leftRightViewBox,		"V",			"Key Release",			"Select Key",		"Select Event");
		SetInput(_bestBoundingBox,		"B",			"Key Release",			"Select Key",		"Select Event");
	}
}

void EventsFrame::actionPerformed(SuiActionEvent suiActionEvent)
{
	SuiComponent* com = (SuiComponent*)suiActionEvent.GetComponent();
	if(com->GetName().compare("Save") == 0)
	{
		WriteInputsToFile();
	}
}

vector<int> EventsFrame::GetObjSelectionInput()		{	return GetInputVec(_objSelectionBox);	}
vector<int> EventsFrame::GetObjSaveInput()			{	return GetInputVec(_objSaveBox);		}
vector<int> EventsFrame::GetObjDuplicateInput()		{	return GetInputVec(_objDuplicateBox);	}
vector<int> EventsFrame::GetObjDeleteInput()		{	return GetInputVec(_objDeleteBox);		}
vector<int> EventsFrame::GetObjFocusInput()			{	return GetInputVec(_objFocusBox);		}
vector<int> EventsFrame::GetPhysicsInput()			{	return GetInputVec(_physicsBox);		}
vector<int> EventsFrame::GetWireFrameInput()		{	return GetInputVec(_wireFrameBox);		}
vector<int> EventsFrame::GetCameraViewInput()		{	return GetInputVec(_cameraViewBox);		}
vector<int> EventsFrame::GetTopBottomViewInput()	{	return GetInputVec(_topBottomViewBox);	}
vector<int> EventsFrame::GetFrontBackViewInput()	{	return GetInputVec(_frontBackViewBox);	}
vector<int> EventsFrame::GetLeftRightViewInput()	{	return GetInputVec(_leftRightViewBox);	}
vector<int> EventsFrame::GetBestBoundingInput()		{	return GetInputVec(_bestBoundingBox);	}

vector<int> EventsFrame::GetInputVec(SuiBox* box)
{
	vector<SuiComponent*> comList = box->getComponentList();

	vector<int> vec;

	for(int i=0;i<comList.size(); i++)
	{
		if(comList[i]->GetComponentID() == SuiComponent::BOX)
		{
			vector<SuiComponent*> innerComList = ((SuiBox*)comList[i])->getComponentList();

			SuiChoice* keyChoice = (SuiChoice*)innerComList[0];
			SuiChoice* eventChoice = (SuiChoice*)innerComList[1];

			string keyName = keyChoice->GetItem(keyChoice->GetSelectedIndex());
			vec.push_back(GetKeyID(keyName));

			string eventName = eventChoice->GetItem(eventChoice->GetSelectedIndex());
			vec.push_back(GetEventID(eventName));
		}
	}

	return vec;
}

void EventsFrame::SetInput(SuiBox* box, string str1, string str2, string str3, string str4)
{
	vector<SuiComponent*> comList = box->getComponentList();

	SuiBox* subBox1 = (SuiBox*)comList[1];
	SuiBox* subBox2 = (SuiBox*)comList[2];

	SuiChoice* choice1 = (SuiChoice*)(subBox1->getComponentList())[0];
	SuiChoice* choice2 = (SuiChoice*)(subBox1->getComponentList())[1];

	SuiChoice* choice3 = (SuiChoice*)(subBox2->getComponentList())[0];
	SuiChoice* choice4 = (SuiChoice*)(subBox2->getComponentList())[1];

	choice1->SetSelect(str1);
	choice2->SetSelect(str2);
	choice3->SetSelect(str3);
	choice4->SetSelect(str4);
}

int EventsFrame::GetKeyID(string name)
{
	if(name.length() == 1)
	{
		return (int)name[0];
	}
	else
	{
		if(name.compare(" ") == 0)			return 0;
		else if(name.compare("Left Mouse") == 0)	return SuiInput::MOUSE_LEFT;
		else if(name.compare("Middle Mouse") == 0)	return SuiInput::MOUSE_MIDDLE;
		else if(name.compare("Right Mouse") == 0)	return SuiInput::MOUSE_RIGHT;
		else if(name.compare("Mouse Wheel") == 0)	return SuiInput::MOUSE_WHEEL;

		else if(name.compare("Control") == 0)	return VK_CONTROL;
		else if(name.compare("Shift") == 0)		return VK_CONTROL;
		else if(name.compare("Alt") == 0)		return VK_CONTROL;
		else if(name.compare("Space") == 0)		return VK_CONTROL;
		else if(name.compare("Enter") == 0)		return VK_CONTROL;
		else if(name.compare("Escape") == 0)	return VK_CONTROL;
		else if(name.compare("Tab") == 0)		return VK_CONTROL;
		else if(name.compare("Delete") == 0)	return VK_CONTROL;

		else if(name.compare("Up Arrow") == 0)		return VK_UP;
		else if(name.compare("Down Arrow") == 0)	return VK_DOWN;
		else if(name.compare("Left Arrow") == 0)	return VK_LEFT;
		else if(name.compare("Right Arrow") == 0)	return VK_RIGHT;
	}

	return 0;
}

int EventsFrame::GetEventID(string name)
{
	if(name.compare(" ") == 0)							return 0;
	else if(name.compare("Key Type") == 0)				return SuiInput::KEY_TYPE;
	else if(name.compare("Key Press") == 0)				return SuiInput::KEY_PRESS;
	else if(name.compare("Key Release") == 0)			return SuiInput::KEY_RELEASE;
	else if(name.compare("Mouse Click") == 0)			return SuiInput::MOUSE_CLICK;
	else if(name.compare("Mouse Press") == 0)			return SuiInput::MOUSE_PRESS;
	else if(name.compare("Mouse Release") == 0)			return SuiInput::MOUSE_RELEASE;
	else if(name.compare("Mouse Drag") == 0)			return SuiInput::MOUSE_DRAG;
	else if(name.compare("Mouse Double Click") == 0)	return SuiInput::MOUSE_DOUBLE_CLICK;
	else if(name.compare("Mouse Wheel Up") == 0)		return SuiInput::MOUSE_WHEEL_UP;
	else if(name.compare("Mouse Wheel Down") == 0)		return SuiInput::MOUSE_WHEEL_DOWN;

	return 0;
}

SuiBox* EventsFrame::GetEventBox(string name)
{
	SuiBox* eventBox = new SuiBox(SuiBox::H_ALIGNMENT);
	eventBox->SetData(name);

	SuiLabel* label = new SuiLabel(name, SuiLabel::CENTER);
	label->SetMargin(5,5,5,5);

	SuiBox* keyEventPairBox1 = GetKeyEventPair();
	keyEventPairBox1->SetMargin(5, 5, 5, 5);

	SuiBox* keyEventPairBox2 = GetKeyEventPair();
	keyEventPairBox2->SetMargin(5, 5, 5, 5);

	eventBox->AddLabel(label);
	eventBox->AddBox(keyEventPairBox1);
	eventBox->AddBox(keyEventPairBox2);

	eventBox->SetOnOffEnable(false);

	return eventBox;
}

SuiBox* EventsFrame::GetKeyEventPair()
{
	SuiChoice* keys = new SuiChoice(15);

	keys->Add(" ");
	keys->Add("Left Mouse");
	keys->Add("Middle Mouse");
	keys->Add("Right Mouse");
	keys->Add("Mouse Wheel");

	keys->Add("Control");
	keys->Add("Shift");
	keys->Add("Alt");
	keys->Add("Space");
	keys->Add("Enter");
	keys->Add("Escape");
	keys->Add("Tab");
	keys->Add("Delete");
	keys->Add("Up Arrow");
	keys->Add("Down Arrow");
	keys->Add("Left Arrow");
	keys->Add("Right Arrow");

	keys->Add("A");
	keys->Add("B");
	keys->Add("C");
	keys->Add("D");
	keys->Add("E");
	keys->Add("F");
	keys->Add("G");
	keys->Add("H");
	keys->Add("I");
	keys->Add("J");
	keys->Add("K");
	keys->Add("L");
	keys->Add("M");
	keys->Add("N");
	keys->Add("O");
	keys->Add("P");
	keys->Add("Q");
	keys->Add("R");
	keys->Add("S");
	keys->Add("T");
	keys->Add("U");
	keys->Add("V");
	keys->Add("W");
	keys->Add("X");
	keys->Add("Y");
	keys->Add("Z");

	keys->Add("0");
	keys->Add("1");
	keys->Add("2");
	keys->Add("3");
	keys->Add("4");
	keys->Add("5");
	keys->Add("6");
	keys->Add("7");
	keys->Add("8");
	keys->Add("9");

	keys->SetSelect(0);

	SuiChoice* events = new SuiChoice(11);
	events->Add(" ");
	events->Add("Key Type");
	events->Add("Key Press");
	events->Add("Key Release");
	events->Add("Mouse Click");
	events->Add("Mouse Press");
	events->Add("Mouse Release");
	events->Add("Mouse Drag");
	events->Add("Mouse Double Click");
	events->Add("Mouse Wheel Up");
	events->Add("Mouse Wheel Down");
	events->SetSelect(0);

	SuiBox* box = new SuiBox(SuiBox::H_ALIGNMENT);
	
	box->AddChoice( keys );
	box->AddChoice( events );
	
	return box;
}

bool EventsFrame::ReadInputsFromFile()
{
	FILE* fp = fopen("data/Inputs.txt", "r");
	if(fp)
	{
		fclose(fp);
		fp = NULL;
	}
	else
		return false;

	FileReader fr("data/Inputs.txt");
	
	vector<string> vec1 = fr.Split( fr.GetTagStringData(_objSelectionBox->GetData(), false), ',');
	vector<string> vec2 = fr.Split( fr.GetTagStringData(_objFocusBox->GetData(), false), ',');
	vector<string> vec3 = fr.Split( fr.GetTagStringData(_objSaveBox->GetData(), false), ',');
	vector<string> vec4 = fr.Split( fr.GetTagStringData(_objDuplicateBox->GetData(), false), ',');
	vector<string> vec5 = fr.Split( fr.GetTagStringData(_objDeleteBox->GetData(), false), ',');
	vector<string> vec6 = fr.Split( fr.GetTagStringData(_physicsBox->GetData(), false), ',');
	vector<string> vec7 = fr.Split( fr.GetTagStringData(_wireFrameBox->GetData(), false), ',');
	vector<string> vec8 = fr.Split( fr.GetTagStringData(_cameraViewBox->GetData(), false), ',');
	vector<string> vec9 = fr.Split( fr.GetTagStringData(_topBottomViewBox->GetData(), false), ',');
	vector<string> vec10 = fr.Split( fr.GetTagStringData(_frontBackViewBox->GetData(), false), ',');
	vector<string> vec11 = fr.Split( fr.GetTagStringData(_leftRightViewBox->GetData(), false), ',');
	vector<string> vec12 = fr.Split( fr.GetTagStringData(_bestBoundingBox->GetData(), false), ',');

	SetInput(_objSelectionBox,		vec1[0], vec1[1], vec1[2], vec1[3]);
	SetInput(_objFocusBox,			vec2[0], vec2[1], vec2[2], vec2[3]);
	SetInput(_objSaveBox,			vec3[0], vec3[1], vec3[2], vec3[3]);
	SetInput(_objDuplicateBox,		vec4[0], vec4[1], vec4[2], vec4[3]);
	SetInput(_objDeleteBox,			vec5[0], vec5[1], vec5[2], vec5[3]);
	SetInput(_physicsBox,			vec6[0], vec6[1], vec6[2], vec6[3]);
	SetInput(_wireFrameBox,			vec7[0], vec7[1], vec7[2], vec7[3]);
	SetInput(_cameraViewBox,		vec8[0], vec8[1], vec8[2], vec8[3]);
	SetInput(_topBottomViewBox,		vec9[0], vec9[1], vec9[2], vec9[3]);
	SetInput(_frontBackViewBox,		vec10[0], vec10[1], vec10[2], vec10[3]);
	SetInput(_leftRightViewBox,		vec11[0], vec11[1], vec11[2], vec11[3]);
	SetInput(_bestBoundingBox,		vec12[0], vec12[1], vec12[2], vec12[3]);

	return true;
}

void EventsFrame::WriteInputsToFile()
{
	FILE* fp = fopen("data/Inputs.txt", "w");

	char arr[1024];

	for(int i=0; i<12; i++)
	{
		SuiBox* box = NULL;

		if(i == 0) box = _objSelectionBox;
		else if(i == 1) box = _objFocusBox;
		else if(i == 2) box = _objSaveBox;
		else if(i == 3) box = _objDuplicateBox;
		else if(i == 4) box = _objDeleteBox;
		else if(i == 5) box = _physicsBox;
		else if(i == 6) box = _wireFrameBox;
		else if(i == 7) box = _cameraViewBox;
		else if(i == 8) box = _topBottomViewBox; 
		else if(i == 9) box = _frontBackViewBox;
		else if(i == 10) box =_leftRightViewBox;
		else if(i == 11) box =_bestBoundingBox;

		vector<SuiComponent*> comList = box->getComponentList();

		SuiBox* subBox1 = (SuiBox*)comList[1];
		SuiBox* subBox2 = (SuiBox*)comList[2];

		SuiChoice* choice1 = (SuiChoice*)(subBox1->getComponentList())[0];
		SuiChoice* choice2 = (SuiChoice*)(subBox1->getComponentList())[1];

		SuiChoice* choice3 = (SuiChoice*)(subBox2->getComponentList())[0];
		SuiChoice* choice4 = (SuiChoice*)(subBox2->getComponentList())[1];

		string line = "";

		sprintf(arr, "<%s>\n", box->GetData().c_str());
		line = string(arr);
		fwrite(line.c_str(), 1, line.length(), fp);

		sprintf(arr, "%s,%s,%s,%s\n", choice1->GetSelectedStr().c_str(), choice2->GetSelectedStr().c_str(), choice3->GetSelectedStr().c_str(), choice4->GetSelectedStr().c_str());
		line = string(arr);
		fwrite(line.c_str(), 1, line.length(), fp);

		sprintf(arr, "</%s>\n\n", box->GetData().c_str());
		line = string(arr);
		fwrite(line.c_str(), 1, line.length(), fp);
	}

	fclose(fp);
}

EventsFrame::~EventsFrame()
{
	if(_frame)
	{
		delete _frame;
		_frame = NULL;
	}
}
