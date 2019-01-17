#ifndef Pointers_H
#define Pointers_H

#include "Model3D/Model3D.h"
#include "MathLib/Vector3.h"
#include "Cam.h"

class Pointers
{
private:
	Model3D* _model;

	float _anchorW;
	int _activePointer;
	CVector3 _activeVec;
	CVector3 _activeCenter;

	float _moveLen;
	int _anchorType;
	
	void UpdatePos(Cam* cam);

public:
	static const int NONE = 0;
	static const int X_POINTER = 1;
	static const int Y_POINTER = 2;
	static const int Z_POINTER = 3;

	static const int ANCHOR_RECT = 1;
	static const int ANCHOR_CIRCLE = 2;

	CVector3 center;
	CVector3 leftVec;
	CVector3 upVec;
	CVector3 forwardVec;

	Pointers();
	~Pointers();
	
	void Update(Cam* cam);
	void MoveModelOnMouseInput(Model3D* model);

	float GetMoveLen();
	void SetModel(Model3D* model);

	bool IsXActive();
	bool IsYActive();
	bool IsZActive();

	void SetAnchorsAsCircles();
	void SetAnchorsAsRect();
	int GetAnchorType();
	void ChangeAnchorType();

	int GetActiveAxis();
	void Draw(float windowW, float windowH);
};

#endif