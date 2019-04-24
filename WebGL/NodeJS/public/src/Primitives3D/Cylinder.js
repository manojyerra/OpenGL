
class Cylinder extends Primitive3D
{
	constructor()
	{
		super(Primitive3D.CYLINDER);
	}
	
	async initWithPosAndSize(x, y, z, r, h)
	{
		this.m[12] = x;
		this.m[13] = y;
		this.m[14] = z;

		this._r = r;
		this._h = h;

		this.initCommon();
	}
	
	async initWithMatAndSize(mat, r, h)
	{
		for(var i=0; i<16; i++)
		{
			this.m[i] = mat[i];
		}

		this._r = r;
		this._h = h;

		this.initCommon();
	}
	
	async initWithCylinder(cylinder)
	{
		for(var i=0; i<16; i++)
		{
			this.m[i] = cylinder.m[i];
		}

		this._r = cylinder.getRadius();
		this._h = cylinder.getHeight();
		this._id = cylinder.getID();

		await this.initCommon();
	}
	
	async initCommon()
	{
		this._vertexBufferID = 0;
		this._colorBufferID = 0;
		this._vertexCount = 0;
		this._randomColor = new RandomColor();

		this._shaderProgram = new ShaderProgram();		
		await this._shaderProgram.init("shaders/Primitive3D/Primitive3D.vs", "shaders/Primitive3D/Primitive3D.fs");

		this.generateBufferID();
	}

	getRadius()	{ return this._r; }
	getHeight()	{ return this._h; }

	setRadius(r)
	{ 
		if(r > 0)
			this._r = r; 
	}

	setHeight(h)
	{
		if(h > 0)
			this._h = h;
	}

	volume()
	{
		return PI_VAL * this._r * this._r * this._h;
	}

	draw()
	{
		if(!this._visible)
			return;

		this._shaderProgram.begin();

		this._scaleMat[0] = this._r * 2;
		this._scaleMat[5] = this._h;
		this._scaleMat[10] = this._r * 2;

		var projMatLoc = gl.getUniformLocation(this._shaderProgram.programID, "projMat");
		var modelMatLoc = gl.getUniformLocation(this._shaderProgram.programID, "modelMat");
		var oriMatLoc = gl.getUniformLocation(this._shaderProgram.programID, "oriMat");
		var scaleMatLoc = gl.getUniformLocation(this._shaderProgram.programID, "scaleMat");
				
		gl.uniformMatrix4fv(projMatLoc, false, cam3D.projMat.m);
		gl.uniformMatrix4fv(modelMatLoc, false, cam3D.modelMat.m);
		gl.uniformMatrix4fv(oriMatLoc, false, this.m);
		gl.uniformMatrix4fv(scaleMatLoc, false, this._scaleMat);

		var colorID = gl.getAttribLocation(this._shaderProgram.programID, "color");
		gl.enableVertexAttribArray(colorID);
		gl.bindBuffer(gl.ARRAY_BUFFER, this._colorBufferID);
		gl.vertexAttribPointer( colorID, 4, gl.UNSIGNED_BYTE, gl.FALSE, 0, 0);

		var vertexID = gl.getAttribLocation(this._shaderProgram.programID, "vertex");
		gl.enableVertexAttribArray(vertexID);
		gl.bindBuffer(gl.ARRAY_BUFFER, this._vertexBufferID);
		gl.vertexAttribPointer(vertexID, 3, gl.FLOAT, gl.FALSE, 0, 0);

		gl.drawArrays(gl.TRIANGLES, 0, this._vertexCount);
		
		gl.bindBuffer(gl.ARRAY_BUFFER, null);
		gl.disableVertexAttribArray(vertexID);
		gl.disableVertexAttribArray(colorID);

		this._shaderProgram.end();
	}
	
	//private methods...

	generateBufferID()
	{
		var buffer = new GLBuffer(true, false, false);

		var radius = 0.5;
		var halfLength = 0.5;
		var piVal = 3.14159265;

		buffer.glBegin(gl.TRIANGLES);

		for(var i=20; i<=360; i+=20)
		{
			var theta = (i-20)*piVal/180.0;
			var nextTheta = (i)*piVal/180.0;

			if(this._useRandomColors) 
				buffer.glColoruia(this._randomColor.nextColor(), this._randomColorAlpha);
			
			buffer.glVertex3f(radius*Math.cos(nextTheta),	halfLength,		radius*Math.sin(nextTheta));
			buffer.glVertex3f(radius*Math.cos(theta),		-halfLength,	radius*Math.sin(theta));
			buffer.glVertex3f(radius*Math.cos(theta),		halfLength,		radius*Math.sin(theta));

			buffer.glVertex3f(radius*Math.cos(theta),		-halfLength,	radius*Math.sin(theta));
			buffer.glVertex3f(radius*Math.cos(nextTheta),	halfLength,		radius*Math.sin(nextTheta));
			buffer.glVertex3f(radius*Math.cos(nextTheta),	-halfLength,	radius*Math.sin(nextTheta));

			if(this._useRandomColors) 
				buffer.glColoruia(this._randomColor.nextColor(), this._randomColorAlpha);
			
			buffer.glVertex3f(radius*Math.cos(nextTheta),	halfLength,		radius*Math.sin(nextTheta));

			if(this._useRandomColors) 
				buffer.glColoruia(this._randomColor.nextColor(), this._randomColorAlpha);
			
			buffer.glVertex3f(radius*Math.cos(theta),	halfLength,		radius*Math.sin(theta));
			buffer.glVertex3f(0, halfLength, 0);		

			if(this._useRandomColors) 
				buffer.glColoruia(this._randomColor.nextColor(), this._randomColorAlpha);
			
			buffer.glVertex3f(0, -halfLength, 0);
			
			if(this._useRandomColors) 
				buffer.glColoruia(this._randomColor.nextColor(), this._randomColorAlpha);
			
			buffer.glVertex3f(radius*Math.cos(theta),		-halfLength,	radius*Math.sin(theta));
			buffer.glVertex3f(radius*Math.cos(nextTheta),	-halfLength,	radius*Math.sin(nextTheta));
		}

		buffer.glEnd();

		this._vertexBufferID = buffer.getVertexBufferID();
		this._colorBufferID = buffer.getColorBufferID();

		this._vertexCount = buffer.getVertexCount();
	}	
}
































