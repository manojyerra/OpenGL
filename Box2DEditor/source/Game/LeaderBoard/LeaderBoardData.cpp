#include "LeaderBoardData.h"
#include "FileReader.h"

LeaderBoardData::LeaderBoardData()
{
	for(int i=0;i<SHOW_PLAYERS_ON_LB;i++)
	{
		memset(ObjectID[i], '\0', 32*sizeof(char));
		memset(NamesArray[i], '\0', 32*sizeof(char));
		memset(CountryNameArray[i], '\0', 32*sizeof(char));
	}

	int size = SHOW_PLAYERS_ON_LB*sizeof(int);

	memset(StrategyModeScoreArray, 0, size);
	memset(TimerModeScoreArray, 0, size);

	memset(UserName, '\0', 32*sizeof(char));
	memset(UserCountryName, '\0', 32*sizeof(char));

	SetUserInfo();

	Rank = 0; 
}


void LeaderBoardData::SetUserInfo()
{
	FileReader fr;

	if(fr.LoadFileFromSaveFolder("userInfo.sda"))
	{
		string id = fr.ReadLine();
		string name = fr.ReadLine();
		string country = fr.ReadLine();
		fr.Close();

		memset(UserName, '\0', 32*sizeof(char));
		memset(UserCountryName, '\0', 32*sizeof(char));

		for(int i=0;i<name.length() && i<31;i++)
			UserName[i] = name[i];

		for(int i=0;i<country.length() && i<31;i++)
			UserCountryName[i] = country[i];
	}
	else
	{
		memset(UserName, '\0', 32*sizeof(char));
		memset(UserCountryName, '\0', 32*sizeof(char));

		UserName[0] = 'u';
		UserName[1] = 'n';
		UserName[2] = 'n';
		UserName[3] = 'a';
		UserName[4] = 'm';
		UserName[5] = 'e';
		UserName[6] = 'd';

		UserCountryName[0] = 'u';
		UserCountryName[1] = 'n';
		UserCountryName[2] = 'n';
		UserCountryName[3] = 'a';
		UserCountryName[4] = 'm';
		UserCountryName[5] = 'e';
		UserCountryName[6] = 'd';
	}
}
