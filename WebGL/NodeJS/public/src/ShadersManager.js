var shadersManagerInstanceCount = 0;

class ShaderInfo
{
	constructor(shaderProgram)
	{
		this._shaderProgram = shaderProgram;
		this._refCount = 1;
	}
	
	getRefCount()
	{
		return this._refCount;
	}

	increaseRefCount()
	{
		this._refCount++;
	}

	decreaseRefCount()
	{
		this._refCount--;

		if (this._refCount < 0)
		{
			throw "Reference count should not be negative number";
		}
	}

	getShaderProgram()
	{
		return this._shaderProgram;
	}
}

	
class ShadersManager
{
	constructor()
	{
		shadersManagerInstanceCount++;
		
		if(shadersManagerInstanceCount > 1)
		{
			throw "More than one object creation is not allowed for ShadersManager Class";
		}
		
		this._shadersMap = new Map(); 
		
	}
	
	async createShaderProgram(vertexShader, fragmentShader)
	{
		var key = vertexShader+":"+fragmentShader;

		var shaderProgram = null;

		if (this._shadersMap.has(key))
		{
			console.log("Shader Manager already contains this shader");
			var shaderInfo = this._shadersMap.get(key);
			shaderInfo.increaseRefCount();
			shaderProgram = shaderInfo.getShaderProgram();			
		}
		else
		{
			console.log("Shader Manager doesn't contains this shader");
			var shaderProgram = new ShaderProgram();
			await shaderProgram.init(vertexShader, fragmentShader);
			var shaderInfo = new ShaderInfo(shaderProgram);
			this._shadersMap.set(key, shaderInfo);
		}

		return shaderProgram;
	}
	
	deleteShaderProgram(shaderProgram)
	{
		var vertexShaderPath = shaderProgram.getVertexShaderFilePath();
		var fragmentShaderPath = shaderProgram.getFragmentShaderFilePath();

		var key = vertexShaderPath+":"+fragmentShaderPath;

		if (this._shadersMap.has(key))
		{
			var shaderInfo = this._shadersMap.get(key);
			shaderInfo.decreaseRefCount();

			if (shaderInfo.getRefCount() == 0)
			{
				shaderInfo.getShaderProgram().delete();
				this._shadersMap.delete(key);
			}
		}
	}	
}


var shadersManager = new ShadersManager();
