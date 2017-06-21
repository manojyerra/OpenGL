#ifndef PAGE_H
#define PAGE_H
#pragma once

#include "CommonLib/Sprite.h"
#include "CommonLib/Input.h"

class Page
{
private:	
public:
	Page(){}
	virtual ~Page(){}
	virtual void Update() = 0;
	virtual void Draw() = 0;
};

#endif
