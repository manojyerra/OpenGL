#include "ShaderProgram.h"


ShaderProgram::ShaderProgram(string vertexShaderFilePath, string fragmentShaderFilePath)
{
	_programID = 0;
	_vertexShaderPath = vertexShaderFilePath;
	_fragmentShaderPath = fragmentShaderFilePath;

	printf("\nCompiling vertex shader...\n");
	GLint vertexShaderObj = CompileShader(_vertexShaderPath.c_str(), GL_VERTEX_SHADER);

	if (vertexShaderObj == -1)
	{
		throw std::exception("Compiling vertex shader failed.");
		return;
	}

	printf("\nCompiling fragment shader...\n");
	GLint fragmentShaderObj = CompileShader(_fragmentShaderPath.c_str(), GL_FRAGMENT_SHADER);

	if (fragmentShaderObj == -1)
	{
		throw std::exception("Compiling fragment shader failed.");
		return;
	}

	// Link the program
	printf("\nLinking program\n");

	_programID = glCreateProgram();
	glAttachShader(_programID, vertexShaderObj);
	glAttachShader(_programID, fragmentShaderObj);
	glLinkProgram(_programID);

	// Check the program
	GLint linkStatus = GL_FALSE;
	int infoLogLength;

	glGetProgramiv(_programID, GL_LINK_STATUS, &linkStatus);
	glGetProgramiv(_programID, GL_INFO_LOG_LENGTH, &infoLogLength);
	
	glDetachShader(_programID, vertexShaderObj);
	glDetachShader(_programID, fragmentShaderObj);
	
	glDeleteShader(vertexShaderObj);
	glDeleteShader(fragmentShaderObj);

	if(!linkStatus)
	{
		printf("\nLinking failed...");

		if(infoLogLength > 0)
		{
			std::vector<char> programInfoLog(infoLogLength+1);
			glGetProgramInfoLog(_programID, infoLogLength, NULL, &programInfoLog[0]);
			printf("%s\n", &programInfoLog[0]);
		}

		throw std::exception("\nLinking failed...");
		return;
	}
	else
	{
		if(infoLogLength > 0)
		{
			std::vector<char> programInfoLog(infoLogLength+1);
			glGetProgramInfoLog(_programID, infoLogLength, NULL, &programInfoLog[0]);
			printf("%s\n", &programInfoLog[0]);
		}
	}
}

unsigned int ShaderProgram::GetFileLength(string filePath)
{
	FILE  *fp = fopen(filePath.c_str(), "rb");
    
    if(fp)
    {
        fseek(fp, 0, SEEK_END);
        unsigned int fileSize = (unsigned int)ftell(fp);
        fseek(fp, 0, SEEK_SET);
        fclose(fp);
        return fileSize;
    }
    
    return 0;
}


GLint ShaderProgram::CompileShader(const char* shaderFilePath, GLenum shaderType)
{
	unsigned int shaderFileLen = GetFileLength( shaderFilePath );
	
	if(shaderFileLen == 0)
	{
		printf("\nError : Shader file length is zero.\n");
		return -1;
	}

	FILE* fp = fopen(shaderFilePath, "rb");
	char* shaderFileData = NULL;

    if(fp)
    {
		shaderFileData = (char*)malloc(shaderFileLen+1);
		memset(shaderFileData, (int)'\0', shaderFileLen+1);
		fread(shaderFileData, 1, shaderFileLen, fp);
		fclose(fp);
	}
	else
	{
		printf("\nError : Unable to open shader file.\n");
		return -1;	//unable to open file.
	}

	GLuint shaderObject =  glCreateShader(shaderType);
	glShaderSource(shaderObject, 1, (const char**)(&(shaderFileData)), NULL);
	glCompileShader(shaderObject);

	free(shaderFileData);

	GLint shaderCompileStatus = GL_FALSE;
	int compilationLogLength;

	glGetShaderiv(shaderObject, GL_COMPILE_STATUS, &shaderCompileStatus);
	glGetShaderiv(shaderObject, GL_INFO_LOG_LENGTH, &compilationLogLength);

	std::vector<char> shaderCompileLog(compilationLogLength+1);
	glGetShaderInfoLog(shaderObject, compilationLogLength, NULL, &shaderCompileLog[0]);

	if(!shaderCompileStatus)
	{
		printf("\nCompilation Failed");
			
		if(compilationLogLength > 0)
			printf("\n%s\n", &shaderCompileLog[0]);

		return -1;
	}
	else
	{
		if(compilationLogLength > 0)
			printf("\n%s\n", &shaderCompileLog[0]);
	}

	return shaderObject;
}

GLuint ShaderProgram::ProgramID()
{
	return _programID;
}

void ShaderProgram::Begin()
{
	glUseProgram(_programID);
}

void ShaderProgram::End()
{
	glUseProgram(0);
}

string ShaderProgram::GetVertexShaderFilePath()
{
	return _vertexShaderPath;
}

string ShaderProgram::GetFragmentShaderFilePath()
{
	return _fragmentShaderPath;
}


ShaderProgram::~ShaderProgram()
{
	if(_programID)
		glDeleteProgram(_programID);
}
