#include "AllDemos.h"
#include "AppInfo.h"
#include "WinUtil.h"
#include "Cam.h"

#include "Util/SuiBatchFiles.h"
#include "Util/ImageBuffer.h"
#include "winbase.h"

static void print(const char * pszFormat, ...)
{
	const int MAX_LEN = 10000;
	char szBuf[MAX_LEN];

	va_list ap;
	va_start(ap, pszFormat);
	vsnprintf_s(szBuf, MAX_LEN, MAX_LEN, pszFormat, ap);
	va_end(ap);

	WCHAR wszBuf[MAX_LEN] = {0};
	MultiByteToWideChar(CP_UTF8, 0, szBuf, -1, wszBuf, sizeof(wszBuf));
	OutputDebugStringW(wszBuf);
	OutputDebugStringA("\n");

	//WideCharToMultiByte(CP_ACP, 0, wszBuf, sizeof(wszBuf), szBuf, sizeof(szBuf), NULL, FALSE);
	//printf("%s\n", szBuf);
}

AllDemos::AllDemos(int windowWidth, int windowHeight)
{
	//AppInfo::GetInstance()->_vboSupport = ExtensionSupported("GL_ARB_vertex_buffer_object");
	//AppInfo::GetInstance()->_textureCompressionSupport = ExtensionSupported("GL_ARB_texture_compression");
	//AppInfo::GetInstance()->_textureCompression_s3tc_Support = ExtensionSupported("GL_EXT_texture_compression_s3tc");

	AppInfo::GetInstance()->_vboSupport = MakeVBOExtensions();
	AppInfo::GetInstance()->_textureCompressionSupport = MakeTextureCompressionExtensions();
	AppInfo::GetInstance()->_textureCompression_s3tc_Support = MakeMultiTexCoordExtensions();

	SuiBatchFiles suiBatchFiles("ButterFly");

	vector<string> vec = suiBatchFiles.GetAllFilePaths(".jpg");

	int bytesSaved = 0;
	int beforeMem = 0;
	int afterMem = 0;

	char arr[10000];

	for(int i=0;i<vec.size();i++)
	{
		int fileSizeBefore = GetFileSize(vec[i]);
	
		ImageBuffer* imgBuf = new ImageBuffer(vec[i]);
		imgBuf->MakeFeasibleToCompress(3);
		
		string savePath = vec[i].substr(0, vec[i].length()-4);
		savePath.append(".png");

		sprintf(arr, "\n%s", savePath.c_str());
		OutputDebugString(arr);
		imgBuf->Write(savePath);
		delete imgBuf;

		int fileSizeAfter = GetFileSize(vec[i]);

		beforeMem += fileSizeBefore;
		afterMem += fileSizeAfter;
		bytesSaved += (fileSizeBefore - fileSizeAfter);

		sprintf(arr, "\n%s Before : %d, After %d", vec[i].c_str(), fileSizeBefore, fileSizeAfter);
		OutputDebugString(arr);
	}
	
	float savedBytes = beforeMem - afterMem;
	float savePercent = savedBytes * 100 / beforeMem;

	sprintf(arr, "\n\nTotal Before : %d, Total After %d, Saved Memory = %f MB, Save Percent = %f", beforeMem, afterMem, (savedBytes/(1024.0f*1024.0f)), savePercent);
	OutputDebugString(arr);

	OutputDebugString("\n\n\t\t*** Completed ***\n\n");


	_process = new Editor(windowWidth, windowHeight);
}

int AllDemos::GetFileSize(string filePath)
{
	FILE* fp = fopen(filePath.c_str(), "rb");

	int fileSize = 0;

	if(fp)
	{
		fseek(fp, 0, SEEK_END);
		fileSize = ftell(fp);
		fclose(fp);
	}

	return fileSize;
}

void AllDemos::Update(float deltaTime)
{
	if(_process)
		_process->Update(deltaTime);
}

void AllDemos::Draw(float deltaTime)
{
	if(_process)
		_process->Draw(deltaTime);
}

AllDemos::~AllDemos()
{
	if(_process)
	{
		delete _process;
		_process = NULL;
	}

	AppInfo::DeleteInstance();
}

void AllDemos::FontUVGen()
{
	string letters = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890`!\"$%^&*()-_=+[{]};:'@#\\|,<.>/?";

	ImageBuffer* imgBuf = new ImageBuffer("fontwithborder.png");
	unsigned char* buf = imgBuf->GetBuffer();
	int bytesPP = imgBuf->GetBytesPerPixel();
	int w = imgBuf->GetWidth();
	int h = imgBuf->GetHeight();

	char* existArr = (char*) malloc( w*h );
	memset(existArr, 0, w*h);

	vector<int> uVec;
	vector<int> vVec;
	vector<int> cwVec;
	int ch = 49;

	vector<char> lettersVec;

	int count = 0;

	for(int y = 365; y<= 365+70*4; y+=70 , count++)
	{
		int v = 0;

		if(count == 0)
			v = 345;
		else if(count == 1)
			v = 412;
		else if(count == 2)
			v = 479;
		else if(count == 3)
			v = 546;

		for(int x=0; x<w; x++)
		{
			if(imgBuf->GetR(x,y) == 0)
				continue;

			vector<Point> points = imgBuf->GetSimilarPixels(x, y, existArr);

			if(points.size() > 0)
			{
				int minX = points[0].x;
				int maxX = points[0].x;

				for(int j=1;j<points.size();j++)
				{
					int px = points[j].x;

					if(px < minX)
						minX = px;

					else if(px > maxX)
						maxX = px;
				}

				uVec.push_back(minX);
				cwVec.push_back(maxX-minX);
				vVec.push_back(v);

				lettersVec.push_back(letters[uVec.size()-1]);

				x = maxX + 1;
			}
		}
	}

	for(int i=0;i<lettersVec.size();i++)
	{
		print("u[(int)'%c'] = %d; v[(int)'%c'] = %d; cw[(int)'%c'] = %d; ch[(int)'%c'] = %d;",
			lettersVec[i], uVec[i], lettersVec[i], vVec[i], lettersVec[i], cwVec[i], lettersVec[i], ch);
	}
}

	//else if(enableApp == PHY_TEST)
	//	_phyTest = new PhyTest(_windowWidth, _windowHeight);
	//else if(enableApp == PHY_EDITOR)
	//	_editor = new Editor(_windowWidth, _windowHeight);

	//_shadow = new GLShadow(512, 512, _windowWidth, _windowHeight);




	//else if(enableApp == PHY_TEST)
	//	_phyTest->Draw();
	//else if(enableApp == PHY_EDITOR)
	//	_editor->Draw();

	//Cam::GetInstance()->_transX -= 0.1;
	//Cam::GetInstance()->Update();
	//Cam::GetInstance()->SetView();
	//Cam::GetInstance()->_transX += 0.1;

	//_shadow->GLShadowSettings(Cam::GetInstance()->GetProjectionMatrix(), Cam::GetInstance()->GetModelViewMatrix());
	//_shadow->SetEnablePolygonOffset(true);

	//_renderTest->Draw();

	//_shadow->SetEnablePolygonOffset(false);
	//_shadow->CopyDepthValues();

	//Cam::GetInstance()->SetView();
	//_shadow->BeginShadowMap();

	//glViewport(0, 0,_windowWidth, _windowHeight);
	//glEnable(GL_LIGHTING);
	//_renderTest->EnableLighting();
	//_renderTest->Draw();
	//glDisable(GL_LIGHTING);

	//Cam::GetInstance()->DrawFloor();
	//_shadow->EndShadowMap();




	//else if(enableApp == PHY_TEST)
	///	_phyTest->Update();
	//else if(enableApp == PHY_EDITOR)
	//	_editor->Update();




	/*

	//SuiBatchFiles suiBatchFiles("D:/work/MySVNWork/JavaRelated/AndroidGames/NumberPuzzle2/assets");
	SuiBatchFiles suiBatchFiles("C:/Users/rahulkumar/Desktop/russian");
	//SuiBatchFiles suiBatchFiles("C:/Users/arc/Downloads/birzzle_fever/assets/Images");

	vector<string> vec = suiBatchFiles.GetAllFilePaths(".png");

	int bytesSaved = 0;
	int beforeMem = 0;
	int afterMem = 0;

	char arr[10000];

	for(int i=0;i<vec.size();i++)
	{
		int fileSizeBefore = GetFileSize(vec[i]);
	
		ImageBuffer* imgBuf = new ImageBuffer(vec[i]);
		////imgBuf->RemoveAlphaLayerIfNoDataLose();
		imgBuf->MakeFeasibleToCompress(1);
		
		//sprintf(arr, "\n%s", vec[i].c_str());
		//OutputDebugString(arr);
		imgBuf->Write(vec[i]);
		delete imgBuf;

		int fileSizeAfter = GetFileSize(vec[i]);

		beforeMem += fileSizeBefore;
		afterMem += fileSizeAfter;
		bytesSaved += (fileSizeBefore - fileSizeAfter);

		sprintf(arr, "\n%s Before : %d, After %d", vec[i].c_str(), fileSizeBefore, fileSizeAfter);
		OutputDebugString(arr);
	}
	
	float savedBytes = beforeMem - afterMem;
	float savePercent = savedBytes * 100 / beforeMem;

	sprintf(arr, "\n\nTotal Before : %d, Total After %d, Saved Memory = %f MB, Save Percent = %f", beforeMem, afterMem, (savedBytes/(1024.0f*1024.0f)), savePercent);
	OutputDebugString(arr);

	OutputDebugString("\n\n\t\t*** Completed ***\n\n");

	*/


	////// ************* ////
	/*
	ImageBuffer* newImg = new ImageBuffer("C:/Users/rahulkumar/Desktop/a.png");
	unsigned char* newBuf = newImg->GetBuffer();

	int bpp = 3;

	char* existArr = (char*) malloc(w*h);
	memset(existArr, 0, w*h);

	uBuf = uImg->GetBuffer();

	for(int y=0; y<h; y++)
	{
		for(int x=0; x<w; x++)
		{
			vector<Point> points = uImg->GetSimilarPixels(x, y, existArr);

			if(points.size() > 0)
			{
				float sumR = 0;
				float sumG = 0;
				float sumB = 0;

				for(int j=0;j<points.size();j++)
				{
					int px = points[j].x;
					int py = points[j].y;

					int pos = (w*py + px)*bpp;

					sumR += newBuf[pos+0];
					sumG += newBuf[pos+1];
					sumB += newBuf[pos+2];
				}

				sumR = sumR / points.size();
				sumG = sumG / points.size();
				sumB = sumB / points.size();

				for(int j=0;j<points.size();j++)
				{
					int px = points[j].x;
					int py = points[j].y;

					int pos = (w*py + px)*bpp;

					uBuf[pos+0] = (unsigned char)sumR;
					uBuf[pos+1] = (unsigned char)sumG;
					uBuf[pos+2] = (unsigned char)sumB;
				}
			}
		}
	}

	uImg->Contrast(10);
	*/