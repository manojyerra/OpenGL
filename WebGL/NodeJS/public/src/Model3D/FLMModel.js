class FLMModel
{
	constructor()
	{
	}
	
	async init(folderPath)
	{
		this._verticesPointer = null;
		this._uvsPointer = null;
		this._normalsPointer = null;
		this._indicesPointer = null;
		this._textureID = 0;
		this._indicesType = 0;
		this._numIndices = 0;
		this._numVertex = 0;

		this._vertexBufferID = 0;		
		this._indicesBufferID = 0;
		
		this._oriMat = new GLMat();

		this._verticesPointer = new Float32Array( await loadArrayBuffer(folderPath+"/vertex.buf") );
		this._uvsPointer = new Float32Array( await loadArrayBuffer(folderPath+"/uv.buf") );
		this._normalsPointer = new Float32Array( await loadArrayBuffer(folderPath+"/normal.buf") );
		this._numVertex = this._verticesPointer.length / 3;

		//console.log("this._normalsPointer : ", this._normalsPointer);
		//console.log("this._verticesPointer : ", this._verticesPointer);
		//console.log("this.normalArrBuffer : ", this._normalsPointer);
				
		var indexArrBuffer = await loadArrayBuffer(folderPath+"/index.buf");
		var numBytesPerIndex = (new Uint32Array(indexArrBuffer, indexArrBuffer.byteLength-4, 1))[0];
		this._numIndices = (indexArrBuffer.byteLength-4) / numBytesPerIndex;
		
		if(numBytesPerIndex == 1) {
			this._indicesPointer = new Uint8Array(indexArrBuffer, 0, this._numIndices);
		}
		else if(numBytesPerIndex == 2) {
			this._indicesPointer = new Uint16Array(indexArrBuffer, 0, this._numIndices);
		}
		else if(numBytesPerIndex == 4) {
			this._indicesPointer = new Uint32Array(indexArrBuffer, 0, this._numIndices);
		}

		await this.readTexture(folderPath+"/texture.png");
				
		this._shaderProgram = new ShaderProgram();		
		await this._shaderProgram.init("./shaders/Model/Model.vs", "./shaders/Model/Model.fs");
		this.generateBufferID();
	}

	draw()
	{
		gl.bindTexture(gl.TEXTURE_2D, this._textureID);
		
		this._shaderProgram.begin();

		var projMatLoc = gl.getUniformLocation(this._shaderProgram.programID, "projMat");
		var modelMatLoc = gl.getUniformLocation(this._shaderProgram.programID, "modelMat");
		var normalMatLoc = gl.getUniformLocation(this._shaderProgram.programID, "normalMat");
		
		gl.uniformMatrix4fv(projMatLoc, false, cam3D.projMat.m);
		gl.uniformMatrix4fv(modelMatLoc, false, cam3D.modelMat.m);
		gl.uniformMatrix3fv(normalMatLoc, false, cam3D.normalMat);
		
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
		gl.bindBuffer(gl.ARRAY_BUFFER, this._normalsBufferID);
		gl.vertexAttribPointer( normalID, 3, gl.FLOAT, gl.TRUE, 0, 0);
		
		var uvID = gl.getAttribLocation(this._shaderProgram.programID, "uv");
		gl.enableVertexAttribArray(uvID);
		gl.bindBuffer(gl.ARRAY_BUFFER, this._uvBufferID);
		gl.vertexAttribPointer( uvID, 2, gl.FLOAT, gl.FALSE, 0, 0);
		
		gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, this._indicesBufferID);
		gl.drawElements(gl.TRIANGLES, this._numIndices, gl.UNSIGNED_SHORT, 0);
		
		this._shaderProgram.end();		
		
		gl.bindTexture(gl.TEXTURE_2D, null);		
	}
	
	//private methods...
	
	generateBufferID()
	{
		this._vertexBufferID = gl.createBuffer();
		gl.bindBuffer(gl.ARRAY_BUFFER, this._vertexBufferID);
		gl.bufferData(gl.ARRAY_BUFFER, this._verticesPointer, gl.STATIC_DRAW);
		
		this._uvBufferID = gl.createBuffer();
		gl.bindBuffer(gl.ARRAY_BUFFER, this._uvBufferID);
		gl.bufferData(gl.ARRAY_BUFFER, this._uvsPointer, gl.STATIC_DRAW);
		
		this._normalsBufferID = gl.createBuffer();
		gl.bindBuffer(gl.ARRAY_BUFFER, this._normalsBufferID);
		gl.bufferData(gl.ARRAY_BUFFER, this._normalsPointer, gl.STATIC_DRAW);
		
		this._indicesBufferID = gl.createBuffer();
		gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, this._indicesBufferID);
		gl.bufferData(gl.ELEMENT_ARRAY_BUFFER, this._indicesPointer, gl.STATIC_DRAW);
	}
	
	async readTexture(texturePath)
	{
		var img = await loadTexture(texturePath);
		this._textureID = GLUtils.generateGLTexureID(img);
	}	
}



























