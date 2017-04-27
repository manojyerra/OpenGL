#include "ModelPropsFrame.h"
#include "SUI/SUIInput.h"
#include "SUI/SUILabel.h"
#include "SUI/SUIBox.h"
//#include "Util/UtilFuncs.h"
//#include "Cam.h"

ModelPropsFrame::ModelPropsFrame(int x, int y, int w, int h, SUIActionListener* ediroActionListener)
{
	//_model3D = NULL;

	_frame = new SUIFrame(x, y, w, h, SUIFrame::V_ALIGNMENT);
	_frame->SetName("Model Properties", SUIFrame::LEFT);

	//Save,Duplicate,Delete,WireFrame,Focus...
	SUIBox* box = new SUIBox(SUIBox::V_ALIGNMENT);
	box->SetMargin(0,0,10,10);
	box->AddButton(new SUIButton("Save", ediroActionListener));
	box->AddButton(new SUIButton("Duplicate", ediroActionListener));
	box->AddButton(new SUIButton("Delete", ediroActionListener));
	box->AddButton(new SUIButton("WireFrame On/Off", ediroActionListener));
	box->AddButton(new SUIButton("Focus On/Off", ediroActionListener));
	_frame->Add(box);


	//Mass and Meterial...
	massLabel = new SUILabel("Mass = 0", SUILabel::CENTER);
	_frame->Add(massLabel);


	//Texture...
	SUIBox* texBox = new SUIBox(SUIBox::H_ALIGNMENT);

	texPathLabel = new SUILabel("", SUILabel::LEFT);
	texBrowse = new SUIButton("Browse Texture", this);

	texBox->AddLabel(texPathLabel);
	texBox->AddButton(texBrowse);

	_frame->Add(texBox);


	//Bounding Box...
	SUIBox* boundingBox = new SUIBox(SUIBox::V_ALIGNMENT);
	boundingBox->SetMargin(0,0,10,10);
	boundingBox->SetName("Bounding Shape", SUIBox::CENTER);

	boundingBox->AddButton(new SUIButton("Best", this));
	boundingBox->AddButton(new SUIButton("Box", this));
	boundingBox->AddButton(new SUIButton("Cylinder", this));
	boundingBox->AddButton(new SUIButton("Sphere", this));
	boundingBox->AddButton(new SUIButton("Cone", this));

	boundingBox->SetOnOffEnable(true);
	boundingBox->SetOn(false);
	_frame->Add(boundingBox);


	_frame->Add(new SUILabel("Translation", SUILabel::CENTER));

	SUIBox* box1 = new SUIBox(SUIBox::H_ALIGNMENT);
	box1->AddLabel(new SUILabel("X", SUILabel::CENTER));
	box1->AddButton(_transXBtn = new SUIButton("0", SUILabel::CENTER));

	SUIBox* box2 = new SUIBox(SUIBox::H_ALIGNMENT);
	box2->AddLabel(new SUILabel("Y", SUILabel::CENTER));
	box2->AddButton(_transYBtn = new SUIButton("0", SUILabel::CENTER));

	SUIBox* box3 = new SUIBox(SUIBox::H_ALIGNMENT);
	box3->AddLabel(new SUILabel("Z", SUILabel::CENTER));
	box3->AddButton(_transZBtn = new SUIButton("0", SUILabel::CENTER));

	_frame->Add(box1);
	_frame->Add(box2);
	_frame->Add(box3);

	SUIButton* resetTrans = new SUIButton("Reset Translation", this);
	_frame->Add(resetTrans);

	_frame->Add(new SUILabel("Rotation", SUILabel::CENTER));

	SUIBox* box4 = new SUIBox(SUIBox::H_ALIGNMENT);
	box4->AddLabel(new SUILabel("X", SUILabel::CENTER));
	box4->AddButton(_rotXBtn = new SUIButton("0", SUILabel::CENTER));

	SUIBox* box5 = new SUIBox(SUIBox::H_ALIGNMENT);
	box5->AddLabel(new SUILabel("Y", SUILabel::CENTER));
	box5->AddButton(_rotYBtn = new SUIButton("0", SUILabel::CENTER));

	SUIBox* box6 = new SUIBox(SUIBox::H_ALIGNMENT);
	box6->AddLabel(new SUILabel("Z", SUILabel::CENTER));
	box6->AddButton(_rotZBtn = new SUIButton("0", SUILabel::CENTER));

	_frame->Add(box4);
	_frame->Add(box5);
	_frame->Add(box6);

	SUIButton* resetRot = new SUIButton("Reset Rotation", this);
	_frame->Add(resetRot);

	_frame->Add(new SUILabel("Scale", SUILabel::CENTER));

	SUIBox* box7 = new SUIBox(SUIBox::H_ALIGNMENT);
	box7->AddLabel(new SUILabel("X", SUILabel::CENTER));
	box7->AddButton(_scaleXBtn = new SUIButton("0", SUILabel::CENTER));

	SUIBox* box8 = new SUIBox(SUIBox::H_ALIGNMENT);
	box8->AddLabel(new SUILabel("Y", SUILabel::CENTER));
	box8->AddButton(_scaleYBtn = new SUIButton("0", SUILabel::CENTER));

	SUIBox* box9 = new SUIBox(SUIBox::H_ALIGNMENT);
	box9->AddLabel(new SUILabel("Z", SUILabel::CENTER));
	box9->AddButton(_scaleZBtn = new SUIButton("0", SUILabel::CENTER));

	_frame->Add(box7);
	_frame->Add(box8);
	_frame->Add(box9);
}

void ModelPropsFrame::AddMeterials(SUIChoice* meterialChoice)
{
	meterialChoice->Add("Select");
	meterialChoice->Add("Plastic");
	meterialChoice->Add("Plaster");
	meterialChoice->Add("Snow");
	meterialChoice->Add("Cork");
	meterialChoice->Add("Lime");
	meterialChoice->Add("Plywood");
	meterialChoice->Add("Books");
	meterialChoice->Add("Coal");
	meterialChoice->Add("Concrete"); 
	meterialChoice->Add("Cement");
	meterialChoice->Add("Sand");
	meterialChoice->Add("Soils");
	meterialChoice->Add("Glass");
	meterialChoice->Add("Mud");
	meterialChoice->Add("Magnesium");
	meterialChoice->Add("Stone");
	meterialChoice->Add("Brick");
	meterialChoice->Add("Aluminium");
	meterialChoice->Add("Granite");
	meterialChoice->Add("Iron");
	meterialChoice->Add("Zinc");
	meterialChoice->Add("Steel");
	meterialChoice->Add("Bronze");
	meterialChoice->Add("Brass");
	meterialChoice->Add("Copper");
	meterialChoice->Add("Lead");
	meterialChoice->Add("Mercury");
	meterialChoice->Add("Gold");
}

//bool ModelPropsFrame::AddBoundingShape(string name, Model3D* model3D)
//{
//	if(model3D == NULL)
//		return false;
//
//	if(	(name.compare("Best") == 0 || name.compare("Box") == 0 || name.compare("Sphere") == 0 || 
//		name.compare("Cylinder") == 0 || name.compare("Cone") == 0) == false)
//		return false;
//
//	const FLMStruct* bin = model3D->GetFLMStruct();
//	BoundingBox _bBox;
//	BoundingCylinder _bCylinder;
//	BoundingSphere _bSphere;
//	BoundingCone _bCone;
//
//	bool forSelection = true;
//	if((int)_rect.w == 0 || (int)_rect.h == 0)
//		forSelection = false;
//
//	
//	vector<int> selIndexVec;
//	float* selVertex = NULL;
//	int arrSize = 0;
//	
//	if(forSelection)
//	{
//		float* ver2D =	new float[bin->numVertex*2];
//
//		Cam* cam = Cam::GetInstance();
//		cam->GetPosOnScreen(bin->vertexBuf, bin->numVertex, model3D->GetOrientation().Get(), ver2D);
//
//		for(int i=0;i<bin->numVertex; i++)
//			if(_rect.Contains( ver2D[2*i+0], ver2D[2*i+1]))
//				selIndexVec.push_back(i);
//
//		delete[] ver2D;
//		ver2D = NULL;
//
//		arrSize = selIndexVec.size() * 3;
//		selVertex = new float[arrSize];
//		memset(selVertex, 0, arrSize*4);
//
//		int count = 0;
//		int numSelVertex = selIndexVec.size();
//		for(int i=0; i<numSelVertex; i++)
//		{
//			int index = selIndexVec[i];
//			selVertex[count+0] = bin->vertexBuf[3*index + 0];
//			selVertex[count+1] = bin->vertexBuf[3*index + 1];
//			selVertex[count+2] = bin->vertexBuf[3*index + 2];
//			count += 3;
//		}
//
//		selIndexVec.clear();
//	}
//
//	if(name.compare("Best") == 0)
//	{
//		if(forSelection)
//		{
//			_bBox.CalcBoundingBox(selVertex, arrSize);
//			_bCylinder.CalcBoundingCylinder(selVertex, arrSize, _bBox.GetMat(), _bBox.GetScale());
//			_bSphere.CalcBoundingSphere(selVertex, arrSize);
//			_bCone.CalcBoundingCone(selVertex, arrSize, _bCylinder.GetMat(), _bCylinder.GetHeight());
//		}
//		else
//		{
//			_bBox.CalcBoundingBox(bin->vertexBuf, bin->numVertex * 3);
//			_bCylinder.CalcBoundingCylinder(bin->vertexBuf, bin->numVertex*3, _bBox.GetMat(), _bBox.GetScale());
//			_bSphere.CalcBoundingSphere(bin->vertexBuf, bin->numVertex*3);
//			_bCone.CalcBoundingCone(bin->vertexBuf, bin->numVertex*3, _bCylinder.GetMat(), _bCylinder.GetHeight());
//
//			model3D->RemoveAllBoundingShapes();
//		}
//
//		float boxVol = _bBox.GetVolume();
//		float cylVol = _bCylinder.GetVolume();
//		float sphereVol = _bSphere.GetVolume();
//		float coneVol = _bCone.GetVolume();
//
//		if(boxVol <= cylVol && boxVol <= sphereVol && boxVol <= coneVol)
//			model3D->AddBoundingShape(Shape::BOX, _bBox.GetMat(), _bBox.GetScale());
//		else if(cylVol <= boxVol && cylVol <= sphereVol && cylVol <= coneVol)
//			model3D->AddBoundingShape(Shape::CYLINDER, _bCylinder.GetMat(), _bCylinder.GetScale());
//		else if(coneVol <= boxVol && coneVol <= sphereVol && coneVol <= cylVol)
//			model3D->AddBoundingShape(Shape::CONE, _bCone.GetMat(), _bCone.GetScale());
//		else if(sphereVol <= boxVol && sphereVol <= coneVol && sphereVol <= cylVol)
//			model3D->AddBoundingShape(Shape::SPHERE, _bSphere.GetMat(), _bSphere.GetScale());
//	}
//	else if(name.compare("Box") == 0)
//	{
//		if(forSelection)
//			_bBox.CalcBoundingBox(selVertex, arrSize);
//		else
//		{
//			_bBox.CalcBoundingBox(bin->vertexBuf, bin->numVertex * 3);
//			model3D->RemoveAllBoundingShapes();
//		}
//
//		model3D->AddBoundingShape(Shape::BOX, _bBox.GetMat(), _bBox.GetScale());
//	}
//	else if(name.compare("Sphere") == 0)
//	{
//		if(forSelection)
//			_bSphere.CalcBoundingSphere(selVertex, arrSize);
//		else
//		{
//			_bSphere.CalcBoundingSphere(bin->vertexBuf, bin->numVertex*3);
//			model3D->RemoveAllBoundingShapes();
//		}
//
//		model3D->AddBoundingShape(Shape::SPHERE, _bSphere.GetMat(), _bSphere.GetScale());
//	}
//	else if(name.compare("Cylinder") == 0)
//	{
//		if(forSelection)
//		{
//			_bBox.CalcBoundingBox(selVertex, arrSize);
//			_bCylinder.CalcBoundingCylinder(selVertex, arrSize, _bBox.GetMat(), _bBox.GetScale());
//		}
//		else
//		{
//			_bBox.CalcBoundingBox(bin->vertexBuf, bin->numVertex * 3);
//			_bCylinder.CalcBoundingCylinder(bin->vertexBuf, bin->numVertex*3, _bBox.GetMat(), _bBox.GetScale());
//			model3D->RemoveAllBoundingShapes();
//		}
//
//		model3D->AddBoundingShape(Shape::CYLINDER, _bCylinder.GetMat(), _bCylinder.GetScale());
//	}
//	else if(name.compare("Cone") == 0)
//	{
//		if(forSelection)
//		{
//			_bBox.CalcBoundingBox(selVertex, arrSize);
//			_bCylinder.CalcBoundingCylinder(selVertex, arrSize, _bBox.GetMat(), _bBox.GetScale());
//			_bSphere.CalcBoundingSphere(selVertex, arrSize);
//			_bCone.CalcBoundingCone(selVertex, arrSize, _bCylinder.GetMat(), _bCylinder.GetHeight());
//		}
//		else
//		{
//			_bBox.CalcBoundingBox(bin->vertexBuf, bin->numVertex * 3);
//			_bCylinder.CalcBoundingCylinder(bin->vertexBuf, bin->numVertex*3, _bBox.GetMat(), _bBox.GetScale());
//			_bSphere.CalcBoundingSphere(bin->vertexBuf, bin->numVertex*3);
//			_bCone.CalcBoundingCone(bin->vertexBuf, bin->numVertex*3, _bCylinder.GetMat(), _bCylinder.GetHeight());
//			model3D->RemoveAllBoundingShapes();
//		}
//
//		model3D->AddBoundingShape(Shape::CONE, _bCone.GetMat(), _bCone.GetScale());
//	}
//
//	if(selVertex)
//	{
//		delete[] selVertex;
//		selVertex = NULL;
//	}
//
//	return true;
//}

void ModelPropsFrame::SetTrans(float x, float y, float z)
{
	char arr[128];

	sprintf(arr, "%.3f", x);	_transXBtn->SetName(arr, SUIButton::CENTER);
	sprintf(arr, "%.3f", y);	_transYBtn->SetName(arr, SUIButton::CENTER);
	sprintf(arr, "%.3f", z);	_transZBtn->SetName(arr, SUIButton::CENTER);
}

void ModelPropsFrame::SetRotation(float x, float y, float z)
{
	char arr[128];

	sprintf(arr, "%.3f", x);	_rotXBtn->SetName(arr, SUIButton::CENTER);
	sprintf(arr, "%.3f", y);	_rotYBtn->SetName(arr, SUIButton::CENTER);
	sprintf(arr, "%.3f", z);	_rotZBtn->SetName(arr, SUIButton::CENTER);
}

void ModelPropsFrame::SetScale(float x, float y, float z)
{
	char arr[128];

	sprintf(arr, "%.3f", x);	_scaleXBtn->SetName(arr, SUIButton::CENTER);
	sprintf(arr, "%.3f", y);	_scaleYBtn->SetName(arr, SUIButton::CENTER);
	sprintf(arr, "%.3f", z);	_scaleZBtn->SetName(arr, SUIButton::CENTER);
}

void ModelPropsFrame::Update()
{
}

//void ModelPropsFrame::UpdateModelProps(Model3D* model3D, Rect rect)
//{
//	_model3D = model3D;
//	_rect = rect;
//
//	Point pos = _model3D->GetPos();
//	Point scale = _model3D->GetScale();
//	Point rot = _model3D->GetRotXYZ();
//
//	SetTrans( pos.x, pos.y, pos.z);
//	SetScale( scale.x,scale.y, scale.z);
//	SetRotation( rot.x,rot.y, rot.z);
//
//	texPathLabel->SetName(_model3D->GetTexturePath(), SUIButton::LEFT);
//
//	char arr[256];
//	if(_model3D->GetMeterialID() == -1)	sprintf(arr,"Mass = %.2f", _model3D->GetMass());
//	else								
//		sprintf(arr,"%s, Mass = %.2f", _model3D->GetMeterialName().c_str(), _model3D->GetMass());
//
//	massLabel->SetName(arr,SUIChoice::CENTER);
//}

void ModelPropsFrame::actionPerformed(SUIActionEvent e)
{
	//SUIComponent* com = (SUIComponent*)e.GetComponent();
	//string name = com->GetName();

	//if(com == texBrowse)
	//{
	//	if(_model3D)
	//	{
	//		vector<string> strVec = UtilFuncs::OpenFileDialog("Images (*.png;*.jpg;*.dds)\0*.png;*.jpg;*.dds\0", false);

	//		string str = strVec[0];
	//		if(str.length() > 0)
	//		{
	//			if(str[str.length()-1] == '\\')
	//				str = str.substr(0, str.length()-1);

	//			if(	StringUtils::endsWith(str, ".png") || 
	//				StringUtils::endsWith(str, ".jpg") || 
	//				StringUtils::endsWith(str, ".dds"))
	//			{
	//				int index = str.find("data\\");
	//				str = str.substr(index, str.length() - index);
	//				texPathLabel->SetName(str, SUIButton::CENTER);

	//				if(texPathLabel->GetName().length() > 0)
	//					_model3D->SetTexture(texPathLabel->GetName());
	//			}
	//		}
	//	}
	//}
	//else if(name.compare("Reset Translation") == 0)
	//{
	//	if(_model3D)
	//		_model3D->SetPos(0,0,0);
	//}
	//else if(name.compare("Reset Rotation") == 0)
	//{
	//	if(_model3D)
	//		_model3D->SetRotXYZ(0,0,0);
	//}
	//else if(AddBoundingShape(name, _model3D))
	//{
	//}
}

void ModelPropsFrame::Draw()
{

}

ModelPropsFrame::~ModelPropsFrame()
{
	if(_frame)
	{
		delete _frame;
		_frame = NULL;
	}
}
