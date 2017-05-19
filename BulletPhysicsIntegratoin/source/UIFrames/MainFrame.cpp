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
	_frame->SetName("Main Frame", SUIFrame::LEFT);

	_frame->Add(new SUIButton("Load Models", this));
	_frame->Add(new SUIButton("Save Models", this));

	_frame->Add( enablePhysics = new SUICheckBox("Enable Physics", SUICheckBox::LEFT, this));
	_frame->Add( pausePhysics = new SUICheckBox("Pause Physics", SUICheckBox::LEFT, this));
	
	_frame->Add( showMarkedObjs = new SUICheckBox("Show Marked Objects", SUICheckBox::LEFT, this));
	_frame->Add( showUnmarkedObjs = new SUICheckBox("Show Unmarked Objects", SUICheckBox::LEFT, this));
	_frame->Add( markAllObjs = new SUIButton("Mark all objects", SUIButton::LEFT, this));
	_frame->Add( unmarkAllObjs = new SUIButton("Unmark all objects", SUIButton::LEFT, this));

	_frame->Add( showBoundShapes = new SUICheckBox("Show Bounding Shapes", SUICheckBox::LEFT, this) );
	_frame->Add( showBoundBox = new SUICheckBox("Show Bounding Box", SUICheckBox::LEFT, this) );
	_frame->Add( onBorder = new SUICheckBox("Selected Object Border", SUICheckBox::LEFT, this) );

	showMarkedObjs->SetSelect(true);
	showUnmarkedObjs->SetSelect(true);
	onBorder->SetSelect(true);

	SUIBox* cameraBox = new SUIBox(SUIBox::V_ALIGNMENT);
	cameraBox->SetMargin(5,5,10,5);
	cameraBox->SetName("Camera Options", SUIBox::LEFT);
	cameraBox->SetOnOffEnable(true);
	cameraBox->SetOn(true);

	cameraBox->AddBox( SetUpPivotBox()		);
	cameraBox->AddBox( SetUpProjBox()		);
	cameraBox->AddBox( SetUpCamViewBox()	);
	cameraBox->SetBgVisible(true);
	cameraBox->SetBgColor(64, 64, 64, 255);

	_frame->Add( cameraBox );
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
	else if(com == enablePhysics)
	{
		_modelsMgr->SetAsPhysicsObjects(enablePhysics->IsSelected());

		if(enablePhysics->IsSelected())
			_modelProps->SetVisible(pausePhysics->IsSelected());
		else
			_modelProps->SetVisible(true);

		if(!_modelProps->IsVisible())
		{
			_modelProps->SetUIValuesFromModel(NULL);
			_modelsMgr->SetSelectedModelIndex(-1);
		}

		bool saveAndBShapesVisibility = !(enablePhysics->IsSelected() && pausePhysics->IsSelected());

		_modelProps->SetVisibleSaveBox(saveAndBShapesVisibility);
		_modelProps->SetVisibleBShapesBox(saveAndBShapesVisibility);
		_modelProps->ResetBounds();
	}
	else if(com == pausePhysics)
	{
		if(enablePhysics->IsSelected())
			_modelProps->SetVisible(pausePhysics->IsSelected());
		else
			_modelProps->SetVisible(true);

		if(!_modelProps->IsVisible())
		{
			_modelProps->SetUIValuesFromModel(NULL);
			_modelsMgr->SetSelectedModelIndex(-1);
		}

		bool saveAndBShapesVisibility = !(enablePhysics->IsSelected() && pausePhysics->IsSelected());

		_modelProps->SetVisibleSaveBox(saveAndBShapesVisibility);
		_modelProps->SetVisibleBShapesBox(saveAndBShapesVisibility);
		_modelProps->ResetBounds();
	}
}

void MainFrame::SetAsOrtho(bool ortho)		{	projRadio->SetSelect( ortho ? 1 : 0);		}
void MainFrame::SelectOriginAsModel()		{	pivot->SetSelect(1);						}
void MainFrame::SetBorderOn(bool on)		{	onBorder->SetSelect(on);					}
void MainFrame::SetPhysicsOn(bool on)		{	enablePhysics->SetSelect(on);				}

bool MainFrame::IsPhysicsEnabled()			{	return enablePhysics->IsSelected();			}
bool MainFrame::IsPhysicsPaused()			{	return pausePhysics->IsSelected();			}

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
	pivotBox->SetMargin(3,3,5,0);
	pivotBox->SetName("Pivot", SUIBox::LEFT);
	pivot = new SUIRadioButton(SUIRadioButton::V_ALIGNMENT);
	pivot->AddCheckBox(new SUICheckBox("Origin as pivot", SUICheckBox::LEFT));
	pivot->AddCheckBox(new SUICheckBox("Selected object as pivot", SUICheckBox::LEFT));
	pivot->AddActionListener(this);
	pivot->SetName("Pivot", SUIRadioButton::LEFT);
	pivot->SetSelect(0);
	pivotBox->AddRadioButton(pivot);
	pivotBox->SetOnOffEnable(true);

	return pivotBox;
}

SUIBox* MainFrame::SetUpProjBox()
{
	SUIBox* projBox = new SUIBox(SUIBox::V_ALIGNMENT);
	projBox->SetMargin(3,3,5,0);
	projBox->SetName("Projection", SUIBox::LEFT);
	projRadio = new SUIRadioButton(SUIRadioButton::V_ALIGNMENT);
	projRadio->SetName("Projection", SUIRadioButton::LEFT);
	projRadio->AddCheckBox(new SUICheckBox("Perspective", SUICheckBox::LEFT));
	projRadio->AddCheckBox(new SUICheckBox("Ortho", SUICheckBox::LEFT));
	projRadio->SetSelect(0);
	projRadio->AddActionListener(this);
	projBox->AddRadioButton(projRadio);
	projBox->SetOnOffEnable(true);
	projBox->SetOn(false);

	return projBox;
}

SUIBox* MainFrame::SetUpCamViewBox()
{
	SUIBox* viewTypeBox = new SUIBox(SUIBox::V_ALIGNMENT);
	viewTypeBox->SetMargin(3,3,5,5);
	viewTypeBox->SetName("View Type", SUIBox::LEFT);

	viewTypeBox->AddButton(new SUIButton("Change View", SUIBox::LEFT, this));
	viewTypeBox->AddButton(new SUIButton("Front View", SUIBox::LEFT, this));
	viewTypeBox->AddButton(new SUIButton("Back View", SUIBox::LEFT, this));
	viewTypeBox->AddButton(new SUIButton("Left View", SUIBox::LEFT, this));
	viewTypeBox->AddButton(new SUIButton("Right View", SUIBox::LEFT, this));
	viewTypeBox->AddButton(new SUIButton("Top View", SUIBox::LEFT, this));
	viewTypeBox->AddButton(new SUIButton("Bottom View", SUIBox::LEFT, this));

	viewTypeBox->SetOnOffEnable(true);
	viewTypeBox->SetOn(false);

	return viewTypeBox;
}

SUIBox* MainFrame::SetUpFloorBox()
{
	SUIBox* floorBox = new SUIBox(SUIBox::V_ALIGNMENT);
	floorBox->AddCheckBox( floorVisible = new SUICheckBox("Visible", SUICheckBox::LEFT, this) );
	floorBox->AddCheckBox( showAxis = new SUICheckBox("Show Axis", SUICheckBox::LEFT, this) );
	floorBox->AddCheckBox( showGrid = new SUICheckBox("Show Grid", SUICheckBox::LEFT, this) );
	floorBox->AddCheckBox( showGridLines = new SUICheckBox("Show Grid Lines", SUICheckBox::LEFT, this) );

	floorBox->SetMargin(5,5,5,0);
	floorBox->SetName("Floor", SUIBox::LEFT);
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
