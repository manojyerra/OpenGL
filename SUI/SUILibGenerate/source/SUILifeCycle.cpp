#include "SUILifeCycle.h"
#include "SUIManager.h"

namespace SUI
{
	void SUILifeCycle::Setup(int windowWidth, int windowHeight)
	{
		SUIManager::GetInstance()->Setup(windowWidth, windowHeight);
	}

	void SUILifeCycle::Draw()
	{
		SUIManager::GetInstance()->Run();
	}

	void SUILifeCycle::Quit()
	{
		SUIManager::GetInstance()->DeleteInstance();
	}
}
