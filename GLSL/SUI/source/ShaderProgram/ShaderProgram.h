#ifndef ShaderProgram_H
#define ShaderProgram_H

#include <GL/glew.h>

#include <string>
#include <vector>
using namespace std;

class ShaderProgram
{
private:
	GLuint _programID;
	string _vertexShaderPath;
	string _fragmentShaderPath;

	unsigned int GetFileLength(string filePath);
	GLint CompileShader(const char* shaderFilePath, GLenum shaderType);

public:
	ShaderProgram(string vertexShaderFilePath, string fragmentShaderFilePath);
	~ShaderProgram();

	GLuint ProgramID();

	void Begin();
	void End();

	string GetVertexShaderFilePath();
	string GetFragmentShaderFilePath();

};

#endif
