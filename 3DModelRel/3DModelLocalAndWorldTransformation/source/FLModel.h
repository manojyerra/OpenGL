#include "GLAndUtil.h"
#include <windows.h>
#include <gl/gl.h>
#include "GLMat.h"
#include "Vector3.h"

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
	bool _wireFrameEnabled;
	bool _boundingBoxEnabled;
	bool _bounding2DRectEnabled;

	vector<float> _borderVec;
	BBox _bBox;

	GLfloat _ka[4];
	GLfloat _kd[4];
	GLfloat _ks[4];
	GLfloat _shininess;

	GLMat _mat;

	void DrawBoundingBox();
	void DrawBounding2DRect();

	GLMat RotThroughArbitary(float X, float Y, float Z, float angle);

public:
	FLModel(string folderPath);
	FLModel(string folderPath, float xPos, float yPos, float zPos);
	FLModel(string folderPath, CVector3 pos);
	FLModel(string folderPath, CVector3 pos, CVector3 rot);
	FLModel(string folderPath, CVector3 pos, CVector3 rot, string rotOrder);
	FLModel(string folderPath, float* mat);

	void Reset(string folderPath, float* mat);
	
	void SetTextureEnable(bool enable);
	bool IsTextureEnabled();
	void EnableWireFrame(bool enable);
	bool IsWireFrameEnabled();
	void EnableBoundingBox(bool enable);
	bool IsBoundingBoxEnabled();
	void EnableBounding2DRect(bool enable);
	bool IsBounding2DRectEnabled();

	void SetMeterial(int lightParam, float r, float g, float b, float a);
	void SetShininess(float val);

	void SetPos(float x, float y, float z);
	void SetPos(CVector3 pos);
	CVector3 GetPos();

	void AddTransInWorld(float x, float y, float z);
	void AddRotateInWorld(char axis, float angle);

	void AddTransInLocal(char axis, float move);
	void AddRotateInLocal(char axis, float angle);

	void Draw();

	void CalcBorder();
	void DrawBorder();
};
