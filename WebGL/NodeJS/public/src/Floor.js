
class Floor
{
	constructor()
	{
	}
	
	async init(gl)
	{
		this._visible = true;
		this._axisVisible = true;
		this._gridVisible = false;
		this._gridLinesVisible = true;

		this._shaderProgram = new ShaderProgram();
		await this._shaderProgram.init(gl, "./shaders/ColorArray/ColorArray.vs", "./shaders/ColorArray/ColorArray.fs");

		var start = -16;
		var end = 16;
		var gap = 1;

		this._axisBuffer = new GLBuffer(true, false, false);
		this._gridBuffer = new GLBuffer(true, false, false);;
		this._gridLinesBuffer = new GLBuffer(true, false, false);;

		this._axisBuffer.glBegin(gl.LINES);

			this._axisBuffer.glColor4ub(255,0,0,255);
			this._axisBuffer.glVertex3f(start,	0.0,	0.0);
			this._axisBuffer.glVertex3f(end,	0.0,	0.0);

			this._axisBuffer.glColor4ub(0,0,255,255);
			this._axisBuffer.glVertex3f(0.0,	0.0,	start);
			this._axisBuffer.glVertex3f(0.0,	0.0,	end);

			this._axisBuffer.glColor4ub(0,255,0,255);
			this._axisBuffer.glVertex3f(0.0,	0.0,	(start+end)/2.0);
			this._axisBuffer.glVertex3f(0.0,	end,	0);

		this._axisBuffer.glEnd();
		

		this._gridLinesBuffer.glBegin(gl.LINES);
		this._gridLinesBuffer.glColor4ub(128,128,128,128);

		for(var i=start; i<=end; i+=gap)
		{
			if(i == 0)
				continue;

			this._gridLinesBuffer.glVertex3f(start,	0.01,	i);
			this._gridLinesBuffer.glVertex3f(end,	0.01,	i);
		}

		for(var i=start; i<=end; i+=gap)
		{
			if(i == 0)
				continue;

			this._gridLinesBuffer.glVertex3f(i,	0.01,	start);
			this._gridLinesBuffer.glVertex3f(i,	0.01,	end);
		}
		this._gridLinesBuffer.glEnd();


		this._gridBuffer.glBegin(gl.TRIANGLES);

		var c1 = 255;
		var c2 = 158;
		var gridCount = 0;
		
		for(var outer = start; outer < end; outer++)
		{
			var temp = c1;
			c1 = c2;
			c2 = temp;
			
			for(var i=start; i<end; i+=gap)
			{
				gridCount++;
				
				if(gridCount%2 == 0)
					this._gridBuffer.glColor4ub(255, 0, 0, 64);
				else
					this._gridBuffer.glColor4ub(0, 255, 0, 64);

				this._gridBuffer.glVertex3f(i,		0, outer*gap); //1
				this._gridBuffer.glVertex3f(i+gap,	0, outer*gap); //2
				this._gridBuffer.glVertex3f(i,		0, (outer+1)*gap); //4

				this._gridBuffer.glVertex3f(i+gap,	0, outer*gap); //2
				this._gridBuffer.glVertex3f(i,		0, (outer+1)*gap); //4
				this._gridBuffer.glVertex3f(i+gap,	0, (outer+1)*gap); //3
			}
		}
		
		this._gridBuffer.glEnd();
	}
		

	IsVisible()
	{
		return this._visible;
	}

	IsAxisVisible()
	{
		return this._axisVisible;
	}

	IsGridVisible()
	{
		return this._gridVisible;
	}

	IsGridLinesVisible()
	{
		return this._gridLinesVisible;
	}

	SetVisible(visible)
	{
		this._visible = visible;
	}

	SetAxisVisible(axisVisible)
	{
		this._axisVisible = axisVisible;
	}

	SetGridVisible(gridVisible)
	{
		this._gridVisible = gridVisible;
	}

	SetGridLinesVisible(gridLinesVisible)
	{
		this._gridLinesVisible = gridLinesVisible;
	}

	Draw(gl)
	{
		if(this._visible == false)
			return;
		
		gl.enable(gl.BLEND);
		gl.blendFunc(gl.SRC_ALPHA, gl.ONE_MINUS_SRC_ALPHA);
		gl.enable(gl.DEPTH_TEST);

		this._shaderProgram.begin();

		var projMatLoc = gl.getUniformLocation(this._shaderProgram.programID, "projMat");
		var modelMatLoc = gl.getUniformLocation(this._shaderProgram.programID, "modelMat");
		gl.uniformMatrix4fv(projMatLoc, false, cam.projMat.m);
		gl.uniformMatrix4fv(modelMatLoc, false, cam.modelMat.m);

		if(this._axisVisible)
		{
			gl.lineWidth(2.0);
				
			var colorID = gl.getAttribLocation(this._shaderProgram.programID, "color");
			gl.enableVertexAttribArray(colorID);
			gl.bindBuffer(gl.ARRAY_BUFFER, this._axisBuffer.getColorBufferID());
			gl.vertexAttribPointer(colorID, 4, gl.UNSIGNED_BYTE, gl.FALSE, 0, 0);

			var vertexID = gl.getAttribLocation(this._shaderProgram.programID, "vertex");
			gl.enableVertexAttribArray(vertexID);
			gl.bindBuffer(gl.ARRAY_BUFFER, this._axisBuffer.getVertexBufferID());
			gl.vertexAttribPointer( vertexID, 3, gl.FLOAT, gl.FALSE, 0, 0);

			gl.drawArrays(gl.LINES, 0, this._axisBuffer.getVertexCount());
			
			//disable vertexattibarray here...
		}

		if(this._gridLinesVisible)
		{
			gl.lineWidth(1.0);

			var colorID = gl.getAttribLocation(this._shaderProgram.programID, "color");
			gl.enableVertexAttribArray(colorID);
			gl.bindBuffer(gl.ARRAY_BUFFER, this._gridLinesBuffer.getColorBufferID());
			gl.vertexAttribPointer(colorID, 4, gl.UNSIGNED_BYTE, gl.FALSE, 0, 0);

			var vertexID = gl.getAttribLocation(this._shaderProgram.programID, "vertex");
			gl.enableVertexAttribArray(vertexID);
			gl.bindBuffer(gl.ARRAY_BUFFER, this._gridLinesBuffer.getVertexBufferID());
			gl.vertexAttribPointer( vertexID, 3, gl.FLOAT, gl.FALSE, 0, 0);

			gl.drawArrays(gl.LINES, 0, this._gridLinesBuffer.getVertexCount());
			//disable vertexattibarray here...
		}

		if(this._gridVisible)
		{
			var colorID = gl.getAttribLocation(this._shaderProgram.programID, "color");
			gl.enableVertexAttribArray(colorID);
			gl.bindBuffer(gl.ARRAY_BUFFER, this._gridBuffer.getColorBufferID());
			gl.vertexAttribPointer(colorID, 4, gl.UNSIGNED_BYTE, gl.FALSE, 0, 0);

			var vertexID = gl.getAttribLocation(this._shaderProgram.programID, "vertex");
			gl.enableVertexAttribArray(vertexID);
			gl.bindBuffer(gl.ARRAY_BUFFER, this._gridBuffer.getVertexBufferID());
			gl.vertexAttribPointer( vertexID, 3, gl.FLOAT, gl.FALSE, 0, 0);

			gl.drawArrays(gl.TRIANGLES, 0, this._gridBuffer.getVertexCount());
			//disable vertexattibarray here...
		}

		this._shaderProgram.end();

		// GLUtil::GLLineWidth(lineWidth_bk);
		// GLUtil::GLEnable(GL_LIGHTING, glLighting);
		// GLUtil::GLEnable(GL_BLEND, blend);
		// GLUtil::GLEnable(GL_DEPTH_TEST, depthTest);

		//glPopMatrix();
	}
	
}

































