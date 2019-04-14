// Load a text resource from a file over the network
//var loadTextResource = function (url, callback) {
//	var request = new XMLHttpRequest();
//	request.open('GET', url + '?please-dont-cache=' + Math.random(), true);
//	request.send();	
//};

function sleep(ms) {
	return new Promise(resolve => setTimeout(resolve, ms));
}

async function loadTextResource(url) {
	
	var request = new XMLHttpRequest();
	request.open('GET', url + '?please-dont-cache=' + Math.random(), true);
	
	request.onload = function (text){
	};
	
	request.send();

	while(request.status == 0) {
		await sleep(2);
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

function createGLBuffer(gl, arr)
{
	var bufferID = gl.createBuffer();
	gl.bindBuffer(gl.ARRAY_BUFFER, bufferID);
	gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(arr), gl.STATIC_DRAW);
	return bufferID;
}

async function InitDemo() 
{
	console.log('From InitDemo method...');
	
	var shaderMgr = new ShadersManager('Manoj', 1);
	console.log(shaderMgr.add());
	
	var canvas = document.getElementById('game-surface');
	var gl = canvas.getContext('webgl');

	if (!gl) 
	{
		console.log('WebGL not supported, falling back on experimental-webgl');
		gl = canvas.getContext('experimental-webgl');
	}

	if (!gl)
	{
		alert('Your browser does not support WebGL');
	}
	
	gl.clearColor(0.2, 0.2, 0.2, 1.0);
	gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);
	
	var vsText = await loadTextResource('./shaders/simple.vs');
	var fsText = await loadTextResource('./shaders/simple.fs');	
	
	var vertexShaderObj = CompileShader(gl, vsText, gl.VERTEX_SHADER);
	var fragmentShaderObj = CompileShader(gl, fsText, gl.FRAGMENT_SHADER);
	
	var programID = gl.createProgram();
	console.log('programID : ', programID);  
	
	LinkShader(gl, programID, vertexShaderObj, fragmentShaderObj);
	
	gl.deleteShader(vertexShaderObj);
	gl.deleteShader(fragmentShaderObj);
	
	var triangleVertices = 
	[
		0.0, 0.5, 0.0,    
		-0.5, -0.5, 0.0,  
		0.5, -0.5, 0.0
	];

	var vertexBufID = createGLBuffer(gl, triangleVertices);

	gl.useProgram(programID);
	
	var vertexLoc = gl.getAttribLocation(programID, 'vertex');
	gl.enableVertexAttribArray(vertexLoc);
	gl.bindBuffer(gl.ARRAY_BUFFER, vertexBufID);
	gl.vertexAttribPointer(vertexLoc,3,gl.FLOAT,gl.FALSE,0,0);

	gl.drawArrays(gl.TRIANGLES, 0, 3);	
	
	gl.useProgram(null);	
}


/*
function loadTextResource(url)
{
	var request = new XMLHttpRequest();
	
	request.open('GET', url + '?please-dont-cache=' + Math.random(), true);
	
	request.onload = function ()
	{
		if (request.status < 200 || request.status > 299) 
		{
			vsText = 'Error: HTTP Status ' + request.status + ' on resource ' + url;
		}
		else 
		{
			vsText = request.responseText;
		}
	};
	
	request.send();
};

	// loadTextResource('./temp.txt', function (vsErr, vsText) {
		// if (vsErr) {
			// alert('Fatal error getting vertex shader (see console)');
			// console.error(vsErr);
		// } else {
			// console.log('temp.txt file text : ', vsText);
		// }
	// });


*/