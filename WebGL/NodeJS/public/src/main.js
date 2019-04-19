
//var shaderProgram = null;
var gl = null;
var floor = null;
var objModel = null;
var sw = 0;
var sh = 0;
var texture = null;


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
	canvas.oncontextmenu = () => false;

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

	gl.enable(gl.BLEND);
	gl.blendFunc(gl.SRC_ALPHA, gl.ONE_MINUS_SRC_ALPHA);
	gl.enable(gl.DEPTH_TEST);
		
	objModel = new ObjLoader();
	await objModel.init("./data/cottage");
	
		
	texture = new Texture();
	await texture.init('./data/Sample.png');
	texture.setBounds(2,1,10,5);
	
	addMouseEvents(canvas);
	addKeyEvents(document);
	
	floor = new Floor();
	await floor.init();
	
	sw = canvas.width;
	sh = canvas.height;
	
	cam.init(sw, sh, 1.0, 10000.0, 0.2);
	
	//shaderProgram = new ShaderProgram();
	//await shaderProgram.init(gl, './shaders/simple.vs', './shaders/simple.fs');
	
	drawScene();
}

function drawScene()
{
	gl.clearColor(0.2, 0.2, 0.2, 1.0);
	gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);
	gl.viewport(0, 0, sw, sh);
	
	cam.setModelViewMatrix();
	input.update();
	cam.updateCamera();
	
	texture.draw();
	
	//drawTriangle();
	
	floor.draw();
	objModel.draw();
		
	requestAnimationFrame(drawScene);
	//cancelAnimationFrame(requestId);
}

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

	
*/














