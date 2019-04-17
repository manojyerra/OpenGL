
var shaderProgram = null;
var gl = null;

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
		
	var canvas = document.getElementById('game-surface');
	gl = canvas.getContext('webgl');

	if (!gl) 
	{
		console.log('WebGL not supported, falling back on experimental-webgl');
		gl = canvas.getContext('experimental-webgl');
	}

	if (!gl)
	{
		alert('Your browser does not support WebGL');
	}
	
	var sw = canvas.width;
	var sh = canvas.height;
	
	
	var _zNear = 1.0;
	var _zFar = 10000.0;
	var _zNearPlaneW = 0.2;
	
	var _zNearPlaneHalfW = _zNearPlaneW/2.0;

	var _left = -_zNearPlaneHalfW;
	var _right = _zNearPlaneHalfW;
	var _bottom = -_zNearPlaneHalfW*sh/sw;
	var _top = _zNearPlaneHalfW*sh/sw;
	
	var glMat = new GLMat();
	
	console.log('_left : ',_left);
	console.log('_right : ',_right);
	console.log('_bottom : ',_bottom);
	console.log('_top : ',_top);
	console.log('_zNear : ',_zNear);
	console.log('_zFar : ',_zFar);
	
	glMat.glOrtho(_left, _right, _bottom, _top, _zNear, _zFar);
	
	console.log('frustum : ',glMat.m);

	
	cam.init(sw, sh, 1.0, 10000.0, 0.2);
	
	shaderProgram = new ShaderProgram();
	await shaderProgram.init(gl, './shaders/simple.vs', './shaders/simple.fs');

	//requestAnimationFrame(drawScene);
	
	drawScene(sw, sh);
}

function drawScene(sw, sh)
{
	gl.clearColor(0.2, 0.2, 0.2, 1.0);
	gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);
	gl.viewport(0, 0, sw, sh);
	
	cam.setModelViewMatrix();
	
	console.log('cam.modelMat.m : ',cam.modelMat.m);
	
	var triangleVertices = 
	[
		0.0, 0.5, 0.0,    
		-0.5, -0.5, 0.0,  
		0.5, -0.5, 0.0
	];

	var vertexBufID = createGLBuffer(gl, triangleVertices);
	
	shaderProgram.begin();
		
	var projMatLoc = gl.getUniformLocation(shaderProgram.programID, 'projMat');
	var modelMatLoc = gl.getUniformLocation(shaderProgram.programID, 'modelMat');
	
	gl.uniformMatrix4fv(projMatLoc, false, cam.projMat.m);
	gl.uniformMatrix4fv(modelMatLoc, false, cam.modelMat.m);
	
	var vertexLoc = gl.getAttribLocation(shaderProgram.programID, 'vertex');
	gl.enableVertexAttribArray(vertexLoc);
	gl.bindBuffer(gl.ARRAY_BUFFER, vertexBufID);
	gl.vertexAttribPointer(vertexLoc,3,gl.FLOAT,gl.FALSE,0,0);

	gl.drawArrays(gl.TRIANGLES, 0, 3);	
	
	shaderProgram.end();
	
	console.log('from drawScene');
	
	//requestAnimationFrame(drawScene);
}
 



/*

	var tempText = await loadTextFile('./temp.obj');

	var lines = tempText.split('\n');
	
    for(var i = 0; i < lines.length; i++)
	{
		var line = lines[i];
		
		if(line.startsWith("v "))
		{
			console.log('vertex : ', line);
		}
		else
		{
			console.log(line);
		}
    }

*/














