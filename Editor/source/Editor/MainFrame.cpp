#include "MainFrame.h"
#include "Sui/SuiButton.h"
#include "Sui/SuiBox.h"

MainFrame::MainFrame(int x, int y, int w, int h, SuiActionListener* actionListener)
{
	_frame = new SuiFrame(x, y, w, h, SuiFrame::V_ALIGNMENT);
	_frame->SetName("Main Frame", SuiFrame::CENTER);

	//fpsLabel = new SuiLabel("FPS = 0", SuiLabel::CENTER);

	//_frame->Add(fpsLabel);
	_frame->Add(new SuiButton("Load Models", actionListener));
	_frame->Add(new SuiButton("Save Models", actionListener));

	enablePhysics = new SuiCheckBox("Enable Physics", SuiCheckBox::CENTER);
	enablePhysics->AddActionListener(actionListener);
	_frame->Add(enablePhysics);

	showObjs = new SuiCheckBox("Show Objects", SuiCheckBox::CENTER);
	showObjs->AddActionListener(actionListener);
	showObjs->SetSelect(true);
	_frame->Add(showObjs);
	
	showBounds = new SuiCheckBox("Show Bounding Shapes", SuiCheckBox::CENTER);
	showBounds->AddActionListener(actionListener);
	showBounds->SetSelect(true);
	_frame->Add(showBounds);

	onBorder = new SuiCheckBox("Selected Object Border", SuiCheckBox::CENTER);
	onBorder->AddActionListener(actionListener);
	//onBorder->SetSelect(true);
	_frame->Add(onBorder);

	SuiBox* pivotBox = new SuiBox(SuiBox::V_ALIGNMENT);
	pivotBox->SetMargin(10,10,10,0);
	pivotBox->SetName("Pivot", SuiBox::CENTER);
	pivot = new SuiRadioButton(SuiRadioButton::V_ALIGNMENT);
	pivot->AddCheckBox(new SuiCheckBox("Origin as pivot", SuiCheckBox::CENTER));
	pivot->AddCheckBox(new SuiCheckBox("Selected object as pivot", SuiCheckBox::CENTER));
	pivot->AddActionListener(actionListener);
	pivot->SetName("Pivot", SuiRadioButton::CENTER);
	pivot->SetSelect(0);
	pivotBox->AddRadioButton(pivot);
	pivotBox->SetOnOffEnable(true);
	
	SuiBox* projBox = new SuiBox(SuiBox::V_ALIGNMENT);
	projBox->SetMargin(10,10,10,0);
	projBox->SetName("Projection", SuiBox::CENTER);
	projRadio = new SuiRadioButton(SuiRadioButton::V_ALIGNMENT);
	projRadio->SetName("Projection", SuiRadioButton::CENTER);
	projRadio->AddCheckBox(new SuiCheckBox("Perspective", SuiCheckBox::CENTER));
	projRadio->AddCheckBox(new SuiCheckBox("Ortho", SuiCheckBox::CENTER));
	projRadio->SetSelect(0);
	projRadio->AddActionListener(actionListener);
	projBox->AddRadioButton(projRadio);
	projBox->SetOnOffEnable(true);
	projBox->SetOn(true);

	//Floor box...
	SuiBox* floorBox = new SuiBox(SuiBox::V_ALIGNMENT);
	floorBox->SetMargin(10,10,10,0);
	floorBox->SetName("Floor", SuiBox::CENTER);
	
	showAxis = new SuiCheckBox("Show Axis", SuiCheckBox::CENTER);
	showGrid = new SuiCheckBox("Show Grid", SuiCheckBox::CENTER);
	showGridLines = new SuiCheckBox("Show Grid Lines", SuiCheckBox::CENTER);

	floorBox->AddCheckBox(showAxis);
	floorBox->AddCheckBox(showGrid);
	floorBox->AddCheckBox(showGridLines);
	floorBox->SetOnOffEnable(true);
	floorBox->SetOn(false);

	//ViewType Box...
	SuiBox* viewTypeBox = new SuiBox(SuiBox::V_ALIGNMENT);
	viewTypeBox->SetMargin(10,10,10,0);
	viewTypeBox->SetName("View Type", SuiBox::CENTER);

	viewTypeBox->AddButton(new SuiButton("Top View", actionListener));
	viewTypeBox->AddButton(new SuiButton("Bottom View", actionListener));
	viewTypeBox->AddButton(new SuiButton("Left View", actionListener));
	viewTypeBox->AddButton(new SuiButton("Right View", actionListener));
	viewTypeBox->AddButton(new SuiButton("Front View", actionListener));
	viewTypeBox->AddButton(new SuiButton("Back View", actionListener));

	viewTypeBox->SetOnOffEnable(true);
	viewTypeBox->SetOn(false);

	//Meterial
	meterialChoice = new SuiChoice(15);
	meterialChoice->SetName("Meterial", SuiChoice::CENTER);

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