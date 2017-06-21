#ifndef EDITOR_H
#define EDITOR_H
#pragma once

#include "Box2D.h"
#include "EditPrim.h"
#include "MyWindow.h"

#include <cstdio>
#include <vector>
#include <string>
using namespace std;


class UndoRedo
{
private:
	vector<EditPrim*> lastUndoVec;
	vector<vector<EditPrim*>> vec;
	int currIndex;

public:
	UndoRedo()
	{
		currIndex = -1;
	}

	void Copy(vector<EditPrim*> copyVec)
	{
		if (currIndex < vec.size()-1)
			DeleteExtraData();

		currIndex++;

		for (int i = 0; i < copyVec.size(); i++)
			copyVec[i] = copyVec[i]->Clone();

		vec.push_back(copyVec);
		//SetLastUndoVec(copyVec);

		//printLog("\nCopy : currIndex  :  %d, vecsize : %d", currIndex, vec.size());
	}

	vector<EditPrim*> UndoData()
	{
		vector<EditPrim*> returnVec;

		currIndex--;

		if(currIndex < -1)
			currIndex = -1;

		if (currIndex >= 0 && currIndex <vec.size())
			returnVec = vec[currIndex];

		//SetLastUndoVec(returnVec);

		printLog("\n UndoData : currIndex :  %d, vecsize : %d", currIndex, vec.size());
		
		return returnVec;
	}

	vector<EditPrim*> RedoData()
	{
		vector<EditPrim*> returnVec;

		currIndex++;

		if (currIndex >= vec.size()-1)
			currIndex = vec.size()-1;

		if (currIndex >= 0 && currIndex < vec.size())
			returnVec = vec[currIndex];

		//SetLastUndoVec(returnVec);

		printLog("\n RedoData : currIndex  :  %d, vecsize : %d", currIndex, vec.size());

		
		return returnVec;
	}

	void DeleteExtraData()
	{
		if (currIndex+1 >= 0)
		{
			for (int i = currIndex+1; i < vec.size(); i++)
			{
				vector<EditPrim*> subVec = vec[i];

				for (int j = 0; j < subVec.size(); j++)
				{
					delete subVec[j];
					subVec[j] = NULL;
				}

				vec.erase(vec.begin() + i);
				i--;
			}
		}

		printLog("\n DeleteExtraData :  Vector size :  %d", vec.size());
	}

	vector<EditPrim*> GetLastUndoData()
	{
		//return lastUndoVec;

		if(currIndex >=0  && currIndex <vec.size())
			return vec[currIndex];

		vector<EditPrim*> temp;
		return temp;
	}

	void SetLastUndoVec(vector<EditPrim*> copyVec)
	{
		for (int i = 0; i<(int)lastUndoVec.size(); i++)
		{
			if (lastUndoVec[i])
			{
				delete lastUndoVec[i];
				lastUndoVec[i] = NULL;
			}
		}

		lastUndoVec.clear();

		for (int i = 0; i < copyVec.size(); i++)
			lastUndoVec.push_back(copyVec[i]->Clone());
	}

	~UndoRedo()
	{
		for (int i = 0; i < vec.size(); i++)
		{
			vector<EditPrim*> subVec = vec[i];

			for (int j = 0; j < subVec.size(); j++)
			{
				delete subVec[j];
				subVec[j] = NULL;
			}

			vec[i].clear();
		}

		vec.clear();

		for (int j = 0; j < lastUndoVec.size(); j++)
		{
			delete lastUndoVec[j];
			lastUndoVec[j] = NULL;
		}
	}
};


class Editor
{
private:
	UndoRedo* _undoRedo;

	vector<EditPrim*> _primVec;
	vector<EditPrim*> _copyPrimVec;
	EditPrim* _activePrim;
	bool _activatePhysics;
	int _compoundShapeID;
	string _currFilePath;
	vector<Point> _pointsVec;
	Point _selP1;
	Point _selP2;
	bool _showOptions;

	MyWindow* _myWindow;
	MyButton* _physicsBtn;
	MyButton* _addLineBtn;
	MyButton* _addCircleBtn;
	MyButton* _addRectBtn;
	MyButton* _addPoly3Btn;
	MyButton* _addPoly4Btn;
	MyButton* _addPoly5Btn;
	MyButton* _addPoly6Btn;
	MyButton* _addPoly7Btn;
	MyButton* _addPoly8Btn;
	MyButton* _lineLoopBtn;
	MyButton* _lineSegLoopBtn;
	MyButton* _comLineLoopBtn;
	MyButton* _deleteBtn;
	MyButton* _duplicateBtn;
	MyButton* _copyBtn;
	MyButton* _pasteBtn;
	MyButton* _selectAllBtn;
	MyButton* _deSelectAllBnt;
	MyButton* _saveBtn;
	MyButton* _loadBtn;
	MyButton* _undoBtn;
	MyButton* _redoBtn;
	MyButton* _horGrid;
	MyButton* _verGrid;

	void AddPrim(EditPrim* prim);
	void ActivatePhysics();
	void DeActivatePhysics();
	void Edit();
	bool CheckButtonEvents(MyButton* btn);
	RectPrim GetRect(Point p1, Point p2);
	bool IsSame(vector<Point> vec1, vector<Point> vec2);
	bool IsCompoundIDExist(int id);
	int GetNewCompoundID();
	int GetMaxCompoundID();
	void AddCompoundLine();
	void DrawLineLoops();

	void Undo();
	void Redo();
	void AddDataForUndo();
	void ActivatePrim(EditPrim* editPrim);
	bool ActivatePrim(float x, float y);
	void DeActiatePrim();
	void DeSelectAll();
	void SelectAll();
	void Duplicate();
	void Copy();
	void Paste();
	void Delete();
	void DeleteActivePrim();
	void DeleteSelectedGroup();
	bool HasGroupSelection();
	void Move(float dx, float dy);
	void AddRotation(float angleInDegrees);
	void MakeLineLoop(bool makeCompound);

	void WriteInFile(string filePath);
	void WritePolyInFile(FILE* fp, EditPrim* prim);
	void ReadFromFile(string filePath);
	void CheckForSaveBeforeLoad();

	string OpenFileDialog(char *filter = "Text documents (*.txt)\0*.txt\0", void* owner = NULL);
	string SaveFileDialog(char *filter = "Text documents (*.txt)\0*.txt\0", void* owner = NULL);

public:
	Editor();
	~Editor();

	void Update(float deltaTime);
	void Draw();
};

#endif