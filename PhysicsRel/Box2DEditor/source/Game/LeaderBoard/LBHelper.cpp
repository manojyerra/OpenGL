#include "LBHelper.h"
#include "UIFont.h"
#include "SaveManager.h"
#include "GameInfo.h"
#include "RectPrim.h"
#include "Platform.h"
#include "math.h"

bool LBHelper::ReadLBData(LeaderBoardData* lbData)
{
	int size = sizeof(LeaderBoardData);

	std::string filePath = LBHelper::GetLBFilePath();

	FILE* fp = fopen(filePath.c_str(),"rb");

	if(fp != NULL)
	{
		int failCount = 0;

		while(true)
		{
			int readSize = fread(lbData, 1, size, fp);
			
			if(readSize == size)
			{
				printLog("online rank : %d, offline rank going generate now... ", lbData->Rank);

				fclose(fp);
				lbData->SetUserInfo();
				return true;
			}
			else
			{
				failCount++;

				if(failCount == 10)
				{
					printLog("Unable to read LB data properly, making LB data empty and left reading.");

					if(lbData)
					{
						delete lbData;
						lbData = new LeaderBoardData();
					}

					return false;
				}
				else
				{
					printLog("issue in reading LB data at cpp, readSize = %d, size = %d", readSize, size);
					printLog("Going try again in few seconds...");

					float val = 0;

					for(int i=0;i<5000;i++)
						val = sqrt((float)234);

					float tempVal = val;
					float temp2 = tempVal;
				}
			}
		}
	}
	else
	{
		printLog("LB file <%s> not found.", filePath.c_str());
	}

	return false;
}


void LBHelper::DrawHeading(int x, int y, int w, int h, int fontH, unsigned int color, int rank)
{
	//UIFont::GetInstance()->SetColor(color);
	//UIFont::GetInstance()->Begin(1);

	//if(SaveManager::GetInstance()->GetGameMode() == SaveManager::STRATEGY_MODE)
	//	UIFont::GetInstance()->DrawFromCenter("Strategy Mode High Scores", x+w/2, y+h/4.5, fontH, 0);
	//else
	//	UIFont::GetInstance()->DrawFromCenter("Timer Mode High Scores", x+w/2, y+h/4.5, fontH, 0);

	//UIFont::GetInstance()->End();

	if(rank == 0)
		RectPrim::Draw(0,0,GameInfo::GetInstance()->CodedW(),GameInfo::GetInstance()->CodedH(), 0, 0x000000aa);

	char chArr[64];
	sprintf(chArr, "Rank : %d", rank);
	UIFont::GetInstance()->SetColor(0xffffffff);
	UIFont::GetInstance()->Begin(1);

	fontH += 5;
	
	if(rank == 0)
	{
		UIFont::GetInstance()->SetColor(0x00ff00ff);
		
		UIFont::GetInstance()->DrawFromCenter("Rank still updating...", x+w/2, y-10+20*h/5, fontH, 0);
		UIFont::GetInstance()->DrawFromCenter("Please check internet connection", x+w/2, fontH + 30 + y-10+20*h/5, fontH, 0);
	}
	else
		UIFont::GetInstance()->DrawFromCenter(chArr, x+w/2, y-28+4*h/5, fontH+10, 0);

	UIFont::GetInstance()->End();
}

void LBHelper::DrawStarsTime(int stars, int time, int x, int y, int fontH)
{
	char chArr[256];
	
	sprintf(chArr, "Time : %s ", GameInfo::GetInstance()->GetTimeStr((int)time, "", "").c_str());

	UIFont::GetInstance()->SetColor(0xffffffff);
	UIFont::GetInstance()->Begin(1);
	UIFont::GetInstance()->Draw(chArr, x+600, y-30, fontH, -0, 0);

	if(stars < 10)			sprintf(chArr, "Stars : 00%d", stars);
	else if(stars < 100)	sprintf(chArr, "Stars : 0%d", stars);
	else					sprintf(chArr, "Stars : %d", stars);

	UIFont::GetInstance()->Draw(chArr, x+600, y-60, fontH, -0, 0);
	UIFont::GetInstance()->End();
}

void LBHelper::DrawScore(int score, int x, int y, int fontH)
{
	char chArr[256];

	sprintf(chArr, "Score : %d", score);

	UIFont::GetInstance()->SetColor(0xffffffff);
	UIFont::GetInstance()->Begin(1);
	UIFont::GetInstance()->Draw(chArr, x, y-35, fontH, -1, 0);
	UIFont::GetInstance()->End();
}

void LBHelper::DrawUserInfo(string name, string country, int x, int y, int fontH, int gap)
{
	y+=5;

	char chArr[256];

	UIFont::GetInstance()->SetColor(0xffffffff);
	UIFont::GetInstance()->Begin(1);

	sprintf(chArr, "Name :  %s", name.c_str());
	UIFont::GetInstance()->Draw(chArr, x, y+(0*(fontH+gap))+5, fontH+2, -1,0);

	sprintf(chArr, "Country :  %s", country.c_str());
	UIFont::GetInstance()->Draw(chArr, x, y+(1*(fontH+gap))+10, fontH-5, -1,0);

	UIFont::GetInstance()->End();
}

void LBHelper::DrawNumber(int num, int x, int y, int fontH, unsigned int color)
{
	char ch[16];
	sprintf(ch, "%d", num);
	UIFont::GetInstance()->SetColor(color);
	UIFont::GetInstance()->Begin(1);
	UIFont::GetInstance()->DrawFromCenter(ch, x, y, fontH, -3);
	UIFont::GetInstance()->End();
}

string LBHelper::GetLBFilePath()
{
	string filePath = "";

	if(SaveManager::GetInstance()->GetGameMode() == SaveManager::STRATEGY_MODE)
		filePath = Platform::GetPlatform()->GetSaveFolderPath()+"/strLB.sda";
	else
		filePath = Platform::GetPlatform()->GetSaveFolderPath()+"/tmrLB.sda";
	
	return filePath;
}