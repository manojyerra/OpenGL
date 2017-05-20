#include "ModelsManager.h"

ModelsManager::ModelsManager()
{
	_vec.clear();
	_selModel = NULL;
	_showOnlySelObject = false;
}

FLModel* ModelsManager::Add(string folderPath)
{
	FLModel* model = new FLModel(folderPath);
	_vec.push_back(model);

	return model;
}

FLModel* ModelsManager::Add(string folderPath, float xPos, float yPos, float zPos)
{
	FLModel* model = new FLModel(folderPath, xPos, yPos, zPos);
	_vec.push_back(model);

	return model;
}

FLModel* ModelsManager::Add(string folderPath, CVector3 pos)
{
	FLModel* model = new FLModel(folderPath, pos);
	_vec.push_back(model);

	return model;
}

FLModel* ModelsManager::Add(string folderPath, CVector3 pos, CVector3 rot)
{
	FLModel* model = new FLModel(folderPath, pos, rot);
	_vec.push_back(model);

	return model;
}

FLModel* ModelsManager::Add(string folderPath, CVector3 pos, CVector3 rot, string rotOrder)
{
	FLModel* model = new FLModel(folderPath, pos, rot, rotOrder);
	_vec.push_back(model);

	return model;
}

FLModel* ModelsManager::Add(string folderPath, float* mat)
{
	FLModel* model = new FLModel(folderPath, mat);
	_vec.push_back(model);

	return model;
}

void ModelsManager::ShowMarkedObjects(bool show)
{
	for(unsigned int i=0; i<_vec.size();i++)
	{
		if(_vec[i]->IsMarked())
			_vec[i]->ShowModel(show);
	}
}

void ModelsManager::ShowUnmarkedObjects(bool show)
{
	for(unsigned int i=0; i<_vec.size();i++)
	{
		if(!_vec[i]->IsMarked())
			_vec[i]->ShowModel(show);
	}
}

void ModelsManager::MarkAllObjects()
{
	for(unsigned int i=0; i<_vec.size();i++)
		_vec[i]->SetMarked(true);
}

void ModelsManager::UnmarkAllObjects()
{
	for(unsigned int i=0; i<_vec.size();i++)
		_vec[i]->SetMarked(false);
}


void ModelsManager::ShowBoundingShapes(bool show)
{
	for(unsigned int i=0; i<_vec.size();i++)
		_vec[i]->ShowBoundingShapes(show);
}

void ModelsManager::SetBoundingBoxEnabled(bool enable)
{
	for(unsigned int i=0; i<_vec.size();i++)
		_vec[i]->SetBoundingBoxEnabled(enable);
}

void ModelsManager::SetAsPhysicsObjects(bool val)
{
	for(unsigned int i=0; i<_vec.size();i++)
		_vec[i]->SetAsPhysicsObject(val);
}


void ModelsManager::ShowOnlySelectedObject(bool show)
{
	_showOnlySelObject = show;
}

bool ModelsManager::IsShowingOnlySelectedObject()
{
	return _showOnlySelObject;
}

FLModel* ModelsManager::Get(unsigned int index)
{
	return _vec[index];
}

FLModel* ModelsManager::GetSelectedModel()
{
	return _selModel;
}

void ModelsManager::SetSelectedModelIndex(int index)
{
	if(index >= 0 && index < (int)Size())
	{
		_selModel = _vec[index];
	}
	else
	{
		_selModel = NULL;
	}
}

int ModelsManager::GetModelIndexByMousePos(float x, float y)
{
	DrawForSelection();
		
	GLubyte data[4];
	glReadPixels((GLint)x, GLUtil::GetWindowHeight()-(GLint)y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, data);

	unsigned int colorVal = (unsigned int)((data[0]<<24) + (data[1]<<16) + (data[2]<<8) + data[3]);

	if(colorVal < Size())
		return colorVal;

	return -1;
}

unsigned int ModelsManager::Size()
{
	return _vec.size();
}

void ModelsManager::DrawForSelection()
{
	GLboolean light = GLUtil::GLEnable(GL_LIGHTING, false);
	GLboolean blend = GLUtil::GLEnable(GL_BLEND, false);
	GLboolean depthTest = GLUtil::GLEnable(GL_DEPTH_TEST, true);
	unsigned int prevColor = GLUtil::GLColor(0x000000ff);

	for(unsigned int i=0; i<_vec.size();i++)
	{
		bool bounding2DRect			= _vec[i]->IsBounding2DRectEnabled();
		bool boundingBox			= _vec[i]->IsBoundingBoxEnabled();
		bool boundingShapes			= _vec[i]->IsShowingBoundingShapes();
		bool textureEnable			= _vec[i]->IsTextureEnabled();
		bool wireFrameLinesEnable	= _vec[i]->IsWireFrameLinesEnabled();
		bool wireFramePointsEnable	= _vec[i]->IsWireFramePointsEnabled();
		bool lightingOn				= _vec[i]->IsLightingEnabled();

		_vec[i]->SetBounding2DRectEnabled(false);
		_vec[i]->SetBoundingBoxEnabled(false);
		_vec[i]->SetTextureEnabled(false);
		_vec[i]->SetWireFrameLinesEnabled(false);
		_vec[i]->SetWireFramePointsEnabled(false);
		_vec[i]->ShowBoundingShapes(false);
		_vec[i]->SetLightingEnabled(false);

		GLUtil::GLColor(i);

		_vec[i]->Draw();

		_vec[i]->SetBounding2DRectEnabled(bounding2DRect);
		_vec[i]->SetBoundingBoxEnabled(boundingBox);
		_vec[i]->SetTextureEnabled(textureEnable);
		_vec[i]->SetWireFrameLinesEnabled(wireFrameLinesEnable);
		_vec[i]->SetWireFramePointsEnabled(wireFramePointsEnable);
		_vec[i]->ShowBoundingShapes(boundingShapes);
		_vec[i]->SetLightingEnabled(lightingOn);
	}

	GLUtil::GLEnable(GL_LIGHTING, light);
	GLUtil::GLEnable(GL_BLEND, blend);
	GLUtil::GLEnable(GL_DEPTH_TEST, depthTest);
	GLUtil::GLColor(prevColor);
}

void ModelsManager::Draw()
{
	if(!_showOnlySelObject)
	{
		for(unsigned int i=0; i<_vec.size();i++)
			_vec[i]->Draw();
	}
	else
	{
		if(_selModel)
			_selModel->Draw();
	}

	if(_selModel)
		_selModel->DrawBorder();
}

ModelsManager::~ModelsManager()
{
	for(unsigned int i=0; i<_vec.size();i++)
	{
		if(_vec[i])
		{
			delete _vec[i];
			_vec[i] = NULL;
		}
	}

	_vec.clear();
}
