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
	unsigned int _numIndices;

	unsigned int _textureID;
	unsigned int _numVertex;

public:
    FLModel(string filePath);
    void Draw();
};
