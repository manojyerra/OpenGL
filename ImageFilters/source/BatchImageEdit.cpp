#include "BatchImageEdit.h"
#include "ImageBuffer.h"
#include "SUI/SUI.h"
#include "SUI/SUIManager.h"

BatchImageEdit::BatchImageEdit(float windowWidth, float windowHeight)
{
	_windowW = windowWidth;
	_windowH = windowHeight;

	_originalImageFrame = new SUIFrame(0,0,0,0,SUIFrame::V_ALIGNMENT);
	_modifiedImageFrame = new SUIFrame(0,0,0,0,SUIFrame::V_ALIGNMENT);

	_originalImageFrame->SetBgVisible(false);
	_modifiedImageFrame->SetBgVisible(false);

	_originalImageFrame->SetBorderColor(255,255,255,255);
	_modifiedImageFrame->SetBorderColor(255,255,255,255);

	_uiDesign = new UIDesign(_windowW, _windowH, this);

	_sourceBaseFolder = NULL;
	_destinBaseFolder = NULL;

	_suiOriginalBuff = NULL;
	_suiCurrentBuff = NULL;
	_suiOptionBeginBuff = NULL;
	_readFilePath = "";

	//SetSourceFolderPath("data");
	//SetDestinFolderPath("data_new");
	//DumpFolderStructure();

	LoadFile("Sample.png");
}

void BatchImageEdit::Update()
{
}

void BatchImageEdit::Draw()
{
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glEnable( GL_BLEND );
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glShadeModel(GL_SMOOTH);

	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glViewport(0, 0, (GLsizei)_windowW, (GLsizei)_windowH);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, _windowW, _windowH, 0, 0, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	if(_suiCurrentBuff == NULL)
		return;

	DrawChecks(_originalImageFrame->GetX(), _originalImageFrame->GetY(), _originalImageFrame->GetW(), _originalImageFrame->GetH(), 10,10);
	DrawTexture(_textureIDOriginal, _originalImageFrame->GetX(), _originalImageFrame->GetY(), _originalImageFrame->GetW(), _originalImageFrame->GetH());

	DrawChecks(_originalTextureW+3,0,_originalTextureW, _originalTextureH, 10,10);
	DrawTexture(_textureID,			_originalTextureW+3,0,_originalTextureW, _originalTextureH);
}

void BatchImageEdit::DrawChecks(int xPos, int yPos, int width, int height, int cellWidth, int cellHeight)
{
	glBegin(GL_TRIANGLES);

	int count = 0;
	int rowsCount = 0;
	bool loopBreak = false;
	int backUpCellWidth = cellWidth;
	int backUpCellHeight = cellHeight;
	
	for(int y=yPos; y<yPos+height+cellHeight; y+=cellHeight)
	{
		cellWidth = backUpCellWidth;
		cellHeight = backUpCellHeight;
		
		rowsCount++;
		count = rowsCount;

		if(y+cellHeight > yPos+height)
		{
			cellHeight = yPos+height - y;
			loopBreak = true;
		}

		for(int x=xPos; x<xPos+width+cellWidth; x+=cellWidth)
		{
			count++;

			bool breakInnerLoop = false;
			if(x+cellWidth > xPos+width)
			{
				cellWidth = xPos+width - x;
				breakInnerLoop = true;
			}

			if(count%2 == 0)
				glColor4f(0.2f, 0.2f, 0.2f, 1.0f);
			else
				glColor4f(0.5f, 0.5f, 0.5f, 1.0f);

			glVertex2d(x,				y);
			glVertex2d(x+cellWidth,		y);
			glVertex2d(x,				y+cellHeight);

			glVertex2d(x+cellWidth,		y);
			glVertex2d(x,				y+cellHeight);
			glVertex2d(x+cellWidth,		y+cellHeight);

			if(breakInnerLoop)
				break;
		}

		if(loopBreak)
			break;
	}

	glEnd();
}

void BatchImageEdit::DrawTexture(unsigned int textureID, int x, int y, int width, int height)
{
	glEnable( GL_TEXTURE_2D );
	glBindTexture( GL_TEXTURE_2D, textureID );

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glBegin( GL_QUADS );
	glTexCoord2d(0.0,0.0); glVertex2d(x,		y);
	glTexCoord2d(1.0,0.0); glVertex2d(x+width,	y);
	glTexCoord2d(1.0,1.0); glVertex2d(x+width,	y+height);
	glTexCoord2d(0.0,1.0); glVertex2d(x,		y+height);
	glEnd();

	glDisable( GL_TEXTURE_2D );
}

void BatchImageEdit::actionPerformed(SUIActionEvent actionEvent)
{
	SUIComponent* com = (SUIComponent*)actionEvent.GetComponent();

	if(com == _uiDesign->GetOptionsList())
	{
		_undoVec.push_back(_suiOptionBeginBuff->Clone());
		_undoIndexVec.push_back(_uiDesign->GetOptionsList()->GetSelectedIndex());

		if(_suiOptionBeginBuff)
		{
			delete _suiOptionBeginBuff;
			_suiOptionBeginBuff = NULL;
		}

		_suiOptionBeginBuff = _suiCurrentBuff->Clone();

		_uiDesign->InVisibleSliderFrameComponents();
		_uiDesign->ActionOnListSelection(_suiCurrentBuff);
		_uiDesign->GetSliderFrame()->ResetBounds();

		glDeleteTextures(1, &_textureID);
		GenerateGLTextureID();
	}
	else if(_uiDesign->IsSliderFrameComponent(com))
	{
		if(_suiCurrentBuff)
		{
			delete _suiCurrentBuff;
			_suiCurrentBuff = NULL;
		}

		_suiCurrentBuff = _suiOptionBeginBuff->Clone();

		_uiDesign->ActionOnSliderMove(_suiCurrentBuff);

		glDeleteTextures(1, &_textureID);
		GenerateGLTextureID();
	}
	else if(com->GetName().compare("Undo") == 0)
	{
		if(_undoVec.size()>0)
		{
			if(_suiOptionBeginBuff)
			{
				delete _suiOptionBeginBuff;
				_suiOptionBeginBuff = NULL;
			}

			_suiOptionBeginBuff = _undoVec[_undoVec.size()-1];

			_undoVec.erase(_undoVec.begin() + _undoVec.size()-1);

			if(_suiCurrentBuff)
			{
				delete _suiCurrentBuff;
				_suiCurrentBuff = NULL;
			}

			_suiCurrentBuff = _suiOptionBeginBuff->Clone();

			_undoIndexVec.erase(_undoIndexVec.begin() + _undoIndexVec.size()-1 );

			if(_undoIndexVec.size() > 0)
				_uiDesign->GetOptionsList()->SetSelect(_undoIndexVec[_undoIndexVec.size()-1]);
			else
				_uiDesign->GetOptionsList()->SetSelect(-1);

			_uiDesign->InVisibleSliderFrameComponents();
			_uiDesign->ActionOnListSelection(_suiCurrentBuff);
			_uiDesign->GetSliderFrame()->ResetBounds();

			//_uiDesign->ActionOnSliderMove(_suiCurrentBuff);

			glDeleteTextures(1, &_textureID);
			GenerateGLTextureID();
		}
	}
	else if(com->GetName().compare("Open File") == 0)
	{
		LoadFile(OpenFileDialog());
	}
	else if(com->GetName().compare("Save File") == 0)
	{
		string saveFilePath = SaveFileDialog();

		if(saveFilePath.length() > 0)
			_suiCurrentBuff->Write(saveFilePath);
	}
}

void BatchImageEdit::LoadFile(string filePath)
{
	if(filePath.length()>0)
	{
		_readFilePath = filePath;
		_suiOriginalBuff = new ImageBuffer(_readFilePath);
		_suiCurrentBuff = _suiOriginalBuff->Clone();
		_suiOptionBeginBuff = _suiCurrentBuff->Clone();

		_textureID = 0;

		GenerateGLTextureID();
		_textureIDOriginal = _textureID;
		_originalTextureW = _suiCurrentBuff->GetWidth();
		_originalTextureH = _suiCurrentBuff->GetHeight();

		float totalEmptyW = _uiDesign->GetListFrame()->GetX()-2;
		float totalEmptyH = _uiDesign->GetSliderFrame()->GetY();

		float perTextureWidth = totalEmptyW/2 - 2;
		float perTextureHeight = totalEmptyH;

		if(_originalTextureW > perTextureWidth)
		{
			_originalTextureH = (int)((float)_originalTextureH * perTextureWidth/ (float)_originalTextureW);
			_originalTextureW = perTextureWidth;
		}

		if(_originalTextureH > perTextureHeight)
		{
			_originalTextureW = (int)((float)_originalTextureW * perTextureHeight/ (float)_originalTextureH);
			_originalTextureH = perTextureHeight;
		}
	
		_originalImageFrame->SetBounds(0,0,_originalTextureW, _originalTextureH);
		_modifiedImageFrame->SetBounds(_originalTextureW+3,0,_originalTextureW, _originalTextureH);

		_originalTextureFormat = GL_RGBA;

		if(_suiCurrentBuff->GetBytesPerPixel() == 3)
				_originalTextureFormat = GL_RGB;

		//InVisibleSliderFrameComponents();
		//ActionOnListSelection();

		GenerateGLTextureID();
	}
}

void BatchImageEdit::GenerateGLTextureID()
{
	glGenTextures(1, &_textureID);
	glBindTexture(GL_TEXTURE_2D, _textureID);

	glPixelStorei( GL_UNPACK_ALIGNMENT, 1);
	if(_suiCurrentBuff->GetBytesPerPixel() == 4)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _suiCurrentBuff->GetWidth(), _suiCurrentBuff->GetHeight(), 0,
						GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*)_suiCurrentBuff->GetBuffer());
	else
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _suiCurrentBuff->GetWidth(), _suiCurrentBuff->GetHeight(), 0,
						GL_RGB, GL_UNSIGNED_BYTE, (GLvoid*)_suiCurrentBuff->GetBuffer());

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
}

string BatchImageEdit::OpenFileDialog(char *filter, HWND owner) 
{
	OPENFILENAME ofn;
	char fileName[MAX_PATH] = "";
	ZeroMemory(&ofn, sizeof(ofn));
	 
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = owner;
	ofn.lpstrFilter = filter;
	ofn.lpstrFile = fileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	ofn.lpstrDefExt = "";
	 
	string fileNameStr;
	 
	if ( GetOpenFileName(&ofn) )
	fileNameStr = fileName;
	 
	return fileNameStr;
}

string BatchImageEdit::SaveFileDialog(char *filter, HWND owner) 
{
	OPENFILENAME ofn;
	char fileName[MAX_PATH] = "";
	ZeroMemory(&ofn, sizeof(ofn));
	 
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = owner;
	ofn.lpstrFilter = filter;
	ofn.lpstrFile = fileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	ofn.lpstrDefExt = "";

	string fileNameStr;

	if(GetSaveFileNameA(&ofn))
		fileNameStr = fileName;

	return fileNameStr;
}

BatchImageEdit::~BatchImageEdit()
{
	for(int i=0;i<_undoVec.size();i++)
	{
		delete _undoVec[i];
		_undoVec[i] = NULL;
	}

	if(_suiOriginalBuff)
	{
		delete _suiOriginalBuff;
		_suiOriginalBuff = NULL;
	}

	if(_suiCurrentBuff)
	{
		delete _suiCurrentBuff;
		_suiCurrentBuff = NULL;
	}

	if(_suiOptionBeginBuff)
	{
		delete _suiOptionBeginBuff;
		_suiOptionBeginBuff = NULL;
	}

	if(_sourceBaseFolder)
	{
		delete _sourceBaseFolder;
		_sourceBaseFolder = NULL;
	}

	if(_destinBaseFolder)
	{
		delete _destinBaseFolder;
		_destinBaseFolder = NULL;
	}

	if(_originalImageFrame)
	{
		delete _originalImageFrame;
		_originalImageFrame = NULL;
	}

	if(_modifiedImageFrame)
	{
		delete _modifiedImageFrame;
		_modifiedImageFrame = NULL;
	}

	if(_uiDesign)
	{
		delete _uiDesign;
		_uiDesign = NULL;
	}
}

void BatchImageEdit::SetSourceFolderPath(string path)
{
	_sourceBaseFolder = new SUIFile(path);
}

void BatchImageEdit::SetDestinFolderPath(string path)
{
	_destinBaseFolder = new SUIFile(path);
}

bool BatchImageEdit::DumpFolderStructure()
{
	if(!_sourceBaseFolder || !_sourceBaseFolder->isDirectory())
		return false;

	if(!_destinBaseFolder)
	{
		string path = _sourceBaseFolder->getAbsolutePath();
		path.append("_new");
		_destinBaseFolder = new SUIFile(path);
	}

	if(!_destinBaseFolder->isDirectory())
		_destinBaseFolder->makeDirectory();

	CreateFolderDump(*_sourceBaseFolder);
	return true;
}

void BatchImageEdit::CreateFolderDump(SUIFile& sourceDir)
{
	vector<SUIFile> filesVec = sourceDir.listFiles();

	for(int i=0; i<(int)filesVec.size(); i++)
	{
		SUIFile sourceFile = filesVec[i];

		if(sourceFile.isHidden())
			continue;

		string remaining = SubtractPath(sourceFile.getAbsolutePath(), _sourceBaseFolder->getAbsolutePath());

		if(remaining.length() <= 0)
			continue;

		SUIFile destinFile( _destinBaseFolder->getAbsolutePath().append ( remaining ) );

		if(sourceFile.isDirectory())
		{
			destinFile.makeDirectory();
			CreateFolderDump(sourceFile);
		}
		else
		{
			ProcessFile(sourceFile, destinFile);
		}
	}
}

void BatchImageEdit::ProcessFile(SUIFile& sourceFile, SUIFile& destinFile)
{
	if(sourceFile.getAbsolutePath().find(".png") == -1)
		return;

	unsigned int startTime = 0;

	//Reading....
	startTime = GetTickCount();
	ImageBuffer suiBuff(sourceFile.getAbsolutePath());
	unsigned int readTime = GetTickCount() - startTime;

	//Process....
	suiBuff.AddAlphaLayer();
	startTime = GetTickCount();
	unsigned int processTime = GetTickCount() - startTime;

	//Write....
	startTime = GetTickCount();
	suiBuff.Write(destinFile.getPath());
	unsigned int writeTime = GetTickCount() - startTime;
}

string BatchImageEdit::SubtractPath(string superSetStr, string subSetStr)
{
	if(superSetStr.length() <= subSetStr.length())
		return "";

	if(superSetStr.find_first_of(subSetStr) == -1)
		return "";

	return superSetStr.substr(subSetStr.length(), superSetStr.length());
}


	//bool success = suiBuff.BeginWrite(destinFile.getPath());
	//
	//if(success)
	//{
	//	int percentDone = 0;
	//	while((percentDone = suiBuff.WritePercent(1)) != 100)
	//	{
	//		_slider->SetValue(percentDone);
	//		SuiManager::GetInstance()->SwapBuffers();
	//	}

	//	_slider->SetValue(100);
	//	SuiManager::GetInstance()->SwapBuffers();
	//}
	


	//SuiComponent* com = (SuiComponent*)actionEvent.GetComponent();

	//if(com == _radioBtn)
	//{
	//	int selectedIndex = ((SuiRadioButton*)com)->GetSelectedIndex();

	//	if(selectedIndex == 0)
	//	{
	//	}
	//}
