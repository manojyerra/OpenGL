#ifndef Looper_H
#define Looper_H

#include "SUI/SUI.h"
#include "SUI/SUIInput.h"
#include "SUI/SUIActionListener.h"
#include "ImageBuffer.h"
#include "BatchImageEdit.h"

class Looper
{
private:
	float _windowW;
	float _windowH;
	BatchImageEdit* _batchImageEdit;

public:
	Looper(int windowWidth, int windowHeight);
	~Looper();

	void Draw(float deltaTime);
};

#endif
