#ifndef SUI_H
#define SUI_H

#define  SUISetup SUILifeCycle::Setup
#define  SUIDraw SUILifeCycle::Draw
#define  SUIQuit SUILifeCycle::Quit

namespace SUI
{
	class SUILifeCycle
	{
	public:
		static void Setup(int windowWidth, int windowHeight);
		static void Draw();
		static void Quit();
	};
}

#endif
