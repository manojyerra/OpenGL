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

	box->AddCheckBox(new SUICheckBox("Texture", this));
	box->AddCheckBox(new SUICheckBox("Bounding Box", this));
	box->AddCheckBox(new SUICheckBox("Bounding 2D Rect", this));
	box->AddCheckBox(new SUICheckBox("Show Bounding Shapes", this));
	box->AddCheckBox(new SUICheckBox("WireFrame Lines", this));
	box->AddCheckBox(new SUICheckBox("WireFrame Points", this));
	box->AddCheckBox(new SUICheckBox("Show Model", this));
	
	_frame->Add(box);
	_frame->Add(CreateLightingUI());
}


SUIBox* ModelPropsFrame::CreateLightingUI()
{
	SUIBox* boxAmbient = new SUIBox(SUIBox::V_ALIGNMENT);
	boxAmbient->SetName("Ambient", SUIBox::LEFT);
	boxAmbient->SetOn(false);
	boxAmbient->SetOnOffEnable(true);
	boxAmbient->AddSlider( _ambientR = new SUISlider("R", 0, 1.0f, false, this) );
	boxAmbient->AddSlider( _ambientG = new SUISlider("G", 0, 1.0f, false, this) );
	boxAmbient->AddSlider( _ambientB = new SUISlider("B", 0, 1.0f, false, this) );

	SUIBox* boxDiffuse = new SUIBox(SUIBox::V_ALIGNMENT);
	boxDiffuse->SetName("Diffuse", SUIBox::LEFT);
	boxDiffuse->SetOn(false);
	boxDiffuse->SetOnOffEnable(true);
	boxDiffuse->AddSlider( _diffuseR = new SUISlider("R", 0, 1.0f, false, this) );
	boxDiffuse->AddSlider( _diffuseG = new SUISlider("G", 0, 1.0f, false, this) );
	boxDiffuse->AddSlider( _diffuseB = new SUISlider("B", 0, 1.0f, false, this) );

	SUIBox* boxSpecular = new SUIBox(SUIBox::V_ALIGNMENT);
	boxSpecular->SetName("Specular", SUIBox::LEFT);
	boxSpecular->SetOnOffEnable(true);
	boxSpecular->SetOn(false);
	boxSpecular->AddSlider( _specularR = new SUISlider("R", 0, 1.0f, false, this) );
	boxSpecular->AddSlider( _specularG = new SUISlider("G", 0, 1.0f, false, this) );
	boxSpecular->AddSlider( _specularB = new SUISlider("B", 0, 1.0f, false, this) );

	SUIBox* boxLighting = new SUIBox(SUIBox::V_ALIGNMENT);
	boxLighting->SetName("Lighting", SUIBox::LEFT);
	boxLighting->SetMargin(5,5,5,5);
	boxLighting->SetOnOffEnable(true);
	boxLighting->AddCheckBox(new SUICheckBox("Lighting On", this));

	boxLighting->AddBox(boxAmbient);
	boxLighting->AddBox(boxDiffuse);
	boxLighting->AddBox(boxSpecular);

	boxLighting->AddSlider( shininessSlider = new SUISlider("Shininess", 0, 20, false, this) );

	return boxLighting;
}


void ModelPropsFrame::actionPerformed(SUIActionEvent e)
{
	SUIComponent* com = (SUIComponent*)e.GetComponent();
	string name = com->GetName();

	FLModel* selModel = _modelsMgr->GetSelectedModel();

	if(selModel)
	{
		if(name == "WireFrame Lines")
		{
			selModel->SetWireFrameLinesEnabled(((SUICheckBox*)com)->IsSelected());
		}
		else if(name == "WireFrame Points")
		{
			selModel->SetWireFramePointsEnabled(((SUICheckBox*)com)->IsSelected());
		}
		else if(name == "Texture")
		{
			selModel->SetTextureEnabled(((SUICheckBox*)com)->IsSelected());
		}
		else if(name == "Bounding Box")
		{
			selModel->SetBoundingBoxEnabled(((SUICheckBox*)com)->IsSelected());
		}
		else if(name == "Bounding 2D Rect")
		{
			selModel->SetBounding2DRectEnabled(((SUICheckBox*)com)->IsSelected());
		}
		else if(name == "Lighting On")
		{
			selModel->SetLightingEnabled(((SUICheckBox*)com)->IsSelected());
		}
		else if(name == "Show Bounding Shapes")
		{
			selModel->ShowBoundingShapes(((SUICheckBox*)com)->IsSelected());
		}
		else if(name == "Show Model")
		{
			selModel->ShowModel(((SUICheckBox*)com)->IsSelected());
		}
		else if( CheckLightBoxUI(com, selModel) )
		{

		}
	}

}


bool ModelPropsFrame::CheckLightBoxUI(SUIComponent* com, FLModel* selModel)
{
	if(com == NULL)
		return false;

	string name = com->GetName();

	if(name == "Shininess")
	{
		selModel->SetShininess( ((SUISlider*)com)->GetValue() );
		return true;
	}
	else if(com == _ambientR || com == _ambientG || com == _ambientB)
	{
		selModel->SetMeterial(GL_AMBIENT, _ambientR->GetValue(), _ambientG->GetValue(), _ambientB->GetValue(), 1.0f);
		return true;
	}
	else if(com == _diffuseR || com == _diffuseG || com == _diffuseB)
	{
		selModel->SetMeterial(GL_DIFFUSE, _diffuseR->GetValue(), _diffuseG->GetValue(), _diffuseB->GetValue(), 1.0f);
		return true;
	}
	else if(com == _specularR || com == _specularG || com == _specularB)
	{
		selModel->SetMeterial(GL_SPECULAR, _specularR->GetValue(), _specularG->GetValue(), _specularB->GetValue(), 1.0f);
		return true;
	}

	return false;
}


ModelPropsFrame::~ModelPropsFrame()
{
	if(_frame)
	{
		delete _frame;
		_frame = NULL;
	}
}
