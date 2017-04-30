#include "MainFrame.h"
#include "SUI/SUIButton.h"
#include "SUI/SUIBox.h"

MainFrame::MainFrame(int x, int y, int w, int h, SUIActionListener* actionListener)
{
	_frame = new SUIFrame(x, y, w, h, SUIFrame::V_ALIGNMENT);
	_frame->SetName("Main Frame", SUIFrame::CENTER);

	//fpsLabel = new SuiLabel("FPS = 0", SuiLabel::CENTER);

	//_frame->Add(fpsLabel);
	_frame->Add(new SUIButton("Load Models", actionListener));
	_frame->Add(new SUIButton("Save Models", actionListener));

	enablePhysics = new SUICheckBox("Enable Physics", SUICheckBox::CENTER);
	enablePhysics->AddActionListener(actionListener);
	_frame->Add(enablePhysics);

	showObjs = new SUICheckBox("Show Objects", SUICheckBox::CENTER);
	showObjs->AddActionListener(actionListener);
	showObjs->SetSelect(true);
	_frame->Add(showObjs);
	
	showBounds = new SUICheckBox("Show Bounding Shapes", SUICheckBox::CENTER);
	showBounds->AddActionListener(actionListener);
	showBounds->SetSelect(true);
	_frame->Add(showBounds);

	onBorder = new SUICheckBox("Selected Object Border", SUICheckBox::CENTER);
	onBorder->AddActionListener(actionListener);
	//onBorder->SetSelect(true);
	_frame->Add(onBorder);

	SUIBox* pivotBox = new SUIBox(SUIBox::V_ALIGNMENT);
	pivotBox->SetMargin(10,10,10,0);
	pivotBox->SetName("Pivot", SUIBox::CENTER);
	pivot = new SUIRadioButton(SUIRadioButton::V_ALIGNMENT);
	pivot->AddCheckBox(new SUICheckBox("Origin as pivot", SUICheckBox::CENTER));
	pivot->AddCheckBox(new SUICheckBox("Selected object as pivot", SUICheckBox::CENTER));
	pivot->AddActionListener(actionListener);
	pivot->SetName("Pivot", SUIRadioButton::CENTER);
	pivot->SetSelect(0);
	pivotBox->AddRadioButton(pivot);
	pivotBox->SetOnOffEnable(true);
	
	SUIBox* projBox = new SUIBox(SUIBox::V_ALIGNMENT);
	projBox->SetMargin(10,10,10,0);
	projBox->SetName("Projection", SUIBox::CENTER);
	projRadio = new SUIRadioButton(SUIRadioButton::V_ALIGNMENT);
	projRadio->SetName("Projection", SUIRadioButton::CENTER);
	projRadio->AddCheckBox(new SUICheckBox("Perspective", SUICheckBox::CENTER));
	projRadio->AddCheckBox(new SUICheckBox("Ortho", SUICheckBox::CENTER));
	projRadio->SetSelect(0);
	projRadio->AddActionListener(actionListener);
	projBox->AddRadioButton(projRadio);
	projBox->SetOnOffEnable(true);
	projBox->SetOn(true);

	//Floor box...
	SUIBox* floorBox = new SUIBox(SUIBox::V_ALIGNMENT);
	floorBox->SetMargin(10,10,10,0);
	floorBox->SetName("Floor", SUIBox::CENTER);
	
	showAxis = new SUICheckBox("Show Axis", SUICheckBox::CENTER);
	showGrid = new SUICheckBox("Show Grid", SUICheckBox::CENTER);
	showGridLines = new SUICheckBox("Show Grid Lines", SUICheckBox::CENTER);

	floorBox->AddCheckBox(showAxis);
	floorBox->AddCheckBox(showGrid);
	floorBox->AddCheckBox(showGridLines);
	floorBox->SetOnOffEnable(true);
	floorBox->SetOn(false);

	//ViewType Box...
	SUIBox* viewTypeBox = new SUIBox(SUIBox::V_ALIGNMENT);
	viewTypeBox->SetMargin(10,10,10,0);
	viewTypeBox->SetName("View Type", SUIBox::CENTER);

	viewTypeBox->AddButton(new SUIButton("Top View", actionListener));
	viewTypeBox->AddButton(new SUIButton("Bottom View", actionListener));
	viewTypeBox->AddButton(new SUIButton("Left View", actionListener));
	viewTypeBox->AddButton(new SUIButton("Right View", actionListener));
	viewTypeBox->AddButton(new SUIButton("Front View", actionListener));
	viewTypeBox->AddButton(new SUIButton("Back View", actionListener));

	viewTypeBox->SetOnOffEnable(true);
	viewTypeBox->SetOn(false);

	//Meterial
	meterialChoice = new SUIChoice(15);
	meterialChoice->SetName("Meterial", SUIChoice::CENTER);

	meterialChoice->Add("Static Object");
	meterialChoice->Add("Aluminium");
	meterialChoice->Add("Books");
	meterialChoice->Add("Brass");
	meterialChoice->Add("Brick");
	meterialChoice->Add("Bronze");
	meterialChoice->Add("Cement");
	meterialChoice->Add("Coal");
	meterialChoice->Add("Concrete"); 
	meterialChoice->Add("Copper");
	meterialChoice->Add("Cork");
	meterialChoice->Add("Glass");
	meterialChoice->Add("Gold");
	meterialChoice->Add("Granite");
	meterialChoice->Add("Iron");
	meterialChoice->Add("Lead");
	meterialChoice->Add("Lime");
	meterialChoice->Add("Magnesium");
	meterialChoice->Add("Mercury");
	meterialChoice->Add("Mud");
	meterialChoice->Add("Plaster");
	meterialChoice->Add("Plastic");
	meterialChoice->Add("Plywood");
	meterialChoice->Add("Sand");
	meterialChoice->Add("Snow");
	meterialChoice->Add("Soils");
	meterialChoice->Add("Steel");
	meterialChoice->Add("Stone");
	meterialChoice->Add("Zinc");

	meterialChoice->AddActionListener(actionListener);
	meterialChoice->SetSelect(0);

	_frame->Add(meterialChoice);
	_frame->Add(pivotBox);
	_frame->Add(projBox);
	//_frame->Add(transBox);
	_frame->Add(viewTypeBox);
	_frame->Add(floorBox);
}

void MainFrame::SetFPS(float fps)
{
	//char arr[128];
	//sprintf(arr, "FPS = %d",(int)fps);
	//fpsLabel->SetName(arr, SuiButton::CENTER);
}

void MainFrame::SetAsOrtho(bool ortho)
{
	if(ortho)
		projRadio->SetSelect(1);
	else
		projRadio->SetSelect(0);
}

bool MainFrame::IsOrtho()			{	return (projRadio->GetSelectedIndex() == 1);	}
bool MainFrame::IsAxisEnable()		{	return showAxis->IsSelected();				}
bool MainFrame::IsGridEnable()		{	return showGrid->IsSelected();				}
bool MainFrame::IsGridLinesEnable()	{	return showGridLines->IsSelected();			}

bool MainFrame::IsPhysicsEnabled()			{	return enablePhysics->IsSelected();	}
bool MainFrame::CanShowObjects()			{	return showObjs->IsSelected();		}
bool MainFrame::CanShowBoundingShapes()		{	return showBounds->IsSelected();	}
bool MainFrame::CanShowBorder()				{	return onBorder->IsSelected();		}

bool MainFrame::IsOriginPivot()				{	return (pivot->GetSelectedIndex() == 0);	}
void MainFrame::SelectOriginAsModel()		{	pivot->SetSelect(1);						}
void MainFrame::BorderOn(bool on)			{	onBorder->SetSelect(on);					}
void MainFrame::PhysicsOn(bool on)			
{	
	enablePhysics->SetSelect(on);				
}


MainFrame::~MainFrame()
{
	if(_frame)
	{
		delete _frame;
		_frame = NULL;
	}
}