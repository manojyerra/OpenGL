#include "ModelsManager.h"

ModelsManager::ModelsManager()
{
	_vec.clear();
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

FLModel* ModelsManager::Get(int index)
{
	return _vec[index];
}

int ModelsManager::Size()
{
	return (int)_vec.size();
}

void ModelsManager::DrawForSelection()
{
	bool light = util::GLEnable(GL_LIGHTING, false);
	bool blend = util::GLEnable(GL_BLEND, false);
	bool depthTest = util::GLEnable(GL_DEPTH_TEST, true);
	unsigned int prevColor = util::GLColor(0x000000ff);

	for(unsigned int i=0; i<_vec.size();i++)
	{
		bool bounding2DRect = _vec[i]->IsBounding2DRectEnabled();
		bool boundingBox	= _vec[i]->IsBoundingBoxEnabled();
		bool textureEnable = _vec[i]->IsTextureEnabled();
		bool wireFrameEnable = _vec[i]->IsWireFrameEnabled();

		_vec[i]->SetBounding2DRectEnabled(false);
		_vec[i]->SetBoundingBoxEnabled(false);
		_vec[i]->SetTextureEnabled(false);
		_vec[i]->SetWireFrameEnabled(false);

		util::GLColor(i);

		_vec[i]->Draw();

		_vec[i]->SetBounding2DRectEnabled(bounding2DRect);
		_vec[i]->SetBoundingBoxEnabled(boundingBox);
		_vec[i]->SetTextureEnabled(textureEnable);
		_vec[i]->SetWireFrameEnabled(wireFrameEnable);
	}

	util::GLEnable(GL_LIGHTING, light);
	util::GLEnable(GL_BLEND, blend);
	util::GLEnable(GL_DEPTH_TEST, depthTest);
	util::GLColor(prevColor);
}

void ModelsManager::SetBoundingBoxEnabled(bool enable)
{
	for(int i=0; i<_vec.size();i++)
	{
		_vec[i]->SetBoundingBoxEnabled(enable);
	}
}

void ModelsManager::Draw()
{
	for(int i=0; i<_vec.size();i++)
	{
		_vec[i]->Draw();
	}
}

ModelsManager::~ModelsManager()
{
	for(int i=0; i<_vec.size();i++)
	{
		if(_vec[i])
		{
			delete _vec[i];
			_vec[i] = NULL;
		}
	}

	_vec.clear();
}
