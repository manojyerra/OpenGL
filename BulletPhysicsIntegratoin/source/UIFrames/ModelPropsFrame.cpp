#include "ModelPropsFrame.h"
#include "SUI/SUIInput.h"
#include "SUI/SUILabel.h"
#include "SUI/SUIBox.h"

ModelPropsFrame::ModelPropsFrame(int x, int y, int w, int h, ModelsManager* modelsMgr)
{
	_modelsMgr = modelsMgr;
	_copyType = NONE;

	_frame = new SUIFrame((float)x, (float)y, (float)w, (float)h, SUIFrame::V_ALIGNMENT);
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
	box->AddCheckBox( _showLocalAxis = new SUICheckBox("Show Local Axis", this) );
	box->AddCheckBox( _markObject = new SUICheckBox("Mark Object", this) );

	_frame->Add(box);
	_frame->Add( SetUpLightingBox() );
	_frame->Add( SetUpTransBox() );
	_frame->Add( SetUpRotationBox() );

	//_frame->SetMinimized(true);
}


SUIBox* ModelPropsFrame::SetUpLightingBox()
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
	boxLighting->SetOn(false);
	boxLighting->AddCheckBox( _lightingChkBox = new SUICheckBox("Lighting On", this));

	boxLighting->AddBox(boxAmbient);
	boxLighting->AddBox(boxDiffuse);
	boxLighting->AddBox(boxSpecular);

	boxLighting->AddSlider( _shininessSlider = new SUISlider("Shininess", 0, 20, false, this) );

	return boxLighting;
}


SUIBox* ModelPropsFrame::SetUpTransBox()
{
	SUIBox* box = new SUIBox(SUIBox::H_ALIGNMENT);
	box->SetName("Translation", SUIBox::LEFT);
	box->SetOn(false);
	box->SetOnOffEnable(true);
	box->SetMargin(5,5,5,5);

	SUIBox* labelsBox = new SUIBox(SUIBox::V_ALIGNMENT);
	labelsBox->AddLabel( _transXLabel = new SUILabel("x : 0", SUIBox::LEFT) );
	labelsBox->AddLabel( _transYLabel = new SUILabel("y : 0", SUIBox::LEFT) );
	labelsBox->AddLabel( _transZLabel = new SUILabel("z : 0", SUIBox::LEFT) );

	SUIBox* btnsBox = new SUIBox(SUIBox::V_ALIGNMENT);
	btnsBox->AddButton( _transXBtn = new SUIButton("Set 0", SUIBox::LEFT, this) );
	btnsBox->AddButton( _transYBtn = new SUIButton("Set 0", SUIBox::LEFT, this) );
	btnsBox->AddButton( _transZBtn = new SUIButton("Set 0", SUIBox::LEFT, this) );

	box->AddBox( labelsBox );
	box->AddBox( btnsBox );

	return box;
}


SUIBox* ModelPropsFrame::SetUpRotationBox()
{
	SUIBox* box = new SUIBox(SUIBox::V_ALIGNMENT);
	box->SetName("Rotation", SUIBox::LEFT);
	box->SetOn(false);
	box->SetOnOffEnable(true);
	box->SetMargin(5,5,5,5);


	SUIBox* box1 = new SUIBox(SUIBox::H_ALIGNMENT);

		SUIBox* labelsBox = new SUIBox(SUIBox::V_ALIGNMENT);
		labelsBox->AddLabel( _rotXLabel = new SUILabel("x:0", SUIBox::LEFT) );
		labelsBox->AddLabel( _rotYLabel = new SUILabel("y:0", SUIBox::LEFT) );
		labelsBox->AddLabel( _rotZLabel = new SUILabel("z:0", SUIBox::LEFT) );

		SUIBox* boxR1 = new SUIBox(SUIBox::V_ALIGNMENT);
		boxR1->AddButton( _rx1 = new SUIButton(">", SUIBox::CENTER, this) );
		boxR1->AddButton( _ry1 = new SUIButton(">", SUIBox::CENTER, this) );
		boxR1->AddButton( _rz1 = new SUIButton(">", SUIBox::CENTER, this) );

		SUIBox* boxR2 = new SUIBox(SUIBox::V_ALIGNMENT);
		boxR2->AddButton( _rx2 = new SUIButton(">>", SUIBox::CENTER, this) );
		boxR2->AddButton( _ry2 = new SUIButton(">>", SUIBox::CENTER, this) );
		boxR2->AddButton( _rz2 = new SUIButton(">>", SUIBox::CENTER, this) );

		SUIBox* boxL1 = new SUIBox(SUIBox::V_ALIGNMENT);
		boxL1->AddButton( _lx1 = new SUIButton("<", SUIBox::CENTER, this) );
		boxL1->AddButton( _ly1 = new SUIButton("<", SUIBox::CENTER, this) );
		boxL1->AddButton( _lz1 = new SUIButton("<", SUIBox::CENTER, this) );

		SUIBox* boxL2 = new SUIBox(SUIBox::V_ALIGNMENT);
		boxL2->AddButton( _lx2 = new SUIButton("<<", SUIBox::CENTER, this) );
		boxL2->AddButton( _ly2 = new SUIButton("<<", SUIBox::CENTER, this) );
		boxL2->AddButton( _lz2 = new SUIButton("<<", SUIBox::CENTER, this) );

	box1->AddBox( labelsBox );
	box1->AddBox( boxR1 );
	box1->AddBox( boxR2 );
	box1->AddBox( boxL1 );
	box1->AddBox( boxL2 );
	
	box->AddBox( box1 );
	box->AddButton( _rotZeroBtn = new SUIButton("Set Rotation to (0,0,0)", SUIBox::LEFT, this) );
	box->AddButton( _copyRotBtn = new SUIButton("Copy Rotation", SUIBox::LEFT, this) );
	box->AddButton( _pasteRotBtn = new SUIButton("Paste Rotation", SUIBox::LEFT, this) );

	
	return box;
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
		else if(com == _showLocalAxis)
		{
			selModel->ShowLocalAxis(((SUICheckBox*)com)->IsSelected());
		}
		else if(com == _markObject)
		{
			selModel->SetMarked(((SUICheckBox*)com)->IsSelected());
		}
		else if(com == _transXBtn)
		{
			CVector3 pos = selModel->GetPos();
			selModel->SetPos(0.0f, pos.y, pos.z);
		}
		else if(com == _transYBtn)
		{
			CVector3 pos = selModel->GetPos();
			selModel->SetPos(pos.x, 0, pos.z);
		}
		else if(com == _transZBtn)
		{
			CVector3 pos = selModel->GetPos();
			selModel->SetPos(pos.x, pos.y, 0);
		}
		
		else if(com == _rx1) selModel->AddRotateInLocal('x', 1);
		else if(com == _ry1) selModel->AddRotateInLocal('y', 1);
		else if(com == _rz1) selModel->AddRotateInLocal('z', 1);

		else if(com == _rx2) selModel->AddRotateInLocal('x', 10);
		else if(com == _ry2) selModel->AddRotateInLocal('y', 10);
		else if(com == _rz2) selModel->AddRotateInLocal('z', 10);

		else if(com == _lx1) selModel->AddRotateInLocal('x', -1);
		else if(com == _ly1) selModel->AddRotateInLocal('y', -1);
		else if(com == _lz1) selModel->AddRotateInLocal('z', -1);

		else if(com == _lx2) selModel->AddRotateInLocal('x', -10);
		else if(com == _ly2) selModel->AddRotateInLocal('y', -10);
		else if(com == _lz2) selModel->AddRotateInLocal('z', -10);

		else if(com == _rotZeroBtn)
		{
			selModel->SetRotation(CVector3(0,0,0));
		}
		else if(com == _copyRotBtn)
		{
			_rotCopy = selModel->GetRotation();
			_copyType = COPY;

			ShowMessageBox(NULL, "Rotation values copied.", "", MESSAGE_OK);
		}
		else if(com == _pasteRotBtn)
		{
			if(_copyType == COPY)
			{
				selModel->SetRotation(_rotCopy);
				UpdateRotationInfo(selModel);
			}

			//selModel->Write();
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
		selModel->SetShininess( (float)((SUISlider*)com)->GetValue() );
		return true;
	}
	else if(com == _ambientR || com == _ambientG || com == _ambientB)
	{
		selModel->SetMeterial(GL_AMBIENT, (float)_ambientR->GetValue(), (float)_ambientG->GetValue(), (float)_ambientB->GetValue(), 1.0f);
		return true;
	}
	else if(com == _diffuseR || com == _diffuseG || com == _diffuseB)
	{
		selModel->SetMeterial(GL_DIFFUSE, (float)_diffuseR->GetValue(), (float)_diffuseG->GetValue(), (float)_diffuseB->GetValue(), 1.0f);
		return true;
	}
	else if(com == _specularR || com == _specularG || com == _specularB)
	{
		selModel->SetMeterial(GL_SPECULAR, (float)_specularR->GetValue(), (float)_specularG->GetValue(), (float)_specularB->GetValue(), 1.0f);
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
		_markObject->SetSelect( model->IsMarked() );
		_showLocalAxis->SetSelect( model->IsShowingLocalAxis() );

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

		UpdateTransInfo(model);
		UpdateRotationInfo(model);
	}
	else
	{
		_texureChkBox->SetSelect( false );
		_boundingBoxChkBox->SetSelect( false );
		_bounding2DRectChkBox->SetSelect( false );
		_showBoundingShapesChkBox->SetSelect( false );
		_wireFrameLinesChkBox->SetSelect( false );
		_wireFramePointsChkBox->SetSelect( false );
		_showModelChkBox->SetSelect( false );
		
		_lightingChkBox->SetSelect( false );
		_shininessSlider->SetValue( 0.0 );

		_ambientR->SetValue( 0.0 );
		_ambientG->SetValue( 0.0 );
		_ambientB->SetValue( 0.0 );

		_diffuseR->SetValue( 0.0 );
		_diffuseG->SetValue( 0.0 );
		_diffuseB->SetValue( 0.0 );

		_specularR->SetValue( 0.0 );
		_specularG->SetValue( 0.0 );
		_specularB->SetValue( 0.0 );

		_transXLabel->SetName("x : 0", SUIBox::LEFT);
		_transYLabel->SetName("y : 0", SUIBox::LEFT);
		_transZLabel->SetName("z : 0", SUIBox::LEFT);

		_rotXLabel->SetName("x:0", SUIBox::LEFT);
		_rotYLabel->SetName("y:0", SUIBox::LEFT);
		_rotYLabel->SetName("z:0", SUIBox::LEFT);
	}
}

void ModelPropsFrame::UpdateTransInfo(FLModel* model)
{
	if(model)
	{
		char chArr[128];

		sprintf(chArr, "x : %.2f", model->GetPos().x);	_transXLabel->SetName(chArr);
		sprintf(chArr, "y : %.2f", model->GetPos().y);	_transYLabel->SetName(chArr);
		sprintf(chArr, "z : %.2f", model->GetPos().z);	_transZLabel->SetName(chArr);
	}
}

void ModelPropsFrame::UpdateRotationInfo(FLModel* model)
{
	if(model)
	{
		char chArr[128];
		
		CVector3 rot = model->GetRotation();

		sprintf(chArr, "x:%.1f", rot.x);	_rotXLabel->SetName(chArr);
		sprintf(chArr, "y:%.1f", rot.y);	_rotYLabel->SetName(chArr);
		sprintf(chArr, "z:%.1f", rot.z);	_rotZLabel->SetName(chArr);
	}
}

void ModelPropsFrame::ShowBoundingShapes(bool show)
{
	_showBoundingShapesChkBox->SetSelect( true );
}

ModelPropsFrame::~ModelPropsFrame()
{
	if(_frame)
	{
		delete _frame;
		_frame = NULL;
	}
}
