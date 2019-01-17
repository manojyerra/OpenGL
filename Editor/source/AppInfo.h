#ifndef AppInfo_H
#define AppInfo_H
#pragma once

class AppInfo
{
private:
	static AppInfo* _ref;
	AppInfo();
	~AppInfo();

public:
	bool _appLanched;
	unsigned int _appLaunchStartTime;
	unsigned int _appLaunchEndTime;

	//Window Info...
	int colorBPP;
	int depthBPP;
	int hasAlphaBuffer;
	int baseWindowW;
	int baseWindowH;
	int currWindowW;
	int currWindowH;
	int monitorW;
	int monitorH;
	bool isFullScreen;
	bool changeDisplay;
	bool maintainRatio;

	int viewX;
	int viewY;
	int viewW;
	int viewH;

	int fps;

	bool _multiSampleEnabled;
	bool _pixelFormatSupportMSAA;
	int _numberOfSamples;
	bool _pixelFormatHasHighestQualityMSAA;

	bool _vboSupport;
	bool _textureCompressionSupport;
	bool _textureCompression_s3tc_Support;

	float _tot3DModelsLoadTime;
	unsigned int _triCount;
	unsigned int _objDrawCount;

	static AppInfo* GetInstance();
	static void DeleteInstance();

	void GetVBOExtensions();
	void Draw();
};

#endif
