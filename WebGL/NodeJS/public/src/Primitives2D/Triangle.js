
class Triangle
{
	constructor()
	{		
	}
	
	async init(p1, p2, p3)
	{
		this._vertexBufferID = 0;
		this._vertexCount = 0;
		
		this._shaderProgram = new ShaderProgram();		
		await this._shaderProgram.init("./shaders/SingleColorPrim.vs", "./shaders/SingleColorPrim.fs");
		
		this.generateBufferID(p1, p2, p3);
		
		this._oriMat = new GLMat();
		this.setColor4ub(255, 255, 255, 255);
	}	
	
	generateBufferID(p1, p2, p3)
	{
		var arr = [
			p1.x, p1.y, p1.z,
			p2.x, p2.y, p2.z,
			p3.x, p3.y, p3.z
		];
		
		this._vertexBufferID = gl.createBuffer();
		gl.bindBuffer(gl.ARRAY_BUFFER, this._vertexBufferID);
		gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(arr), gl.STATIC_DRAW);
				
		this._vertexCount = 3;
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
}































