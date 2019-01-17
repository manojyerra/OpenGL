#ifndef Shape_H
#define Shape_H

class Shape
{
private:
	int _id;
	
public:
	static const int BOX = 1;
	static const int CONE = 2;
	static const int CYLINDER = 3;
	static const int SPHERE = 4;
	static const int MESH = 5;

	Shape(int id)
	{
		_id = id;
	}

	int GetID()
	{
		return _id;
	}

	virtual void SetOpenGLMatrix(float* mat) = 0;
	virtual void GetOpenGLMatrix(float* mat) = 0;
	virtual void Draw(bool randomColors = true) = 0;
};

#endif