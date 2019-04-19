
class ShaderProgram
{
	constructor()
	{
	}
	
	async init(vertexShaderPath, fragmentShaderPath)
	{
		var vsText = await loadTextFile(vertexShaderPath);
		var fsText = await loadTextFile(fragmentShaderPath);	
		
		var vertexShaderObj = this.compileShader(vsText, gl.VERTEX_SHADER);
		var fragmentShaderObj = this.compileShader(fsText, gl.FRAGMENT_SHADER);
		
		this.programID = gl.createProgram();
		
		this.linkShader(this.programID, vertexShaderObj, fragmentShaderObj);
		
		gl.deleteShader(vertexShaderObj);
		gl.deleteShader(fragmentShaderObj);
	}
	
	begin()
	{
		gl.useProgram(this.programID);
	}
	
	end()
	{
		gl.useProgram(null);
	}
	
	compileShader(shaderFileData, shaderType)
	{
		var shaderObject =  gl.createShader(shaderType);
		gl.shaderSource(shaderObject, shaderFileData);
		gl.compileShader(shaderObject);

		var shaderTypeName = "";

		if(shaderType == gl.VERTEX_SHADER) {
			shaderTypeName = "Vertex Shader";
		}
		else if(shaderType == gl.FRAGMENT_SHADER) {
			shaderTypeName = "Fragment Shader";
		}

		if (!gl.getShaderParameter(shaderObject, gl.COMPILE_STATUS))
		{
			console.error("ERROR compiling shader ("+shaderTypeName+") : "+gl.getShaderInfoLog(shaderObject));
			return;
		}
		else if(gl.getShaderInfoLog(shaderObject))
		{
			console.error("Shader ("+shaderTypeName+") Compilation Log : "+gl.getShaderInfoLog(shaderObject));
		}
		
		return shaderObject;
	}

	linkShader(programID, vertexShaderObj, fragmentShaderObj)
	{
		gl.attachShader(programID, vertexShaderObj);
		gl.attachShader(programID, fragmentShaderObj);
		gl.linkProgram(programID);
		
		if(!gl.getProgramParameter(programID, gl.LINK_STATUS))
		{
			console.error('ERROR linking shader : ', gl.getProgramInfoLog(programID));
			return;
		}
		else if(gl.getProgramInfoLog(programID))
		{
			console.error('Shader Linking Log : ', gl.getProgramInfoLog(programID));		
		}
			
		gl.detachShader(programID, vertexShaderObj);
		gl.detachShader(programID, fragmentShaderObj);
	}
	
}