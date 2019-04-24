
class Rect
{
	constructor()
	{		
	}
	
	async init(x, y, w, h)
	{
		this._vertexBufferID = 0;
		this._vertexCount = 0;
		
		this._shaderProgram = new ShaderProgram();		
		await this._shaderProgram.init("./shaders/SingleColorPrim.vs", "./shaders/SingleColorPrim.fs");
		
		this.generateBufferID();
		
		this._oriMat = new GLMat();
		this.setBounds(x, y, w, h);
		this.setColor4ub(255, 255, 255, 255);
	}	

	setBounds(x, y, w, h)
	{
		this._oriMat.m[12] = x;
		this._oriMat.m[13] = y;
		
		this._oriMat.m[0] = w;
		this._oriMat.m[5] = h;		
	}
	
	setColor4ub(r, g, b, a)
	{
		this._r = r*1.0 / 255.0;
		this._g = g*1.0 / 255.0;
		this._b = b*1.0 / 255.0;
		this._a = a*1.0 / 255.0;
	}		
		
	draw()
	{
		this._shaderProgram.begin();
		
		var projMatLoc = gl.getUniformLocation(this._shaderProgram.programID, "projMat");
		var modelMatLoc = gl.getUniformLocation(this._shaderProgram.programID, "modelMat");		
		var oriMatLoc = gl.getUniformLocation(this._shaderProgram.programID, "oriMat");
		
		gl.uniformMatrix4fv(projMatLoc, false, cam2D.projMat.m);
		gl.uniformMatrix4fv(modelMatLoc, false, cam2D.modelMat.m);
		gl.uniformMatrix4fv(oriMatLoc, false, this._oriMat.m);
		
		var colorLoc = gl.getUniformLocation(this._shaderProgram.programID, "color");
		gl.uniform4f(colorLoc, this._r, this._g, this._b, this._a);

		var vertexID = gl.getAttribLocation(this._shaderProgram.programID, "vertex");
		gl.enableVertexAttribArray(vertexID);
		gl.bindBuffer(gl.ARRAY_BUFFER, this._vertexBufferID);
		gl.vertexAttribPointer( vertexID, 3, gl.FLOAT, gl.FALSE, 0, 0);

		gl.drawArrays(gl.TRIANGLE_STRIP, 0, this._vertexCount);

		this._shaderProgram.end();
	}	
	
	//private methods...
	
	generateBufferID()
	{
		var arr = [
			0.0, 0.0, 0.0,
			1.0, 0.0, 0.0,
			0.0, 1.0, 0.0,
			1.0, 1.0, 0.0		
		];
		
		this._vertexBufferID = gl.createBuffer();
		gl.bindBuffer(gl.ARRAY_BUFFER, this._vertexBufferID);
		gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(arr), gl.STATIC_DRAW);
				
		this._vertexCount = 4;
	}	
}































