
class ShaderProgram
{
	constructor(gl, vertexShaderPath, fragmentShaderPath)
	{
		var vsText = loadTextFile(vertexShaderPath);
		var fsText = loadTextFile(fragmentShaderPath);
		
	}
		
	function loadTextFile(url) {
		
		var request = new XMLHttpRequest();
		request.open('GET', url + '?please-dont-cache=' + Math.random(), true);
		
		request.onload = function (text){
		};
		
		request.send();

		while(request.status == 0) {
			//await sleep(2);
			new Promise(resolve => setTimeout(resolve, 5))
		}
		var text = null;
		
		if (request.status < 200 || request.status > 299) {
			text = 'Error: HTTP Status ' + request.status + ' on resource ' + url;
		}
		else {
			text = request.responseText;
		}

		return text;
	}
	
	function CompileShader(gl, shaderFileData, shaderType)
	{
		var shaderObject =  gl.createShader(shaderType);
		gl.shaderSource(shaderObject, shaderFileData);
		gl.compileShader(shaderObject);

		if (!gl.getShaderParameter(shaderObject, gl.COMPILE_STATUS))
		{
			console.error('ERROR compiling shader : ', gl.getShaderInfoLog(shaderObject));
			return;
		}
		else if(gl.getShaderInfoLog(shaderObject))
		{
			console.error('Shader Compilation Log : ', gl.getShaderInfoLog(shaderObject));
		}
		
		return shaderObject;
	}

	function LinkShader(gl, programID, vertexShaderObj, fragmentShaderObj)
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