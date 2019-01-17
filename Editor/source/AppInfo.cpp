#include "AppInfo.h"
#include <stdio.h>
#include "Sui/SuiIncludes.h"
#include "Sui/SuiFont.h"

AppInfo* AppInfo::_ref = NULL;

AppInfo::AppInfo()
{
	_appLaunchStartTime = 0;
	_appLaunchEndTime = 0;
	_appLanched = false;

	colorBPP = 32;
	depthBPP = 24;
	hasAlphaBuffer = 0;
	baseWindowW = 0;
	baseWindowH = 0;
	currWindowW = 0;
	currWindowH = 0;
	monitorW = 0;
	monitorH = 0;
	isFullScreen = false;
	changeDisplay = true;
	maintainRatio = false;

	viewX = 0;
	viewY = 0;
	viewW = 0;
	viewH = 0;

	fps = 1;

	_multiSampleEnabled = false;
	_pixelFormatSupportMSAA = false;
	_numberOfSamples = 0;
	_pixelFormatHasHighestQualityMSAA = false;

	_vboSupport =  false;
	_textureCompressionSupport = false;
	_textureCompression_s3tc_Support = false;

	_tot3DModelsLoadTime = 0;
	_triCount = 0;
	_objDrawCount = 0;
}

AppInfo::~AppInfo()
{
}

AppInfo* AppInfo::GetInstance()
{
	if(_ref == NULL)
		_ref = new AppInfo();
	return _ref;
}

void AppInfo::DeleteInstance()
{
	if(_ref)
	{
		delete _ref;
		_ref = NULL;
	}
}

void AppInfo::Draw()
{
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LINE_SMOOTH);
	glDisable(GL_TEXTURE_2D);

	glEnable( GL_BLEND );
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glShadeModel(GL_SMOOTH);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, baseWindowW, baseWindowH, 0, 0, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	char arr[128];
	vector<string> printVec;

	if(_appLanched)
	{
		unsigned int launchTime = _appLaunchEndTime - _appLaunchStartTime;
		sprintf(arr, "App LaunchTime : %d milliseconds", (int)launchTime);						printVec.push_back(arr);
	}

	sprintf(arr, "CurrWindow width : %d, height : %d ", (int)currWindowW, (int)currWindowH);		printVec.push_back(arr);
	sprintf(arr, "BaseWindow width : %d, height : %d ", (int)baseWindowW, (int)baseWindowH);		printVec.push_back(arr);	
	sprintf(arr, "FPS : %d ", (int)fps);												printVec.push_back(arr);
	sprintf(arr, "ObjectDrawCount = %d", (int)_objDrawCount);							printVec.push_back(arr);
	sprintf(arr, "TriCount = %d", _triCount);											printVec.push_back(arr);
	sprintf(arr, "3D Models LoadTime = %d milliseconds", (int)_tot3DModelsLoadTime);	printVec.push_back(arr);

	printVec.push_back(" ");
	printVec.push_back("About Extenstions: ");

	if(_pixelFormatSupportMSAA)
	{
		printVec.push_back("Pixel Format Support MSAA : true");
		if(_pixelFormatHasHighestQualityMSAA)
			printVec.push_back("Pixel Format Has Highest Quality MSAA : true");
		else
		{
			printVec.push_back("Pixel Format Has Highest Quality MSAA : false");
			sprintf(arr, "Number of sameple using %d ", (int)_numberOfSamples);		printVec.push_back(arr);
		}

		if(_multiSampleEnabled)
			printVec.push_back("MSAA enabled : true");
		else
			printVec.push_back("MSAA enabled : false");
	}
	else
		printVec.push_back("Pixel Format Support MSAA : false");

	if(_vboSupport)
		printVec.push_back("VBO Support : true");
	else
		printVec.push_back("VBO Support  : false");

	if(_textureCompressionSupport)
		printVec.push_back("Texture Compression Support : true");
	else
		printVec.push_back("Texture Compression Support : false");

	if(_textureCompression_s3tc_Support)
		printVec.push_back("Texture Compression s3tc Support : true");
	else
		printVec.push_back("Texture Compression s3tc Support : false");

	float fontSize = SuiFont::GetInstance()->GetFontSize();
	int y = (fontSize+5) * printVec.size();

	glColor4ub(255,255,255,220);
	glBegin(GL_TRIANGLE_STRIP);
	glVertex2f(0,0);
	glVertex2f(300,0);
	glVertex2f(0,y);
	glVertex2f(300,y);
	glEnd();

	SuiFont::GetInstance()->SetColor(0x000000ff);
	SuiFont::GetInstance()->Begin();

	y = 0;
	for(int i=0;i<printVec.size();i++)
	{
		SuiFont::GetInstance()->Draw(printVec[i], 0, y, fontSize);	
		y += fontSize+5;
	}

	SuiFont::GetInstance()->End();
}
