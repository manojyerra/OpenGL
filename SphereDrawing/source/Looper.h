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

	void rot(int axis, float angleInDegrees, float x, float y, float z, float* newX, float* newY, float* newZ);
	void DrawSphere(float cx, float cy, float cz, float r);
	void Update(float deltaTime);
	void Draw();
};

#endif