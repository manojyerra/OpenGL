#ifndef ShadersManager_H
#define ShadersManager_H

#include "ShaderProgram.h"
#include <string>
#include <map>

using namespace std;

class ShaderInfo
{
private: 
	ShaderProgram* _shaderProgram;
	int _refCount;

public: 
	ShaderInfo(ShaderProgram* shaderProgram)
	{
		_shaderProgram = shaderProgram;
		_refCount = 1;
	}

	~ShaderInfo()
	{
	}

	int GetRefCount()
	{
		return _refCount;
	}

	void IncreaseRefCount()
	{
		_refCount++;
	}

	void DecreaseRefCount()
	{
		_refCount--;

		if (_refCount < 0)
		{
			return;  //Reference count should not negative value.
		}
	}

	ShaderProgram* GetShaderProgram()
	{
		return _shaderProgram;
	}
};


class ShadersManager
{
private:
	static ShadersManager* _ref;
	map<string, ShaderInfo*> shadersMap;

	ShadersManager();
	~ShadersManager();

public:
	static ShadersManager* GetInstance();

	ShaderProgram* CreateShaderProgram(string vertexShader, string fragmentShader);
	void DeleteShaderProgram(ShaderProgram* shaderProgram);

	static void DeleteInstance();
};


#endif