#include <windows.h>
#include <gl/gl.h>
#include <vector>
#include <string>
using namespace std;

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

	float _minX;
	float _maxX;
	float _minY;
	float _maxY;
	float _minZ;
	float _maxZ;

public:
    FLModel(string filePath);
    void Draw();
	void DrawBoundingBox();

};
