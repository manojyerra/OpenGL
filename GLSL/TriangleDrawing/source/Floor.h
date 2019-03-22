#ifndef Floor_H
#define Floor_H

#include "ShaderProgram.h"
#include "GLUtil/GLBatch.h"
#include "GLUtil/GLBuffer.h"

class Floor
{
private:
	bool _visible;
	bool _axisVisible;
	bool _gridVisible;
	bool _gridLinesVisible;

	ShaderProgram* _shaderProgram;

	GLBuffer* _axisBuffer;
	GLBuffer* _gridBuffer;
	GLBuffer* _gridLinesBuffer;

public:
	Floor();

	bool IsVisible();
	bool IsAxisVisible();
	bool IsGridVisible();
	bool IsGridLinesVisible();

	void SetVisible(bool visible);
	void SetAxisVisible(bool axisVisible);
	void SetGridVisible(bool floorVisible);
	void SetGridLinesVisible(bool linesVisible);

	void Draw();
};

#endif
