#include "Platform/Platform.h"
#include "GameLoop.h"
//#include "vld.h"

GameLoop* gameLoop = 0;

void Init()
{
	gameLoop = new GameLoop();
}

void Update(float deltaTime)
{
	gameLoop->Run(deltaTime);
}


void OnTouch(int x, int y, int eventType)
{
}


void Terminate()
{
	if(gameLoop)
	{
		delete gameLoop;
		gameLoop = NULL;
	}
}



void RunGame()
{
	Init();

	unsigned long long startTime = Platform::TimerGetMs();

	while(Platform::DeviceCheckQuitRequest() == false)
	{
		if(gameLoop)
		{
			unsigned long systemTime = Platform::TimerGetMs();
			float deltaTime = float(systemTime - startTime)/1000.0f;

			if(deltaTime <= 1.0f/35.0f)
				continue;
			startTime = systemTime;
			Update(deltaTime);
			Platform::Swap_Buffers();
		}
	}

	Terminate();
}


#ifdef  PLATFORM_WIN32
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	Platform::hInstance = hInstance;
	RunGame();
	return 0;
}
#endif


#ifdef  PLATFORM_MARMALADE
int main(int argc, char* argv[])
{
	RunGame();
	return 0;
}
#endif