
var gl = null;
var looper = null;

function createGLBuffer(gl, arr)
{
	var bufferID = gl.createBuffer();
	gl.bindBuffer(gl.ARRAY_BUFFER, bufferID);
	gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(arr), gl.STATIC_DRAW);
	return bufferID;
}

async function initDemo() 
{
	console.log('From InitDemo method...');	
	
	var canvas = document.getElementById('game-surface');
	canvas.width = 900;
	canvas.height = 700;
	canvas.oncontextmenu = () => false;

	addMouseEvents(canvas);
	addKeyEvents(document);

	gl = canvas.getContext('webgl');
	//gl = canvas.getContext('webgl', {antialias: false} );

	if (!gl) 
	{
		console.log('WebGL not supported, falling back on experimental-webgl');
		gl = canvas.getContext('experimental-webgl');
	}

	if (!gl)
	{
		alert('Your browser does not support WebGL');
	}	
	
	looper = new Looper();
	await looper.init(canvas.width, canvas.height);
	
	drawScene();
}

function drawScene()
{	
	input.update();
	this.looper.draw();
	requestAnimationFrame(drawScene);
}

function addKeyEvents(document)
{
	document.addEventListener('keyup',keyUpListener,false);	
	document.addEventListener('keydown',keyDownListener,false);	
	
	function keyUpListener(e) {
		input.storeKeys[e.keyCode] = false;
	}

	function keyDownListener(e) {
		input.storeKeys[e.keyCode] = true;
	}	
}

function addMouseEvents(canvas)
{	
	canvas.addEventListener("mousedown", function (e) {
		
		var pos = getMousePos(e);  

		input.mouseX.store = pos.x;
		input.mouseY.store = pos.y;
		
		if(e.button == 0) {
			input.leftMouseDown.store = true;
		}
		else if(e.button == 1) {
			input.middleMouseDown.store = true;
		}
		else if(e.button == 2) {
			input.rightMouseDown.store = true;
		}

		return false;
		
	}, false);

	canvas.addEventListener("mouseup", function (e) {
		
		var pos = getMousePos(e); 
		
		input.mouseX.store = pos.x;
		input.mouseY.store = pos.y;
		
		if(e.button == 0) {
			input.leftMouseDown.store = false;
		}
		else if(e.button == 1) {
			input.middleMouseDown.store = false;
		}
		else if(e.button == 2) {
			input.rightMouseDown.store = false;
		}

		return false;
		
	}, false);

	canvas.addEventListener("mousemove", function (e) {
		
		var pos = getMousePos(e);
		input.mouseX.store = pos.x;
		input.mouseY.store = pos.y;		
		
	}, false);
		
	canvas.addEventListener('wheel',function(e){
		
		if(e.wheelDelta < 0) {
			input.scrollVal.store = -1;
		}
		else {
			input.scrollVal.store = 1;
		}

		return false;
		
	}, false);
	
	function getMousePos(mouseEvent) {
		var rect = canvas.getBoundingClientRect();
		return {
			x: mouseEvent.clientX - rect.left,
			y: mouseEvent.clientY - rect.top
		};
	}	
}


// canvas.addEventListener("mouseup", function (e) {
  // drawing = false;
// }, false);

// canvas.addEventListener("mousemove", function (e) {
  // mousePos = getMousePos(canvas, e);
// }, false);


// function getMousePos(canvasDom, mouseEvent) {
  // var rect = canvasDom.getBoundingClientRect();
  // return {
    // x: mouseEvent.clientX - rect.left,
    // y: mouseEvent.clientY - rect.top
  // };
// }



/*
	//cancelAnimationFrame(requestId);



function drawTriangle()
{
	var triangleVertices = 
	[
		0.0, 0.5, 0.0,    
		-0.5, -0.5, 0.0,  
		0.5, -0.5, 0.0
	];

	var vertexBufID = createGLBuffer(gl, triangleVertices);
	
	shaderProgram.begin();
		
	var projMatLoc = gl.getUniformLocation(shaderProgram.programID, "projMat");
	var modelMatLoc = gl.getUniformLocation(shaderProgram.programID, "modelMat");
	
	gl.uniformMatrix4fv(projMatLoc, false, cam.projMat.m);
	gl.uniformMatrix4fv(modelMatLoc, false, cam.modelMat.m);
	
	var vertexLoc = gl.getAttribLocation(shaderProgram.programID, "vertex");
	gl.enableVertexAttribArray(vertexLoc);
	gl.bindBuffer(gl.ARRAY_BUFFER, vertexBufID);
	gl.vertexAttribPointer(vertexLoc,3,gl.FLOAT,gl.FALSE,0,0);

	gl.drawArrays(gl.TRIANGLES, 0, 3);
	
	shaderProgram.end();	
}



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

	
	

	var arr = new Uint8Array(3);
	arr[0] = 1;
	arr[1] = 2;
	arr[2] = 3;
	
	var newArr = new Uint8Array(5);
	newArr[0] = 11;
	newArr[1] = 12;
	newArr[2] = 13;
	newArr[3] = 14;
	
	newArr.set(arr);
	
	//var dstU8 = new Uint8Array(10);
	//var srcU8 = new Uint8Array(3);
	//dstU8.set(srcU8);
		
	console.log('arr : ',arr);
	console.log('newArr : ',newArr);
	
	arr = null;

	console.log('after...');
	console.log('arr : ',arr);
	console.log('newArr : ',newArr);

	
	//requestAnimationFrame(function() {
    //   drawScene(sw, sh);
    //});


	if(input.IsMouseClicked())
	{
		//console.log('mosue clicked');
	}
	if(input.IsMiddleMouseClicked())
	{
		//console.log('middle mouse clicked');
	}
	if(input.IsRightMouseClicked())
	{
		//console.log('right mouse clicked');
	}
	if(input.IsRightMouseDragged())
	{
		//console.log('IsRightMouseDragged : true');
	}
	
	if(input.IsMouseReleased())
	{
		//console.log('mosue Released');
	}
	if(input.IsMiddleMouseReleased())
	{
		//console.log('middle mouse Released');
	}
	if(input.IsRightMouseReleased())
	{
		//console.log('right mouse Released');
	}


	//gl.disable(gl.SAMPLE_COVERAGE);
	//gl.disable(gl.SAMPLE_ALPHA_TO_COVERAGE);
	//gl.sampleCoverage(0.0, false);

	//console.log('sampleCoverageValue : ',gl.getParameter(gl.SAMPLE_COVERAGE_VALUE));  // 0.5
	//console.log('SAMPLE_COVERAGE_INVERT : ', gl.getParameter(gl.SAMPLE_COVERAGE_INVERT)); // false


	// gl.disable(gl.MULTISAMPLE);
	// gl.disable(gl.SAMPLE_COVERAGE);
	// gl.disable(gl.SAMPLE_ALPHA_TO_COVERAGE);
	// gl.sampleCoverage(0.0, true);	
*/














