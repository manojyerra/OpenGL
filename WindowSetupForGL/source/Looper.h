#ifndef Looper_H
#define Looper_H

class Looper
{
private:
	float _windowW;
	float _windowH;

public:
	Looper(int windowWidth, int windowHeight);
	~Looper();

	void Update(float deltaTime);
	void Draw();
};

#endif