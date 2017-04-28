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

FLModel* ModelsManager::GetSelectedModel()
{
	return _vec[0];
}


int ModelsManager::Size()
{
	return (int)_vec.size();
}

void ModelsManager::DrawForSelection()
{
	GLboolean light = glUtil::GLEnable(GL_LIGHTING, false);
	GLboolean blend = glUtil::GLEnable(GL_BLEND, false);
	GLboolean depthTest = glUtil::GLEnable(GL_DEPTH_TEST, true);
	unsigned int prevColor = glUtil::GLColor(0x000000ff);

	for(unsigned int i=0; i<_vec.size();i++)
	{
		bool bounding2DRect = _vec[i]->IsBounding2DRectEnabled();
		bool boundingBox	= _vec[i]->IsBoundingBoxEnabled();
		bool textureEnable = _vec[i]->IsTextureEnabled();
		bool wireFrameLinesEnable = _vec[i]->IsWireFrameLinesEnabled();
		bool wireFramePointsEnable = _vec[i]->IsWireFramePointsEnabled();

		_vec[i]->SetBounding2DRectEnabled(false);
		_vec[i]->SetBoundingBoxEnabled(false);
		_vec[i]->SetTextureEnabled(false);
		_vec[i]->SetWireFrameLinesEnabled(false);
		_vec[i]->SetWireFramePointsEnabled(false);

		glUtil::GLColor(i);

		_vec[i]->Draw();

		_vec[i]->SetBounding2DRectEnabled(bounding2DRect);
		_vec[i]->SetBoundingBoxEnabled(boundingBox);
		_vec[i]->SetTextureEnabled(textureEnable);
		_vec[i]->SetWireFrameLinesEnabled(wireFrameLinesEnable);
		_vec[i]->SetWireFramePointsEnabled(wireFramePointsEnable);
	}

	glUtil::GLEnable(GL_LIGHTING, light);
	glUtil::GLEnable(GL_BLEND, blend);
	glUtil::GLEnable(GL_DEPTH_TEST, depthTest);
	glUtil::GLColor(prevColor);
}

void ModelsManager::SetBoundingBoxEnabled(bool enable)
{
	for(unsigned int i=0; i<_vec.size();i++)
	{
		_vec[i]->SetBoundingBoxEnabled(enable);
	}
}

void ModelsManager::Draw()
{
	for(unsigned int i=0; i<_vec.size();i++)
	{
		_vec[i]->Draw();
	}
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
