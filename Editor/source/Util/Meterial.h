#ifndef Meterial_H
#define Meterial_H

class Meterial
{
private:
	static Meterial* _ref;
	Meterial();

public:
	float* massArr;
	~Meterial();
	static Meterial* GetInstance();
	static void DeleteInstance();
};

#endif
