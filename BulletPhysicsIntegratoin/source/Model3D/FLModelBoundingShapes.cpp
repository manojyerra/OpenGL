#include "FLModelBoundingShapes.h"
#include "CFileReader.h"
#include "ImageBuffer.h"
#include "Util/GLUtil.h"
#include "Cam.h"

FLModelBoundingShapes::FLModelBoundingShapes()
{
	_boundingShapes.clear();
}

FLModelBoundingShapes::~FLModelBoundingShapes()
{
	for(unsigned int i=0; i<_boundingShapes.size(); i++)
	{
		if(_boundingShapes[i])
		{
			delete _boundingShapes[i];
			_boundingShapes[i] = NULL;
		}
	}

	_boundingShapes.clear();
}


void FLModelBoundingShapes::AddBoundingShape(Shape* shape)
{
	_boundingShapes.push_back(shape);
}

Shape* FLModelBoundingShapes::AddBestBoudingShapeByVerticesOnRect(Rect* rect, float* verArr, int numVertex, float* mat)
{
	float x = rect->x;
	float y = rect->y;
	float w = rect->w;
	float h = rect->h;

	vector<float> verVec = GLUtil::GetVerticesOnRect(verArr, numVertex, mat, x, y, w, h);
	
	Shape* bShape = NULL;

	if(verVec.size() > 0)
	{
		bShape = Shape::GetBestFitBoundingShape(&verVec[0], verVec.size());

		_boundingShapes.push_back(bShape);
	}

	return bShape;
}

Shape* FLModelBoundingShapes::AddBoudingShapeByVerticesOnRect(Rect* rect, int boundingShapeID, float* verArr, int numVertex, float* mat)
{
	float x = rect->x;
	float y = rect->y;
	float w = rect->w;
	float h = rect->h;

	vector<float> verVec = GLUtil::GetVerticesOnRect(verArr, numVertex, mat, x, y, w, h);
	
	Shape* bShape = NULL;

	if(verVec.size() > 0)
	{
		bShape = Shape::GetBoundingShape(&verVec[0], verVec.size(), boundingShapeID);

		if(bShape)
			_boundingShapes.push_back(bShape);
	}

	return bShape;
}

Shape* FLModelBoundingShapes::Get(unsigned int index)
{
	return _boundingShapes[index];
}

unsigned int FLModelBoundingShapes::Size()
{
	return _boundingShapes.size();
}

void FLModelBoundingShapes::Draw()
{
	for(unsigned int i=0; i<_boundingShapes.size(); i++)
		_boundingShapes[i]->Draw();
}

