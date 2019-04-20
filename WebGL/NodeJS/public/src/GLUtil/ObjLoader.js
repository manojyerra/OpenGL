
class Point
{
	constructor(x, y, z)
	{
		this.x = x;
		this.y = y;
		this.z = z;
	}
}

class ObjLoader
{
	constructor()
	{
	}
	
	async init(folderPath)
	{
		await this.readObjFile(folderPath+"/objFile.obj");
		await this.readTexture(folderPath+"/texture.png");
	}
	
	async readObjFile(filePath)
	{
		var fileData = await loadTextFile(filePath);
		
		var lines = fileData.split("\n");
		
		var vertexVec = Array(0);
		var uvVec = Array(0);
		var normalVec = Array(0);
		
		for(var i=0; i<lines.length; i++)
		{
			var line = lines[i];
			
			if(line.startsWith("v "))
			{
				vertexVec.push( this.getVertex(line) );
			}
			else if(line.startsWith("vt "))
			{
				uvVec.push( this.getUV(line) );
			}
			else if(line.startsWith("vn "))
			{
				normalVec.push( this.getNormal(line) );
			}		
		}
		
		this.v = Array(3).fill(0);
		this.t = Array(3).fill(0);
		this.n = Array(3).fill(0);
		
		var uvsExist = uvVec.length > 0;
		var normalsExist = normalVec.length > 0;
		
		console.log('uvsExist : ', uvsExist);
		console.log('normalsExist : ', normalsExist);
		console.log('vertexVec size : ', vertexVec.length);
		console.log('normalVec size : ', normalVec.length);
		
			
		var vertexArr = Array(0).fill(0);
		var uvArr = Array(0).fill(0);
		var normalArr = Array(0).fill(0);
		
		for(var num=0; num<lines.length; num++)
		{
			var line = lines[num];
			
			if(line.startsWith("f "))
			{
				if(uvsExist && normalsExist)
				{
					this.readFace(line, true, true);
				}
				else if(!uvsExist && normalsExist)
				{
					this.readFace(line, false, true);
				}
				else if(uvsExist && !normalsExist)
				{
					this.readFace(line, true, false);
				}
				else if(!uvsExist && !normalsExist)
				{
					this.readFace(line, false, false);
				}

				for(var i=0; i<3; i++)
				{
					this.v[i]--;
					this.t[i]--;
					this.n[i]--;
				}

				for(var i=0; i<3; i++)
				{
					vertexArr.push(vertexVec[this.v[i]].x);
					vertexArr.push(vertexVec[this.v[i]].y);
					vertexArr.push(vertexVec[this.v[i]].z);

					if(uvsExist)
					{
						uvArr.push(uvVec[this.t[i]].x);
						uvArr.push(uvVec[this.t[i]].y);
					}

					if(normalsExist)
					{
						normalArr.push(normalVec[this.n[i]].x);
						normalArr.push(normalVec[this.n[i]].y);
						normalArr.push(normalVec[this.n[i]].z);	
					}
				}
			}
		}
		
		this._vertexCount = vertexArr.length / 3;		
		
		this._vertexBufferID = gl.createBuffer();
		gl.bindBuffer(gl.ARRAY_BUFFER, this._vertexBufferID);
		gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(vertexArr), gl.STATIC_DRAW);

		if(uvsExist)
		{
			this._uvBufferID = gl.createBuffer();
			gl.bindBuffer(gl.ARRAY_BUFFER, this._uvBufferID);
			gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(uvArr), gl.STATIC_DRAW);
		}

		if(normalsExist)
		{
			this._normalBufferID = gl.createBuffer();
			gl.bindBuffer(gl.ARRAY_BUFFER, this._normalBufferID);
			gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(normalArr), gl.STATIC_DRAW);
		}
		
		this._shaderProgram = new ShaderProgram();
		await this._shaderProgram.init("./shaders/Model/Model.vs", "./shaders/Model/Model.fs");
	}

	async readTexture(texturePath)
	{
		var img = await loadTexture(texturePath);
		this._textureID = generateGLTexureID(img);
	}
	
	readFace(line, readUV, readNormal)
	{
		var tokens = line.split(" ");
		
		var subTokens1 = tokens[1].split("/");
		var subTokens2 = tokens[2].split("/");
		var subTokens3 = tokens[3].split("/");
		
		this.v[0] = parseInt(subTokens1[0]);
		this.v[1] = parseInt(subTokens2[0]);
		this.v[2] = parseInt(subTokens3[0]);
			
		if(readUV)
		{
			this.t[0] = parseInt(subTokens1[1]);
			this.t[1] = parseInt(subTokens2[1]);
			this.t[2] = parseInt(subTokens3[1]);
		}

		if(readNormal)
		{
			this.n[0] = parseInt(subTokens1[2]);
			this.n[1] = parseInt(subTokens2[2]);
			this.n[2] = parseInt(subTokens3[2]);
		}
	}	
	
	getVertex(line)
	{
		var tokens = line.split(" ");
		return new Point(parseFloat(tokens[1]), parseFloat(tokens[2]), parseFloat(tokens[3]));
	}
	
	getUV(line)
	{
		var tokens = line.split(" ");
		return new Point(parseFloat(tokens[1]), parseFloat(tokens[2]), 0.0);
	}
	
	getNormal(line)
	{
		var tokens = line.split(" ");
		return new Point(parseFloat(tokens[1]), parseFloat(tokens[2]), parseFloat(tokens[3]));
	}
	
	draw()
	{
		//gl.enable( gl.TEXTURE_2D );
		gl.bindTexture(gl.TEXTURE_2D, this._textureID);
		
		this._shaderProgram.begin();

		var projMatLoc = gl.getUniformLocation(this._shaderProgram.programID, "projMat");
		var modelMatLoc = gl.getUniformLocation(this._shaderProgram.programID, "modelMat");
		var normalMatLoc = gl.getUniformLocation(this._shaderProgram.programID, "normalMat");
		
		gl.uniformMatrix4fv(projMatLoc, false, cam.projMat.m);
		gl.uniformMatrix4fv(modelMatLoc, false, cam.modelMat.m);
		gl.uniformMatrix3fv(normalMatLoc, false, cam.normalMat);
		
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
		
		var uvID = gl.getAttribLocation(this._shaderProgram.programID, "uv");
		gl.enableVertexAttribArray(uvID);
		gl.bindBuffer(gl.ARRAY_BUFFER, this._uvBufferID);
		gl.vertexAttribPointer( uvID, 2, gl.FLOAT, gl.FALSE, 0, 0);
		
		gl.drawArrays(gl.TRIANGLES, 0, this._vertexCount);
		
		this._shaderProgram.end();
		
		gl.bindTexture(gl.TEXTURE_2D, null);		
		//gl.disable( gl.TEXTURE_2D );
	}
}





















