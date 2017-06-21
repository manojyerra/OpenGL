#ifndef GAMELOOP_H
#define GAMELOOP_H
#pragma once

#include "GameInit.h"
#include "PhysicsLib/PhyPoly.h"
#include "PhysicsLib/PhyCircle.h"
#include "MathLib/Poly2D.h"
#include "Primitives/Prim.h"
#include "Box2D/Box2D.h"
#include "UI/MyWindow.h"
#include "physicslib/PhyCompoundShape.h"
#include <cstdio>
#include <vector>
#include <string>
using namespace std;

class GameLoop
{
private:
	GameInit* _gameInit;

	vector<Prim*> _primVec;
	Prim* _copyPrim;
	Prim* _activePrim;
	bool _activatePhysics;
	int _compoundShapeID;
	bool _dataModified;
	string _currFilePath;

	MyWindow* _myWindow;
	MyButton* _physicsModeBtn;
	MyButton* _editModeBtn;
	MyButton* _compoundModeBtn;

	float _orthoX;
	float _orthoY;
	float _orthoW;
	float _orthoH;

	//b2RevoluteJointDef revJointDef;
	//PhyPoly* poly;
	//PhyCircle* smallCircle;
	//PhyCircle* circle;

	////b2DistanceJointDef distJointDef;

public:
	
	GameLoop();
	~GameLoop();

	void Run(float deltaTime);
	void Update(float deltaTime);
	void AddPrim(Prim* prim);
	void Draw();

	void DeleteSelectedShape();
	void ActivatePhysics();
	void DeActivatePhysics();
	void Edit();
	void EditInPhysicsMode(float deltaTime);
	void EditInCompoundMode();
	int GetNumCompoundPrims(int compoundShapeID);
	void AddOrDeleteOptions(string label);

	bool Select(float x, float y);
	bool Copy();
	bool Paste();

	void WriteInFile(string filePath);
	void WritePolyInFile(FILE* fp, Prim* prim);
	void ReadFromFile(string filePath);
	void CheckForSaveBeforeLoad();

	string OpenFileDialog(char *filter = "Text documents (*.txt)\0*.txt\0", HWND owner = NULL);
	string SaveFileDialog(char *filter = "Text documents (*.txt)\0*.txt\0", HWND owner = NULL);
};

#endif



	//string OpenFileDialog(char *filter = "All Files (*.txt)\0*.*\0", HWND owner = NULL);
	//string SaveFileDialog(char *filter = "(*.txt)\0*.*\0", HWND owner = NULL);
	//string SaveFileDialog(char *filter = "Text documents (.txt)|*.txt", HWND owner = NULL);
