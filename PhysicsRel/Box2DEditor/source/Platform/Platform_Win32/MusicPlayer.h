#ifdef _PLATFORM_WIN32

#include "Includes.h"
#include <string>
using namespace std;

class MusicPlayer
{
private:
	int _prevState;
	int _state;
	string _musicFilePath;

public:
	static const int PLAY = 1;
	static const int PAUSE = 2;

	MusicPlayer();
	~MusicPlayer();

	void SetMusicFilePath(string filePath);

	void Play(bool paly);
	bool IsPlaying();
};

#endif
