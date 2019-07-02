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

	void Init(string filePath, int type);

public:
	static const int FLM_WITHOUT_INDEX_SIZE = 1;
	static const int FLM_WITH_INDEX_SIZE = 2;

    FLModel(string folderPath);
	FLModel(string folderPath, int type);
    void Draw();
};
