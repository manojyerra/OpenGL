
class Looper
{
	constructor()
	{
	}
	
	async init(sw, sh)
	{	
		this._sw = sw;
		this._sh = sh;
	
		cam3D.init(sw, sh, 1.0, 10000.0, 0.2);
		cam2D.init(sw, sh);
		
		this.floor = new Floor();
		await this.floor.init();
		
		//await this.load3DPrimitives();
		
		//this.objModel = new ObjLoader();
		//await this.objModel.init("./data/objModels/alien");
		
		this.flmModel = new FLMModel();
		await this.flmModel.init("./data/flmModels/alien");
		
		//this.texture = new GLTexture();
		//await this.texture.init('./data/Sample32Bit.png', true);
		//this.texture.setBounds(100,200,sw,sh);
		//this.texture.setBounds(1,2,5,2);		
		
		//this.fbo = new GLFBO(sw, sh);		
	}
	
	draw()
	{
		gl.enable(gl.BLEND);
		gl.blendFunc(gl.SRC_ALPHA, gl.ONE_MINUS_SRC_ALPHA);
		gl.enable(gl.DEPTH_TEST);
		
		gl.viewport(0, 0, this._sw, this._sh);
		//this.fbo.bind();
		
		gl.clearColor(0.2, 0.2, 0.2, 1.0);
		gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);
		
		cam3D.updateCamera();
		
		//this.texture.setBounds(1,2,5,2);
		//this.texture.draw(cam3D.projMat.m, cam3D.modelMat.m);
		
		this.floor.draw();
		this.flmModel.draw();
		//this.objModel.draw();
		
		//this.box.draw();
		//this.cone.draw();
		//this.cylinder.draw();
		//this.sphere.draw();

		//gl.disable(gl.DEPTH_TEST);
		//this.texture.setBounds(10,10,500,500);
		//this.texture.draw(cam2D.projMat.m, cam2D.modelMat.m);
		//gl.enable(gl.DEPTH_TEST);

		//this.fbo.unBind();

		//gl.clearColor(0.2, 0.2, 0.2, 1.0);
		//gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);
		//gl.enable(gl.DEPTH_TEST);
		
		//this.rect.draw();
		//this.triangle.draw();
	}
	
	
	//private methods...
	
	async load3DPrimitives()
	{
		this.box = new Box();
		await this.box.initWithPosAndSize(0,0,0, 2,3,4);
		this.box.setSize(3, 1, 6);
		this.box.setPos(-10, 0, -10);
		
		this.cone = new Cone();
		await this.cone.initWithPosAndSize(0, 0, 0, 2, 3);
		this.cone.setRadius(1.5);
		this.cone.setHeight(2);
		this.cone.setPos(-5, 0, -10);
		
		this.cylinder = new Cylinder(0, 0, 0, 3, 2);
		await this.cylinder.initWithPosAndSize();
		this.cylinder.setRadius(1.5);
		this.cylinder.setHeight(2);
		this.cylinder.setPos(0, 0, -10);
		
		this.sphere = new Sphere();
		await this.sphere.initWithPosAndSize(0, 0, 0, 2);
		this.sphere.setPos(5, 0, 0);
		this.sphere.setRadius(5);
		
		this.rect = new Rect();
		await this.rect.init(100, 100, 200, 300);

		this.triangle = new Triangle();
		await this.triangle.init(new CVector3(100,100,0), new CVector3(200,200,0), new CVector3(50,200,0));
		this.triangle.setColor4ub(0, 128, 0, 255);		
	}	
}



/*
	var prevClearColor = GLUtils.clearColor(1.0, 0.0, 1.0, 1.0);
	var prevEnableVal = GLUtils.enable(gl.BLEND, true);
	var prevLineWidth = GLUtils.lineWidth(2.0);
	//var prevDepthFunc = GLUtils.depthFunc(1.0);
	
	//var projValues = GLUtils.getProjectionValues(cam3D.projMat.m);
	//console.log('projValues : ', projValues);
	
	//var pos2D = GLUtils.get2DPosOnScreenFrom3DPos(new CVector3(0.0, 0.0, 0.0), cam3D.modelMat.m, cam3D.projMat.m, sw, sh);
	//console.log('2DPos : ', pos2D);
	
	var pos3DVec = Array(0);
	pos3DVec.push(0.0);
	pos3DVec.push(0.0);
	pos3DVec.push(0.0);
	pos3DVec.push(1.0);
	pos3DVec.push(0.0);
	pos3DVec.push(0.0);
	pos3DVec.push(0.0);
	pos3DVec.push(1.0);
	pos3DVec.push(0.0);
	
	var pos2DVec = GLUtils.get2DPosArrOnScreenFrom3DPosArr(pos3DVec, 3, cam3D.modelMat.m, cam3D.projMat.m, sw, sh);
	console.log('pos2DVec : ',pos2DVec);
	
	var pos2DOnRect = GLUtils.getVerticesOnRect(pos3DVec, 3, cam3D.modelMat.m, cam3D.projMat.m, null, 0,0,450,450, sw, sh);
	console.log('pos2DOnRect : ',pos2DOnRect);
	
	var minMax = GLUtils.getMinMaxPoints(pos3DVec);
	console.log('minMax : ',minMax);



	var binaryData = await loadArrayBuffer('data/floatArr.buf');
	var floatArr = new Float32Array(binaryData);
	console.log(floatArr[0]);
	console.log(floatArr[1]);
	console.log(floatArr[2]);
	console.log(floatArr[3]);
	console.log(floatArr[4]);
	console.log(floatArr[5]);
*/