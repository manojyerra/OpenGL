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

	box->AddCheckBox( _texureChkBox = new SUICheckBox("Texture", this) );
	box->AddCheckBox( _boundingBoxChkBox = new SUICheckBox("Bounding Box", this) );
	box->AddCheckBox( _bounding2DRectChkBox = new SUICheckBox("Bounding 2D Rect", this) );
	box->AddCheckBox( _showBoundingShapesChkBox = new SUICheckBox("Show Bounding Shapes", this) );
	box->AddCheckBox( _wireFrameLinesChkBox = new SUICheckBox("WireFrame Lines", this) );
	box->AddCheckBox( _wireFramePointsChkBox = new SUICheckBox("WireFrame Points", this) );
	box->AddCheckBox( _showModelChkBox = new SUICheckBox("Show Model", this) );
	
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
	boxLighting->AddCheckBox( _lightingChkBox = new SUICheckBox("Lighting On", this));

	boxLighting->AddBox(boxAmbient);
	boxLighting->AddBox(boxDiffuse);
	boxLighting->AddBox(boxSpecular);

	boxLighting->AddSlider( _shininessSlider = new SUISlider("Shininess", 0, 20, false, this) );

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


void ModelPropsFrame::SetUIValuesFromModel(FLModel* model)
{
	if(model)
	{
		_texureChkBox->SetSelect( model->IsTextureEnabled() );
		_boundingBoxChkBox->SetSelect( model->IsBoundingBoxEnabled() );
		_bounding2DRectChkBox->SetSelect( model->IsBounding2DRectEnabled() );
		_showBoundingShapesChkBox->SetSelect( model->IsShowingBoundingShapes() );
		_wireFrameLinesChkBox->SetSelect( model->IsWireFrameLinesEnabled() );
		_wireFramePointsChkBox->SetSelect( model->IsWireFramePointsEnabled() );
		_showModelChkBox->SetSelect( model->IsShowingModel() );
		
		_lightingChkBox->SetSelect( model->IsLightingEnabled() );
		_shininessSlider->SetValue( model->GetShininess() );

		unsigned int ambient = model->GetMeterial(GL_AMBIENT);
		unsigned int diffuse = model->GetMeterial(GL_DIFFUSE);
		unsigned int specular = model->GetMeterial(GL_SPECULAR);

		_ambientR->SetValue( (float)((ambient >> 24) & 255) / 255.0f );
		_ambientG->SetValue( (float)((ambient >> 16) & 255) / 255.0f );
		_ambientB->SetValue( (float)((ambient >> 8) & 255) / 255.0f );

		_diffuseR->SetValue( (float)((diffuse >> 24) & 255) / 255.0f );
		_diffuseG->SetValue( (float)((diffuse >> 16) & 255) / 255.0f );
		_diffuseB->SetValue( (float)((diffuse >> 8) & 255) / 255.0f );

		_specularR->SetValue( (float)((specular >> 24) & 255) / 255.0f );
		_specularG->SetValue( (float)((specular >> 16) & 255) / 255.0f );
		_specularB->SetValue( (float)((specular >> 8) & 255) / 255.0f );
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
