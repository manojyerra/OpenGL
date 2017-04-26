#ifndef SUI_CYCLE_H
#define SUI_CYCLE_H

class SuiCycle
{
public:
	virtual void Update() = 0;
	virtual void Draw() = 0;
	virtual ~SuiCycle() {}
};

#endif