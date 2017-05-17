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
	_frame->Add( SetUpSaveBox() );
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
	SUIBox* box = new SUIBox(SUIBox::V_ALIGNMENT);
	box->SetName("Translation", SUIBox::LEFT);
	box->SetOn(false);
	box->SetOnOffEnable(true);
	box->SetMargin(5,5,5,5);

	SUIBox* box1 = new SUIBox(SUIBox::H_ALIGNMENT);

	SUIBox* tfBox = new SUIBox(SUIBox::V_ALIGNMENT);

	tfBox->AddTextField( _transXTF = new SUITextField("0", SUIBox::LEFT, this, SUITextField::INPUT_DOUBLE) );
	tfBox->AddTextField( _transYTF = new SUITextField("0", SUIBox::LEFT, this, SUITextField::INPUT_DOUBLE) );
	tfBox->AddTextField( _transZTF = new SUITextField("0", SUIBox::LEFT, this, SUITextField::INPUT_DOUBLE) );

	SUIBox* btnsBox = new SUIBox(SUIBox::V_ALIGNMENT);
	btnsBox->AddButton( _transXZeroBtn = new SUIButton("Set 0", SUIBox::LEFT, this) );
	btnsBox->AddButton( _transYZeroBtn = new SUIButton("Set 0", SUIBox::LEFT, this) );
	btnsBox->AddButton( _transZZeroBtn = new SUIButton("Set 0", SUIBox::LEFT, this) );

	SUIBox* lastBox = new SUIBox(SUIBox::H_ALIGNMENT);
	lastBox->AddButton( _copyTransBtn = new SUIButton("Copy Trans", SUIBox::LEFT, this) );
	lastBox->AddButton( _pasteTransBtn = new SUIButton("Paste Trans", SUIBox::LEFT, this) );

	box1->AddBox( tfBox );
	box1->AddBox( btnsBox );

	box->AddBox(box1);
	box->AddBox(lastBox);

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

		SUIBox* tfBox = new SUIBox(SUIBox::V_ALIGNMENT);
		tfBox->AddTextField( _rotXTF = new SUITextField("0", SUIBox::LEFT, this, SUITextField::INPUT_DOUBLE) );
		tfBox->AddTextField( _rotYTF = new SUITextField("0", SUIBox::LEFT, this, SUITextField::INPUT_DOUBLE) );
		tfBox->AddTextField( _rotZTF = new SUITextField("0", SUIBox::LEFT, this, SUITextField::INPUT_DOUBLE) );

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

	box1->AddBox( tfBox );
	box1->AddBox( boxL2 );
	box1->AddBox( boxL1 );
	box1->AddBox( boxR1 );
	box1->AddBox( boxR2 );
	
	box->AddBox( box1 );

	SUIBox* lastBox = new SUIBox(SUIBox::H_ALIGNMENT);
	lastBox->AddButton( _rotZeroBtn = new SUIButton("Set (0,0,0)", SUIBox::LEFT, this) );
	lastBox->AddButton( _copyRotBtn = new SUIButton("Copy Rot", SUIBox::CENTER, this) );
	lastBox->AddButton( _pasteRotBtn = new SUIButton("Paste Rot", SUIBox::CENTER, this) );
	
	box->AddBox(lastBox);
	return box;
}

SUIBox* ModelPropsFrame::SetUpSaveBox()
{
	SUIBox* box = new SUIBox(SUIBox::V_ALIGNMENT);
	box->SetName("Save Options", SUIBox::LEFT);
	box->SetOn(false);
	box->SetOnOffEnable(true);
	box->SetMargin(5,5,5,5);

	box->AddButton( _saveBoundingShapes = new SUIButton("Set Bounding Shapes", SUIBox::LEFT, this) );
	box->AddButton( _saveAABB = new SUIButton("Set AABB", SUIBox::LEFT, this) );
	box->AddButton( _saveTransformation = new SUIButton("Set Transformation", SUIBox::LEFT, this) );
	box->AddButton( _saveAll = new SUIButton("Save All", SUIBox::LEFT, this) );

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
		else if(com == _transXZeroBtn)
		{
			CVector3 pos = selModel->GetPos();
			selModel->SetPos(0.0f, pos.y, pos.z);
			_transXTF->SetDouble(0, 2);
		}
		else if(com == _transYZeroBtn)
		{
			CVector3 pos = selModel->GetPos();
			selModel->SetPos(pos.x, 0, pos.z);
			_transYTF->SetDouble(0, 2);
		}
		else if(com == _transZZeroBtn)
		{
			CVector3 pos = selModel->GetPos();
			selModel->SetPos(pos.x, pos.y, 0);
			_transZTF->SetDouble(0, 2);
		}
		else if(com == _transXTF)
		{
			SUITextField* textField = (SUITextField*)com;
			CVector3 pos = selModel->GetPos();
			selModel->SetPos(textField->GetDouble(), pos.y, pos.z);
		}
		else if(com == _transYTF)
		{
			SUITextField* textField = (SUITextField*)com;
			CVector3 pos = selModel->GetPos();
			selModel->SetPos(pos.x, textField->GetDouble(), pos.z);
		}
		else if(com == _transZTF)
		{
			SUITextField* textField = (SUITextField*)com;
			CVector3 pos = selModel->GetPos();
			selModel->SetPos(pos.x, pos.y, textField->GetDouble());
		}		
		else if(com == _rotXTF)
		{
			SUITextField* textField = (SUITextField*)com;
			CVector3 rot = selModel->GetRotation();
			selModel->SetRotation(CVector3(textField->GetDouble(), rot.y, rot.z));
		}
		else if(com == _rotYTF)
		{
			SUITextField* textField = (SUITextField*)com;
			CVector3 rot = selModel->GetRotation();
			selModel->SetRotation(CVector3(rot.x, textField->GetDouble(), rot.z));
		}
		else if(com == _rotZTF)
		{
			SUITextField* textField = (SUITextField*)com;
			CVector3 rot = selModel->GetRotation();
			selModel->SetRotation(CVector3(rot.x, rot.y, textField->GetDouble()));
		}

		else if(com == _rx1) { selModel->AddRotateInLocal('x', 1); UpdateRotationInfo(selModel); }
		else if(com == _ry1) { selModel->AddRotateInLocal('y', 1); UpdateRotationInfo(selModel); }
		else if(com == _rz1) { selModel->AddRotateInLocal('z', 1); UpdateRotationInfo(selModel); }

		else if(com == _rx2) { selModel->AddRotateInLocal('x', 10); UpdateRotationInfo(selModel); }
		else if(com == _ry2) { selModel->AddRotateInLocal('y', 10); UpdateRotationInfo(selModel); }
		else if(com == _rz2) { selModel->AddRotateInLocal('z', 10); UpdateRotationInfo(selModel); }

		else if(com == _lx1) { selModel->AddRotateInLocal('x', -1); UpdateRotationInfo(selModel); }
		else if(com == _ly1) { selModel->AddRotateInLocal('y', -1); UpdateRotationInfo(selModel); }
		else if(com == _lz1) { selModel->AddRotateInLocal('z', -1); UpdateRotationInfo(selModel); }

		else if(com == _lx2) { selModel->AddRotateInLocal('x', -10); UpdateRotationInfo(selModel); }
		else if(com == _ly2) { selModel->AddRotateInLocal('y', -10); UpdateRotationInfo(selModel); }
		else if(com == _lz2) { selModel->AddRotateInLocal('z', -10); UpdateRotationInfo(selModel); }

		else if(com == _rotZeroBtn)
		{
			selModel->SetRotation(CVector3(0,0,0));
			UpdateRotationInfo(selModel);
		}
		else if(com == _copyRotBtn)
		{
			_rotCopy = selModel->GetRotation();
			_copyType = ROT;

			ShowMessageBox(NULL, "Rotation values copied.", "", MESSAGE_OK);
		}
		else if(com == _pasteRotBtn)
		{
			if(_copyType == ROT)
			{
				selModel->SetRotation(_rotCopy);
				UpdateRotationInfo(selModel);
			}
		}
		else if(com == _copyTransBtn)
		{
			_transCopy = selModel->GetPos();
			_copyType = TRANS;

			ShowMessageBox(NULL, "Tranlation values copied.", "", MESSAGE_OK);
		}
		else if(com == _pasteTransBtn)
		{
			if(_copyType == TRANS)
			{
				selModel->SetPos(_transCopy);
				UpdateTransInfo(selModel);
			}
		}
		else if(com == _saveBoundingShapes)
		{
			selModel->WriteBoundingShapes();
			ShowMessageBox(NULL, "Saved Bounding Shapes", "", MESSAGE_OK);
		}
		else if(com == _saveAABB)
		{
			selModel->WriteAABB();
			ShowMessageBox(NULL, "Saved AABB", "", MESSAGE_OK);
		}
		else if(com == _saveTransformation)
		{
			selModel->WriteTranformation();
			ShowMessageBox(NULL, "Saved Transformation", "", MESSAGE_OK);
		}
		else if(com == _saveAll)
		{
			selModel->Write();
			ShowMessageBox(NULL, "Saved", "", MESSAGE_OK);
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

		_transXTF->SetDouble(0, 2);
		_transYTF->SetDouble(0, 2);
		_transZTF->SetDouble(0, 2);

		_rotXTF->SetDouble(0, 2);
		_rotYTF->SetDouble(0, 2);
		_rotYTF->SetDouble(0, 2);
	}
}

void ModelPropsFrame::UpdateTransInfo(FLModel* model)
{
	if(model)
	{
		_transXTF->SetDouble(model->GetPos().x, 2);
		_transYTF->SetDouble(model->GetPos().y, 2);
		_transZTF->SetDouble(model->GetPos().z, 2);
	}
}

void ModelPropsFrame::UpdateRotationInfo(FLModel* model)
{
	if(model)
	{
		CVector3 rot = model->GetRotation();

		_rotXTF->SetDouble(rot.x, 2);
		_rotYTF->SetDouble(rot.y, 2);
		_rotZTF->SetDouble(rot.z, 2);
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
