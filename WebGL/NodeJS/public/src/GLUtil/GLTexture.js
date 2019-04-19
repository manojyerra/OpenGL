
class Texture
{
	constructor()
	{
	}
	
	async init(texturePath)
	{
		this._vertexBufferID = 0;
		this._uvBufferID = 0;
		this._vertexCount = 0;

		this._shaderProgram = new ShaderProgram();		
		await this._shaderProgram.init("./shaders/UVArray/UVArray.vs", "./shaders/UVArray/UVArray.fs");
		
		this.generateBufferID();
		
		var img = await loadTexture(texturePath);
		
		this._textureID = generateGLTexureID(img);
		
		this._oriMat = new GLMat();
		this.setBounds(0,0,img.width, img.height);
	}
	
	setBounds(x, y, w, h)
	{
		this._oriMat.m[12] = x;
		this._oriMat.m[13] = y;
		
		this._oriMat.m[0] = w;
		this._oriMat.m[5] = h;		
	}
	
	// generateGLTexureID(img)
	// {
		// var textureID = gl.createTexture();
		// gl.bindTexture(gl.TEXTURE_2D, textureID);
		
		// gl.pixelStorei(gl.UNPACK_FLIP_Y_WEBGL, true);
		// gl.texImage2D(gl.TEXTURE_2D, 0, gl.RGBA, gl.RGBA, gl.UNSIGNED_BYTE, img);
		// gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MAG_FILTER, gl.LINEAR);
		// gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.LINEAR);
		
		// return textureID;
	// }
	
	generateBufferID()
	{
		var buffer = new GLBuffer(false, true, false);

		buffer.glBegin(gl.TRIANGLE_STRIP);

		var w = this._drawW;
		var h = this._drawH;

		buffer.glTexCoord2f(0, 0);
		buffer.glVertex3f(0, 0, 0);

		buffer.glTexCoord2f(1, 0);
		buffer.glVertex3f(1, 0, 0);

		buffer.glTexCoord2f(0, 1);
		buffer.glVertex3f(0, 1, 0);

		buffer.glTexCoord2f(1, 1);
		buffer.glVertex3f(1, 1, 0);

		buffer.glEnd();

		this._vertexBufferID = buffer.getVertexBufferID();
		this._uvBufferID = buffer.getUVBufferID();
		this._vertexCount = buffer.getVertexCount();

		buffer = null;	
	}
	
	draw()
	{
		this._shaderProgram.begin();
		
		gl.enable( gl.TEXTURE_2D );
		gl.bindTexture(gl.TEXTURE_2D, this._textureID);

		var projMatLoc = gl.getUniformLocation(this._shaderProgram.programID, "projMat");
		var modelMatLoc = gl.getUniformLocation(this._shaderProgram.programID, "modelMat");		
		gl.uniformMatrix4fv(projMatLoc, false, cam.projMat.m);
		gl.uniformMatrix4fv(modelMatLoc, false, cam.modelMat.m);

		var oriMatLoc = gl.getUniformLocation(this._shaderProgram.programID, "oriMat");
		gl.uniformMatrix4fv(oriMatLoc, false, this._oriMat.m);

		var uvID = gl.getAttribLocation(this._shaderProgram.programID, "uv");
		gl.enableVertexAttribArray(uvID);
		gl.bindBuffer(gl.ARRAY_BUFFER, this._uvBufferID);
		gl.vertexAttribPointer(uvID, 2, gl.FLOAT, gl.FALSE, 0, 0);

		var vertexID = gl.getAttribLocation(this._shaderProgram.programID, "vertex");
		gl.enableVertexAttribArray(vertexID);
		gl.bindBuffer(gl.ARRAY_BUFFER, this._vertexBufferID);
		gl.vertexAttribPointer( vertexID, 3, gl.FLOAT, gl.FALSE, 0, 0);

		gl.drawArrays(gl.TRIANGLE_STRIP, 0, this._vertexCount);

		gl.bindTexture(gl.TEXTURE_2D, null);		
		gl.disable( gl.TEXTURE_2D );

		this._shaderProgram.end();
		
	}

}




























