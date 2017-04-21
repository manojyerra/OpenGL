#include "GLAndUtil.h"
#include <windows.h>
#include <gl/gl.h>
#include <vector>
#include <string>
using namespace std;

class BBox
{
public:
	float minX;
	float minY;
	float minZ;
	float maxX;
	float maxY;
	float maxZ;

	BBox()
	{
		minX = minY = minZ = maxX = maxY = maxZ = 0;
	}

	BBox(float* vertexPointer, int numVertices)
	{
		CalcBBox(vertexPointer, numVertices);
	}

	void CalcBBox(float* vertexPointer, int numVertices)
	{
		minX = vertexPointer[0];
		maxX = vertexPointer[0];
		minY = vertexPointer[1];
		maxY = vertexPointer[1];
		minZ = vertexPointer[2];
		maxZ = vertexPointer[2];

		for(int i=3; i<numVertices*3; i+=3)
		{
			float x = vertexPointer[i+0];
			float y = vertexPointer[i+1];
			float z = vertexPointer[i+2];

			if(x < minX) minX = x;
			if(x > maxX) maxX = x;

			if(y < minY) minY = y;
			if(y > maxY) maxY = y;
			
			if(z < minZ) minZ = z;
			if(z > maxZ) maxZ = z;
		}		
	}

	void Draw()
	{
		float cx = ( minX + maxX ) / 2.0f;
		float cy = ( minY + maxY ) / 2.0f;
		float cz = ( minZ + maxZ ) / 2.0f;

		float w = ( maxX - minX ) / 2.0f;
		float h = ( maxY - minY ) / 2.0f;
		float d = ( maxZ - minZ ) / 2.0f;

		unsigned int prevColor = util::GLColor(0xff0000ff);

		glBegin(GL_QUADS);

		// Front Face
		glColor4f(0, 1.0f, 0, 0.5);
		glVertex3f(cx-w, cy-h,  cz+d);  // Bottom Left Of The Texture and Quad
		glVertex3f(cx+w, cy-h,  cz+d);  // Bottom Right Of The Texture and Quad
		glVertex3f(cx+w, cy+h,  cz+d);  // Top Right Of The Texture and Quad
		glVertex3f(cx-w, cy+h,  cz+d);  // Top Left Of The Texture and Quad

		// Back Face
		glColor4f(1, 0.0f, 0, 0.5);
		glVertex3f(cx-w, cy-h, cz-d);  // Bottom Right Of The Texture and Quad
		glVertex3f(cx-w, cy+h, cz-d);  // Top Right Of The Texture and Quad
		glVertex3f(cx+w, cy+h, cz-d);  // Top Left Of The Texture and Quad
		glVertex3f(cx+w, cy-h, cz-d);  // Bottom Left Of The Texture and Quad

		// Top Face
		glColor4f(1, 1.0f, 0, 0.5);
		glVertex3f(cx-w, cy+h, cz-d);  // Top Left Of The Texture and Quad
		glVertex3f(cx-w, cy+h, cz+d);  // Bottom Left Of The Texture and Quad
		glVertex3f(cx+w, cy+h, cz+d);  // Bottom Right Of The Texture and Quad
		glVertex3f(cx+w, cy+h, cz-d);  // Top Right Of The Texture and Quad
		
		// Bottom Face
		glColor4f(0, 0, 1, 0.5);
		glVertex3f(cx-w, cy-h, cz-d);  // Top Right Of The Texture and Quad
		glVertex3f(cx+w, cy-h, cz-d);  // Top Left Of The Texture and Quad
		glVertex3f(cx+w, cy-h, cz+d);  // Bottom Left Of The Texture and Quad
		glVertex3f(cx-w, cy-h, cz+d);  // Bottom Right Of The Texture and Quad

		// Right face
		glColor4f(1, 0, 1, 0.5);
		glVertex3f(cx+w, cy-h, cz-d);  // Bottom Right Of The Texture and Quad
		glVertex3f(cx+w, cy+h, cz-d);  // Top Right Of The Texture and Quad
		glVertex3f(cx+w, cy+h, cz+d);  // Top Left Of The Texture and Quad
		glVertex3f(cx+w, cy-h, cz+d);  // Bottom Left Of The Texture and Quad

		// Left Face
		glColor4f(0, 1, 1, 0.5);
		glVertex3f(cx-w, cy-h, cz-d);  // Bottom Left Of The Texture and Quad
		glVertex3f(cx-w, cy-h, cz+d);  // Bottom Right Of The Texture and Quad
		glVertex3f(cx-w, cy+h, cz+d);  // Top Right Of The Texture and Quad
		glVertex3f(cx-w, cy+h, cz-d);  // Top Left Of The Texture and Quad

		glEnd();

		util::GLColor(prevColor);
	}
};

class FLModel
{
private:
	unsigned char* _verticesPointer;
	unsigned char* _uvsPointer;
	unsigned char* _normalsPointer;
	unsigned char* _indicesPointer;
	unsigned int _indicesType;

	unsigned int _numVertex;
	unsigned int _numIndices;

	unsigned int _textureID;
	bool _isTextureEnabled;

	BBox _bBox;
	vector<float> _borderVec;

public:
    FLModel(string filePath);
	void SetTextureEnable(bool enable);
	bool IsTextureEnabled();
    void Draw();
	void DrawBoundingBox();
	void DrawBounding2DRect();
	void CalcBorder();
	void DrawBorder();
};
