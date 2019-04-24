
class Sphere extends Primitive3D
{
	constructor()
	{
		super(Primitive3D.SPHERE);
	}
	
	async initWithPosAndSize(x, y, z, r)
	{
		this.m[12] = x;
		this.m[13] = y;
		this.m[14] = z;
		
		this._r = r;
		
		await this.initCommon();		
	}
	
	async initWithSphere(sphere)
	{
		for(var i=0; i<16; i++)
		{
			this.m[i] = sphere.m[i];
		}
		
		this._r = sphere.getRadius();
		
		await this.initCommon();
	}

	async initWithMatAndSize(mat, r)
	{
		for(var i=0; i<16; i++)
		{
			this.m[i] = mat[i];
		}
		
		this._r = r;
		
		await this.initCommon();
	}

	async initCommon()
	{
		this._vertexBufferID = 0;
		this._normalBufferID = 0;
		this._vertexCount = 0;
		this._randomColor = new RandomColor();

		this._shaderProgram = new ShaderProgram();
		await this._shaderProgram.init("shaders/Primitive3D/Sphere.vs", "shaders/Primitive3D/Sphere.fs");

		this.generateBufferID();
	}
	
	getRadius()
	{
		return this._r;
	}

	setRadius(r)
	{
		if(r > 0)
			this._r = r;
	}

	volume()
	{
		return (4.0/3.0) * PI_VAL * this._r * this._r * this._r;
	}
	
	draw()
	{
		if(!this._visible)
			return;
		
		this._scaleMat[0] = this._r;
		this._scaleMat[5] = this._r;
		this._scaleMat[10] = this._r;
		
		this._shaderProgram.begin();		

		var projMatLoc = gl.getUniformLocation(this._shaderProgram.programID, "projMat");
		var modelMatLoc = gl.getUniformLocation(this._shaderProgram.programID, "modelMat");
		var normalMatLoc = gl.getUniformLocation(this._shaderProgram.programID, "normalMat");
		var oriMatLoc = gl.getUniformLocation(this._shaderProgram.programID, "oriMat");
		var scaleMatLoc = gl.getUniformLocation(this._shaderProgram.programID, "scaleMat");		
		
		gl.uniformMatrix4fv(projMatLoc, false, cam3D.projMat.m);
		gl.uniformMatrix4fv(modelMatLoc, false, cam3D.modelMat.m);
		gl.uniformMatrix3fv(normalMatLoc, false, cam3D.normalMat);
		gl.uniformMatrix4fv(oriMatLoc, false, this.m);
		gl.uniformMatrix4fv(scaleMatLoc, false, this._scaleMat);		
		
		gl.uniform3f(gl.getUniformLocation(this._shaderProgram.programID, "lightPos"), 0.0, 0.0, 0.0);
		gl.uniform4f(gl.getUniformLocation(this._shaderProgram.programID, "ambient"), 0.2, 0.2, 0.2, 1.0);
		gl.uniform4f(gl.getUniformLocation(this._shaderProgram.programID, "diffuse"), 0.8, 0.8, 0.8, 1.0);
		gl.uniform4f(gl.getUniformLocation(this._shaderProgram.programID, "specular"), 0.0, 0.0, 0.0, 1.0);
		gl.uniform1f(gl.getUniformLocation(this._shaderProgram.programID, "shininess"), 0.52);

		var vertexID = gl.getAttribLocation(this._shaderProgram.programID, "vertex");
		gl.enableVertexAttribArray(vertexID);
		gl.bindBuffer(gl.ARRAY_BUFFER, this._vertexBufferID);
		gl.vertexAttribPointer( vertexID, 3, gl.FLOAT, gl.FALSE, 0, 0);

		var normalID = gl.getAttribLocation(this._shaderProgram.programID, "normal");
		gl.enableVertexAttribArray(normalID);
		gl.bindBuffer(gl.ARRAY_BUFFER, this._normalBufferID);
		gl.vertexAttribPointer( normalID, 3, gl.FLOAT, gl.TRUE, 0, 0);

		gl.drawArrays(gl.TRIANGLES, 0, this._vertexCount);
		
		this._shaderProgram.end();
	}
	
	//private methods...

	generateBufferID()
	{
		var buffer = new GLBuffer(false, false, true);

		var radius = 0.5;
		var piVal = 3.14159265;

		buffer.glBegin(gl.TRIANGLES);
		
		var degToRad = 0.017453;

		var delta = 5;
		
		var x = radius;
		var y = 0;
		var z = 0;

		for(var zAngle=-90; zAngle<90; zAngle+=2*delta)
		{
			var posZ1 = this.rot(3, zAngle, x, y, z);
			var posZ2 = this.rot(3, zAngle+2*delta,	x, y, z);

			for(var yAngle = 0; yAngle<=360; yAngle+=delta)
			{
				var pos1 = this.rot(2, yAngle, posZ1.x, posZ1.y, posZ1.z);
				var pos2 = this.rot(2, yAngle, posZ2.x, posZ2.y, posZ2.z);

				var pos3 = this.rot(2, yAngle+delta, posZ1.x, posZ1.y, posZ1.z);
				var pos4 = this.rot(2, yAngle+delta, posZ2.x, posZ2.y, posZ2.z);

				buffer.glNormal3f(pos3.x, pos3.y, pos3.z);
				buffer.glVertex3f(pos3.x, pos3.y, pos3.z);

				buffer.glNormal3f(pos2.x, pos2.y, pos2.z);
				buffer.glVertex3f(pos2.x, pos2.y, pos2.z);

				buffer.glNormal3f(pos1.x, pos1.y, pos1.z);
				buffer.glVertex3f(pos1.x, pos1.y, pos1.z);

				buffer.glNormal3f(pos2.x, pos2.y, pos2.z);
				buffer.glVertex3f(pos2.x, pos2.y, pos2.z);

				buffer.glNormal3f(pos3.x, pos3.y, pos3.z);
				buffer.glVertex3f(pos3.x, pos3.y, pos3.z);

				buffer.glNormal3f(pos4.x, pos4.y, pos4.z);
				buffer.glVertex3f(pos4.x, pos4.y, pos4.z);
			}
		}

		buffer.glEnd();

		this._vertexBufferID = buffer.getVertexBufferID();
		this._normalBufferID = buffer.getNormalBufferID();
		this._vertexCount = buffer.getVertexCount();
	}	
	
	rot(axis, angleInDegrees, x, y, z)
	{
		var angleInRadians = angleInDegrees*(3.14159) / 180.0;		//converting to radiuns

		var cosVal = Math.cos(angleInRadians);
		var sinVal = Math.sin(angleInRadians);

		var	 newP = new CVector3();
		
		if(axis == 3)
		{
			newP.x = x*cosVal - y*sinVal;
			newP.y = x*sinVal + y*cosVal;
			newP.z = z;
		}
		else if(axis == 1)
		{
			newP.y = y*cosVal - z*sinVal;
			newP.z = y*sinVal + z*cosVal;
			newP.x = x;
		}
		else if(axis == 2)
		{
			newP.z = z*cosVal - x*sinVal;
			newP.x = z*sinVal + x*cosVal;
			newP.y = y;
		}
		
		return newP;
	}	
}
































