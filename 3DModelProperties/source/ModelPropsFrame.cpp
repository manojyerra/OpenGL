#include "ModelPropsFrame.h"
#include "SUI/SUIInput.h"
#include "SUI/SUILabel.h"
#include "SUI/SUIBox.h"

ModelPropsFrame::ModelPropsFrame(int x, int y, int w, int h, ModelsManager* modelsMgr)
{
	_modelsMgr = modelsMgr;

	_frame = new SUIFrame(x, y, w, h, SUIFrame::V_ALIGNMENT);
	_frame->SetName("Model Properties", SUIFrame::LEFT);

	SUIBox* box = new SUIBox(SUIBox::V_ALIGNMENT);
	box->SetMargin(0,0,10,10);

	box->AddCheckBox(new SUICheckBox("WireFrame Lines", this));
	box->AddCheckBox(new SUICheckBox("WireFrame Points", this));
	box->AddCheckBox(new SUICheckBox("Texture", this));
	box->AddCheckBox(new SUICheckBox("Bounding Box", this));
	box->AddCheckBox(new SUICheckBox("Bounding 2D Rect", this));
	
	_frame->Add(box);
}


void ModelPropsFrame::actionPerformed(SUIActionEvent e)
{
	SUIComponent* com = (SUIComponent*)e.GetComponent();
	string name = com->GetName();

	if(name == "WireFrame Lines")
	{
		FLModel* selModel = _modelsMgr->GetSelectedModel();

		if(selModel)
			selModel->SetWireFrameLinesEnabled(((SUICheckBox*)com)->IsSelected());
	}
	else if(name == "WireFrame Points")
	{
		FLModel* selModel = _modelsMgr->GetSelectedModel();

		if(selModel)
			selModel->SetWireFramePointsEnabled(((SUICheckBox*)com)->IsSelected());
	}
	else if(name == "Texture")
	{
		FLModel* selModel = _modelsMgr->GetSelectedModel();

		if(selModel)
			selModel->SetTextureEnabled(((SUICheckBox*)com)->IsSelected());
	}
	else if(name == "Bounding Box")
	{
		FLModel* selModel = _modelsMgr->GetSelectedModel();

		if(selModel)
			selModel->SetBoundingBoxEnabled(((SUICheckBox*)com)->IsSelected());
	}
	else if(name == "Bounding 2D Rect")
	{
		FLModel* selModel = _modelsMgr->GetSelectedModel();

		if(selModel)
			selModel->SetBounding2DRectEnabled(((SUICheckBox*)com)->IsSelected());
	}
}


ModelPropsFrame::~ModelPropsFrame()
{
	if(_frame)
	{
		delete _frame;
		_frame = NULL;
	}
}
