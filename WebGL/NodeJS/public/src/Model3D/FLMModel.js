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
		this._indicesType = 0;
		this._numIndices = 0;
		this._numVertex = 0;

		this._vertexBufferID = 0;		
		this._indicesBufferID = 0;
		
		this._baseTexID = null;
		this._normalMapTexID = null;
		
		this._oriMat = new GLMat();


		await this.readFLMFiles(folderPath);
		await this.readTextures(folderPath);

		this._shaderProgram = await shadersManager.createShaderProgram("./shaders/Model/normalMap/Model.vs", "./shaders/Model/normalMap/Model.fs");
	}

	draw()
	{
		this._shaderProgram.begin();
		
		gl.activeTexture(gl.TEXTURE0);
		gl.bindTexture(gl.TEXTURE_2D, this._baseTexID);
		gl.uniform1i(gl.getUniformLocation(this._shaderProgram.programID, "baseTexture"), 0);
		
		gl.activeTexture(gl.TEXTURE0+1);
		gl.bindTexture(gl.TEXTURE_2D, this._normalMapTexID);
		gl.uniform1i(gl.getUniformLocation(this._shaderProgram.programID, "normalMap"), 1);		

		gl.activeTexture(gl.TEXTURE0+2);
		gl.bindTexture(gl.TEXTURE_2D, this._metallicTexID);
		gl.uniform1i(gl.getUniformLocation(this._shaderProgram.programID, "metallic"), 2);		

		// gl.activeTexture(gl.TEXTURE0+3);
		// gl.bindTexture(gl.TEXTURE_2D, this._unityGlossTexID);
		// gl.uniform1i(gl.getUniformLocation(this._shaderProgram.programID, "unityGloss"), 3);		

		var projMatLoc = gl.getUniformLocation(this._shaderProgram.programID, "projMat");
		var modelMatLoc = gl.getUniformLocation(this._shaderProgram.programID, "modelMat");
		var normalMatLoc = gl.getUniformLocation(this._shaderProgram.programID, "normalMat");
		
		gl.uniformMatrix4fv(projMatLoc, false, cam3D.projMat.m);
		gl.uniformMatrix4fv(modelMatLoc, false, cam3D.modelMat.m);
		gl.uniformMatrix3fv(normalMatLoc, false, cam3D.normalMat);

// Ns 96.078431
// Ka 1.000000 1.000000 1.000000
// Kd 0.175206 0.175206 0.175206
// Ks 0.743243 0.743243 0.743243
// Ke 0.000000 0.000000 0.000000

// Ns 86.274510
// Ka 1.000000 1.000000 1.000000
// Kd 0.308108 0.308108 0.308108
// Ks 0.290541 0.290541 0.290541

// sliver meterial
// ambient 0.000000, 0.000000, 0.000000, 1.0
// diffuse 0.800000, 0.800000, 0.800000, 1.0
// specular 0.200000, 0.200000, 0.200000, 1.0
// shininess 0.3



		gl.uniform3f(gl.getUniformLocation(this._shaderProgram.programID, "lightPos"), 0.0, 0.0, 0.0);
		gl.uniform4f(gl.getUniformLocation(this._shaderProgram.programID, "ambient"), 0.000000, 0.000000, 0.000000, 1.0);
		gl.uniform4f(gl.getUniformLocation(this._shaderProgram.programID, "diffuse"), 0.800000, 0.800000, 0.800000, 1.0);
		gl.uniform4f(gl.getUniformLocation(this._shaderProgram.programID, "specular"), 0.200000, 0.200000, 0.200000, 1.0);
		gl.uniform1f(gl.getUniformLocation(this._shaderProgram.programID, "shininess"), 0.3);

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
		
		var tangentID = gl.getAttribLocation(this._shaderProgram.programID, "tangent");
		gl.enableVertexAttribArray(tangentID);
		gl.bindBuffer(gl.ARRAY_BUFFER, this._tangentBufferID);
		gl.vertexAttribPointer( tangentID, 3, gl.FLOAT, gl.FALSE, 0, 0);
		
		// var biTangentID = gl.getAttribLocation(this._shaderProgram.programID, "biTangent");
		// gl.enableVertexAttribArray(biTangentID);
		// gl.bindBuffer(gl.ARRAY_BUFFER, this._biTangentBufferID);
		// gl.vertexAttribPointer( biTangentID, 3, gl.FLOAT, gl.FALSE, 0, 0);
		
		gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, this._indicesBufferID);
		gl.drawElements(gl.TRIANGLES, this._numIndices, gl.UNSIGNED_SHORT, 0);
		
		this._shaderProgram.end();		
		
		gl.bindTexture(gl.TEXTURE_2D, null);		
	}
	
	//private methods...
	
	async readFLMFiles(folderPath)
	{
		this._verticesPointer = new Float32Array( await loadArrayBuffer(folderPath+"/vertex.buf") );
		this._uvsPointer = new Float32Array( await loadArrayBuffer(folderPath+"/uv.buf") );
		this._normalsPointer = new Float32Array( await loadArrayBuffer(folderPath+"/normal.buf") );
		this._numVertex = this._verticesPointer.length / 3;

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
		
		//creating tangent and bitangent buffers.
		
		this._tangentPointer = new Float32Array(this._verticesPointer.length);
		this._biTangentPointer = new Float32Array(this._verticesPointer.length);
		
		var v = this._verticesPointer;
		var t = this._uvsPointer;
		var tp = this._tangentPointer;
		var btp = this._biTangentPointer;
		
		for (var i = 0 ; i < this._numIndices ; i += 3)
		{
			var index0 = this._indicesPointer[i+0];
			var index1 = this._indicesPointer[i+1];
			var index2 = this._indicesPointer[i+2];
			
			var v0 = new CVector3( v[index0*3+0], v[index0*3+1], v[index0*3+2]);
			var v1 = new CVector3( v[index1*3+0], v[index1*3+1], v[index1*3+2]);
			var v2 = new CVector3( v[index2*3+0], v[index2*3+1], v[index2*3+2]);
			
			var t0 = new CVector3( t[index0*2+0], v[index0*2+1], 0.0);
			var t1 = new CVector3( t[index1*2+0], v[index1*2+1], 0.0);
			var t2 = new CVector3( t[index2*2+0], v[index2*2+1], 0.0);
			
			var Edge1 = new CVector3(v1.x - v0.x, v1.y - v0.y, v1.z - v0.z);
			var Edge2 = new CVector3(v2.x - v0.x, v2.y - v0.y, v2.z - v0.z);

			var DeltaU1 = t1.x - t0.x;
			var DeltaV1 = t1.y - t0.y;
			var DeltaU2 = t2.x - t0.x;
			var DeltaV2 = t2.y - t0.y;

			var f = 1.0 / (DeltaU1 * DeltaV2 - DeltaU2 * DeltaV1);

			var Tangent = new CVector3(0,0,0);
			var Bitangent = new CVector3(0,0,0);

			Tangent.x = f * (DeltaV2 * Edge1.x - DeltaV1 * Edge2.x);
			Tangent.y = f * (DeltaV2 * Edge1.y - DeltaV1 * Edge2.y);
			Tangent.z = f * (DeltaV2 * Edge1.z - DeltaV1 * Edge2.z);

			Bitangent.x = f * (-DeltaU2 * Edge1.x - DeltaU1 * Edge2.x);
			Bitangent.y = f * (-DeltaU2 * Edge1.y - DeltaU1 * Edge2.y);
			Bitangent.z = f * (-DeltaU2 * Edge1.z - DeltaU1 * Edge2.z);

			tp[index0*3+0] += Tangent.x;
			tp[index0*3+1] += Tangent.y;
			tp[index0*3+2] += Tangent.z;

			tp[index1*3+0] += Tangent.x;
			tp[index1*3+1] += Tangent.y;
			tp[index1*3+2] += Tangent.z;

			tp[index2*3+0] += Tangent.x;
			tp[index2*3+1] += Tangent.y;
			tp[index2*3+2] += Tangent.z;


			btp[index0*3+0] += Bitangent.x;
			btp[index0*3+1] += Bitangent.y;
			btp[index0*3+2] += Bitangent.z;

			btp[index1*3+0] += Bitangent.x;
			btp[index1*3+1] += Bitangent.y;
			btp[index1*3+2] += Bitangent.z;

			btp[index2*3+0] += Bitangent.x;
			btp[index2*3+1] += Bitangent.y;
			btp[index2*3+2] += Bitangent.z;
		}

		for (var i = 0 ; i < tp.length ; i += 3)
		{
			var tan = new CVector3(tp[i+0], tp[i+1], tp[i+2]);
			tan.normalize();
			tp[0] = tan.x;
			tp[1] = tan.y;
			tp[2] = tan.z;
			
			var biTan = new CVector3(btp[i+0], btp[i+1], btp[i+2]);
			biTan.normalize();
			btp[0] = biTan.x;
			btp[1] = biTan.y;
			btp[2] = biTan.z;
		}
		
		this.generateBufferID();
	}
	
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

		this._tangentBufferID = gl.createBuffer();
		gl.bindBuffer(gl.ARRAY_BUFFER, this._tangentBufferID);
		gl.bufferData(gl.ARRAY_BUFFER, this._tangentPointer, gl.STATIC_DRAW);
		
		this._biTangentBufferID = gl.createBuffer();
		gl.bindBuffer(gl.ARRAY_BUFFER, this._biTangentBufferID);
		gl.bufferData(gl.ARRAY_BUFFER, this._biTangentPointer, gl.STATIC_DRAW);
	}
	
	async readTextures(folderPath)
	{
		var img = await loadTexture(folderPath+"/baseTexture.jpg");
		this._baseTexID = GLUtils.generateGLTexureID(img);
		
		img = await loadTexture(folderPath+"/normalMap.jpg");
		this._normalMapTexID = GLUtils.generateGLTexureID(img);
		
		img = await loadTexture(folderPath+"/metallic.jpg");
		this._metallicTexID = GLUtils.generateGLTexureID(img);

		//img = await loadTexture(folderPath+"/unityGloss.jpg");
		//this._unityGlossTexID = GLUtils.generateGLTexureID(img);
	}
}



























