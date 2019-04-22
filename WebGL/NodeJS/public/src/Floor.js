
class Floor
{
	constructor()
	{
	}
	
	async init()
	{
		this._visible = true;
		this._axisVisible = true;
		this._gridVisible = false;
		this._gridLinesVisible = true;

		this._shaderProgram = new ShaderProgram();
		await this._shaderProgram.init("./shaders/ColorArray/ColorArray.vs", "./shaders/ColorArray/ColorArray.fs");

		var start = -16;
		var end = 16;
		var gap = 1;
		
		this._oriMat = new GLMat();

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
		

	isVisible()
	{
		return this._visible;
	}

	isAxisVisible()
	{
		return this._axisVisible;
	}

	isGridVisible()
	{
		return this._gridVisible;
	}

	isGridLinesVisible()
	{
		return this._gridLinesVisible;
	}

	setVisible(visible)
	{
		this._visible = visible;
	}

	setAxisVisible(axisVisible)
	{
		this._axisVisible = axisVisible;
	}

	setGridVisible(gridVisible)
	{
		this._gridVisible = gridVisible;
	}

	setGridLinesVisible(gridLinesVisible)
	{
		this._gridLinesVisible = gridLinesVisible;
	}

	draw()
	{
		if(this._visible == false)
			return;

		this._shaderProgram.begin();

		var projMatLoc = gl.getUniformLocation(this._shaderProgram.programID, "projMat");
		var modelMatLoc = gl.getUniformLocation(this._shaderProgram.programID, "modelMat");
		var oriMatLoc = gl.getUniformLocation(this._shaderProgram.programID, "oriMat");
		
		gl.uniformMatrix4fv(projMatLoc, false, cam3D.projMat.m);
		gl.uniformMatrix4fv(modelMatLoc, false, cam3D.modelMat.m);
		gl.uniformMatrix4fv(oriMatLoc, false, this._oriMat.m);

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

































