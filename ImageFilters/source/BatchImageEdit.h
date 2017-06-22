#ifndef BATCH_IMAGE_H
#define BATCH_IMAGE_H
#pragma once

#include "SUI/SUI.h"
#include "SUI/SUIFrame.h"
#include "SUI/SUIList.h"
#include "SUI/SUIBox.h"

#include "SUI/SUIMouseListener.h"
#include "SUI/SUIActionListener.h"
#include "SUI/SUIMouseEvent.h"
#include "SUI/SUIFile.h"

#include "ImageBuffer.h"

#include "UIDesign.h"

#include <vector>
using namespace std;

class BatchImageEdit : public SUIActionListener
{
private:
	UIDesign* _uiDesign;

	SUIFrame* _originalImageFrame;
	SUIFrame* _modifiedImageFrame;
	
	SUIFile* _sourceBaseFolder;
	SUIFile* _destinBaseFolder;

	ImageBuffer* _suiOriginalBuff;
	ImageBuffer* _suiCurrentBuff;
	ImageBuffer* _suiOptionBeginBuff;

	vector<ImageBuffer*> _undoVec;
	vector<int> _undoIndexVec;

	unsigned int _textureID;
	unsigned int _textureIDOriginal;
	int _originalTextureW;
	int _originalTextureH;
	int _originalTextureFormat;

	string _readFilePath;

	float _windowW;
	float _windowH;

	void DrawChecks(int xPos, int yPos, int width, int height, int cellWidth, int cellHeight);
	void DrawTexture(unsigned int textureID, int xPos, int yPos, int widht, int height);
	void LoadFile(string filePath);

	void CreateFolderDump(SUIFile& suiFile);
	void ProcessFile(SUIFile& sourceFile, SUIFile& destinFile);
	string SubtractPath(string superSetStr, string subSetStr);

public:
	BatchImageEdit(float windowWidth, float windowHeight);
	~BatchImageEdit();

	void SetSourceFolderPath(string path);
	void SetDestinFolderPath(string path);

	void Update();
	void Draw();

	bool DumpFolderStructure();
	void actionPerformed(SUIActionEvent actionEvent);
	void GenerateGLTextureID();

	string OpenFileDialog(char *filter = "PNG (*.png)\0*.png\0", HWND owner = NULL);
	string SaveFileDialog(char *filter = "PNG (*.png)\0*.png\0", HWND owner = NULL);
};

#endif


