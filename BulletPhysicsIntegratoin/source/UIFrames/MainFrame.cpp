#include "MainFrame.h"
#include "SUI/SUIButton.h"
#include "SUI/SUIBox.h"

MainFrame::MainFrame(int x, int y, int w, int h, Cam* cam, Floor* floor, ModelsManager* modelsMgr, ModelPropsFrame* modelProps)
{
	_cam = cam;
	_floor = floor;
	_modelsMgr = modelsMgr;
	_modelProps = modelProps;

	_frame = new SUIFrame((float)x, (float)y, (float)w, (float)h, SUIFrame::V_ALIGNMENT);
	_frame->SetName("Main Frame", SUIFrame::CENTER);

	_frame->Add(new SUIButton("Load Models", this));
	_frame->Add(new SUIButton("Save Models", this));

	_frame->Add( enablePhysics = new SUICheckBox("Enable Physics", SUICheckBox::CENTER, this));
	_frame->Add( showMarkedObjs = new SUICheckBox("Show Marked Objects", SUICheckBox::CENTER, this));
	_frame->Add( showUnmarkedObjs = new SUICheckBox("Show Unmarked Objects", SUICheckBox::CENTER, this));
	_frame->Add( markAllObjs = new SUIButton("Mark all objects", SUIButton::CENTER, this));
	_frame->Add( unmarkAllObjs = new SUIButton("Unmark all objects", SUIButton::CENTER, this));

	_frame->Add( showBoundShapes = new SUICheckBox("Show Bounding Shapes", SUICheckBox::CENTER, this) );
	_frame->Add( showBoundBox = new SUICheckBox("Show Bounding Box", SUICheckBox::CENTER, this) );
	_frame->Add( onBorder = new SUICheckBox("Selected Object Border", SUICheckBox::CENTER, this) );

	_frame->Add( textField = new SUITextField("100.0", SUICheckBox::LEFT, this, SUITextField::INPUT_DOUBLE) );

	showMarkedObjs->SetSelect(true);
	showUnmarkedObjs->SetSelect(true);
	onBorder->SetSelect(true);

	_frame->Add( SetUpPivotBox()	);
	_frame->Add( SetUpProjBox()		);
	_frame->Add( SetUpCamViewBox()	) ;
	_frame->Add( SetUpFloorBox()	);
}

void MainFrame::actionPerformed(SUIActionEvent e)
{
	SUIComponent* com = (SUIComponent*)e.GetComponent();
	string name = com->GetName();

	if(name == "Projection")
	{
		int selIndex = ((SUIRadioButton*)com)->GetSelectedIndex();

		if(selIndex == 0)			_cam->SetPerspectiveView();
		else if(selIndex == 1)		_cam->SetOrthoView();
	}
	else if(name == "Pivot")
	{
		int selIndex = ((SUIRadioButton*)com)->GetSelectedIndex();

		if(selIndex == 0)
		{
			_cam->SetPivot(CVector3(0,0,0));
		}
		else if(selIndex == 1)
		{
			FLModel* flModel = _modelsMgr->GetSelectedModel();

			if(flModel)
				_cam->SetPivot(CVector3(flModel->GetPos()));
		}
	}
	else if(name == "Change View")		_cam->ChangeView();
	else if(name == "Front View")		_cam->SetFrontView();
	else if(name == "Back View")		_cam->SetBackView();
	else if(name == "Left View")		_cam->SetLeftView();
	else if(name == "Right View")		_cam->SetRightView();
	else if(name == "Top View")			_cam->SetTopView();
	else if(name == "Bottom View")		_cam->SetBottomView();

	else if(name == "Show Axis")		_floor->SetAxisVisible(showAxis->IsSelected());
	else if(name == "Show Grid")		_floor->SetGridVisible(showGrid->IsSelected());
	else if(name == "Show Grid Lines")	_floor->SetGridLinesVisible(showGridLines->IsSelected());
	else if(com == floorVisible)		_floor->SetVisible(floorVisible->IsSelected());

	else if(com == showMarkedObjs)
	{
		_modelsMgr->ShowMarkedObjects(showMarkedObjs->IsSelected());
		_modelProps->SetUIValuesFromModel( _modelsMgr->GetSelectedModel() );
	}
	else if(com == showUnmarkedObjs)
	{
		_modelsMgr->ShowUnmarkedObjects(showUnmarkedObjs->IsSelected());
		_modelProps->SetUIValuesFromModel( _modelsMgr->GetSelectedModel() );
	}
	else if(com == showBoundShapes)
	{
		_modelsMgr->ShowBoundingShapes(showBoundShapes->IsSelected());
		_modelProps->SetUIValuesFromModel( _modelsMgr->GetSelectedModel() );
	}
	else if(com == showBoundBox)
	{
		_modelsMgr->SetBoundingBoxEnabled(showBoundBox->IsSelected());
		_modelProps->SetUIValuesFromModel( _modelsMgr->GetSelectedModel() );
	}
	else if(com == markAllObjs)
	{
		_modelsMgr->MarkAllObjects();
		_modelProps->SetUIValuesFromModel( _modelsMgr->GetSelectedModel() );
	}
	else if(com == unmarkAllObjs)
	{
		_modelsMgr->UnmarkAllObjects();
		_modelProps->SetUIValuesFromModel( _modelsMgr->GetSelectedModel() );
	}
	else if(com == textField)
	{
		double val = textField->GetDouble();
		textField->SetDouble(val*2);
	}
}

void MainFrame::SetAsOrtho(bool ortho)		{	projRadio->SetSelect( ortho ? 1 : 0);		}
void MainFrame::SelectOriginAsModel()		{	pivot->SetSelect(1);						}
void MainFrame::SetBorderOn(bool on)		{	onBorder->SetSelect(on);					}
void MainFrame::SetPhysicsOn(bool on)		{	enablePhysics->SetSelect(on);				}

bool MainFrame::IsPhysicsEnabled()			{	return enablePhysics->IsSelected();			}
//bool MainFrame::IsShowingObjects()			{	return showObjs->IsSelected();				}
bool MainFrame::IsShowingBoundingShapes()	{	return showBoundShapes->IsSelected();		}
bool MainFrame::IsShowingBorder()			{	return onBorder->IsSelected();				}

bool MainFrame::IsOriginAsPivot()			{	return (pivot->GetSelectedIndex() == 0);	}
bool MainFrame::IsSelectedObjectAsPivot()	{	return (pivot->GetSelectedIndex() == 1);	}

bool MainFrame::IsOrtho()					{	return (projRadio->GetSelectedIndex() == 1);}
bool MainFrame::IsAxisEnable()				{	return showAxis->IsSelected();				}
bool MainFrame::IsGridEnable()				{	return showGrid->IsSelected();				}
bool MainFrame::IsGridLinesEnable()			{	return showGridLines->IsSelected();			}

SUIBox* MainFrame::SetUpPivotBox()
{
	SUIBox* pivotBox = new SUIBox(SUIBox::V_ALIGNMENT);
	pivotBox->SetMargin(10,10,10,0);
	pivotBox->SetName("Pivot", SUIBox::CENTER);
	pivot = new SUIRadioButton(SUIRadioButton::V_ALIGNMENT);
	pivot->AddCheckBox(new SUICheckBox("Origin as pivot", SUICheckBox::CENTER));
	pivot->AddCheckBox(new SUICheckBox("Selected object as pivot", SUICheckBox::CENTER));
	pivot->AddActionListener(this);
	pivot->SetName("Pivot", SUIRadioButton::CENTER);
	pivot->SetSelect(0);
	pivotBox->AddRadioButton(pivot);
	pivotBox->SetOnOffEnable(true);

	return pivotBox;
}

SUIBox* MainFrame::SetUpProjBox()
{
	SUIBox* projBox = new SUIBox(SUIBox::V_ALIGNMENT);
	projBox->SetMargin(10,10,10,0);
	projBox->SetName("Projection", SUIBox::CENTER);
	projRadio = new SUIRadioButton(SUIRadioButton::V_ALIGNMENT);
	projRadio->SetName("Projection", SUIRadioButton::CENTER);
	projRadio->AddCheckBox(new SUICheckBox("Perspective", SUICheckBox::CENTER));
	projRadio->AddCheckBox(new SUICheckBox("Ortho", SUICheckBox::CENTER));
	projRadio->SetSelect(0);
	projRadio->AddActionListener(this);
	projBox->AddRadioButton(projRadio);
	projBox->SetOnOffEnable(true);
	projBox->SetOn(true);

	return projBox;
}

SUIBox* MainFrame::SetUpCamViewBox()
{
	SUIBox* viewTypeBox = new SUIBox(SUIBox::V_ALIGNMENT);
	viewTypeBox->SetMargin(10,10,10,0);
	viewTypeBox->SetName("View Type", SUIBox::CENTER);

	viewTypeBox->AddButton(new SUIButton("Change View", this));
	viewTypeBox->AddButton(new SUIButton("Front View", this));
	viewTypeBox->AddButton(new SUIButton("Back View", this));
	viewTypeBox->AddButton(new SUIButton("Left View", this));
	viewTypeBox->AddButton(new SUIButton("Right View", this));
	viewTypeBox->AddButton(new SUIButton("Top View", this));
	viewTypeBox->AddButton(new SUIButton("Bottom View", this));

	viewTypeBox->SetOnOffEnable(true);
	viewTypeBox->SetOn(false);

	return viewTypeBox;
}

SUIBox* MainFrame::SetUpFloorBox()
{
	SUIBox* floorBox = new SUIBox(SUIBox::V_ALIGNMENT);
	floorBox->AddCheckBox( floorVisible = new SUICheckBox("Visible", SUICheckBox::CENTER, this) );
	floorBox->AddCheckBox( showAxis = new SUICheckBox("Show Axis", SUICheckBox::CENTER, this) );
	floorBox->AddCheckBox( showGrid = new SUICheckBox("Show Grid", SUICheckBox::CENTER, this) );
	floorBox->AddCheckBox( showGridLines = new SUICheckBox("Show Grid Lines", SUICheckBox::CENTER, this) );

	floorBox->SetMargin(10,10,10,0);
	floorBox->SetName("Floor", SUIBox::CENTER);
	floorBox->SetOnOffEnable(true);
	floorBox->SetOn(false);

	floorVisible->SetSelect( _floor->IsVisible() );
	showAxis->SetSelect( _floor->IsAxisVisible() );
	showGrid->SetSelect( _floor->IsGridVisible() );
	showGridLines->SetSelect( _floor->IsGridLinesVisible() );

	return floorBox;
}

MainFrame::~MainFrame()
{
	if(_frame)
	{
		delete _frame;
		_frame = NULL;
	}
}
