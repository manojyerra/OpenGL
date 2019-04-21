
class Box extends Shape
{
	constructor()
	{
		super(Shape.BOX);
		
		this._w = 0;
		this._h = 0;
		this._d = 0;
	}
	
	async initWithPosAndSize(x, y, z, w, h, d)
	{
		this.m[12] = x;
		this.m[13] = y;
		this.m[14] = z;

		this._w = w;
		this._h = h;
		this._d = d;

		this.initCommon();
	}

	async initWithMatAndSize(mat, size)
	{
		for(var i=0; i<16; i++)
		{
			this.m[i] = mat[i];
		}
		
		this._w = size.x;
		this._h = size.y;
		this._d = size.z;

		this.initCommon();
	}

	async initWithBox(box)
	{
		console.log('called init(box) ');
		
		for(var i=0; i<16; i++)
		{
			this.m[i] = box.m[i];
		}		

		this._w = box.getSize().x;
		this._h = box.getSize().y;
		this._d = box.getSize().z;

		this._id = box.getID();

		this.initCommon();
	}

	async initCommon()
	{
		this._vertexBufferID = 0;
		this._colorBufferID = 0;
		this._vertexCount = 0;

		this._shaderProgram = new ShaderProgram();
		await this._shaderProgram.init("shaders/ColorArray/ColorArray.vs", "shaders/ColorArray/ColorArray.fs");

		this._randomColor = new RandomColor();

		this.generateBufferID();		
	}

	generateBufferID()
	{
		var buffer = new GLBuffer(true, false, false);

		buffer.glBegin(gl.TRIANGLES);

		//if(this._useRandomColors)
		//	_randomColor.Reset();

		var w = 1.0/2.0;
		var h = 1.0/2.0;
		var d = 1.0/2.0;
		
		//Front face
		if(this._useRandomColors) buffer.glColoruia(this._randomColor.nextColor(), this._randomColorAlpha);
		buffer.glVertex3f(+w, -h, +d);
		if(this._useRandomColors) buffer.glColoruia(this._randomColor.nextColor(), this._randomColorAlpha);
		buffer.glVertex3f(+w, +h, +d);
		buffer.glVertex3f(-w, -h, +d);
		buffer.glVertex3f(-w, -h, +d);
		buffer.glVertex3f(+w, +h, +d);
		if(this._useRandomColors) buffer.glColoruia(this._randomColor.nextColor(), this._randomColorAlpha);
		buffer.glVertex3f(-w, +h, +d);

		//Back face
		if(this._useRandomColors) buffer.glColoruia(this._randomColor.nextColor(), this._randomColorAlpha);
		buffer.glVertex3f(+w, -h, -d);
		if(this._useRandomColors) buffer.glColoruia(this._randomColor.nextColor(), this._randomColorAlpha);
		buffer.glVertex3f(-w, -h, -d);
		buffer.glVertex3f(+w, +h, -d);
		buffer.glVertex3f(+w, +h, -d);
		buffer.glVertex3f(-w, -h, -d);
		if(this._useRandomColors) buffer.glColoruia(this._randomColor.nextColor(), this._randomColorAlpha);
		buffer.glVertex3f(-w, +h, -d);

		//Top face
		if(this._useRandomColors) buffer.glColoruia(this._randomColor.nextColor(), this._randomColorAlpha);
		buffer.glVertex3f(+w, +h, -d);
		if(this._useRandomColors) buffer.glColoruia(this._randomColor.nextColor(), this._randomColorAlpha);
		buffer.glVertex3f(-w, +h, -d);
		buffer.glVertex3f(+w, +h, +d);
		buffer.glVertex3f(+w, +h, +d);
		buffer.glVertex3f(-w, +h, -d);
		if(this._useRandomColors) buffer.glColoruia(this._randomColor.nextColor(), this._randomColorAlpha);
		buffer.glVertex3f(-w, +h, +d);

		//Bottom face
		if(this._useRandomColors) buffer.glColoruia(this._randomColor.nextColor(), this._randomColorAlpha);
		buffer.glVertex3f(+w, -h, -d);
		if(this._useRandomColors) buffer.glColoruia(this._randomColor.nextColor(), this._randomColorAlpha);
		buffer.glVertex3f(+w, -h, +d);
		buffer.glVertex3f(-w, -h, -d);
		buffer.glVertex3f(-w, -h, -d);
		buffer.glVertex3f(+w, -h, +d);
		if(this._useRandomColors) buffer.glColoruia(this._randomColor.nextColor(), this._randomColorAlpha);
		buffer.glVertex3f(-w, -h, +d);

		//Right face
		if(this._useRandomColors) buffer.glColoruia(this._randomColor.nextColor(), this._randomColorAlpha);
		buffer.glVertex3f(+w, +h, -d);
		if(this._useRandomColors) buffer.glColoruia(this._randomColor.nextColor(), this._randomColorAlpha);
		buffer.glVertex3f(+w, +h, +d);
		buffer.glVertex3f(+w, -h, -d);
		buffer.glVertex3f(+w, -h, -d);
		buffer.glVertex3f(+w, +h, +d);
		if(this._useRandomColors) buffer.glColoruia(this._randomColor.nextColor(), this._randomColorAlpha);
		buffer.glVertex3f(+w, -h, +d);

		//Left face
		if(this._useRandomColors) buffer.glColoruia(this._randomColor.nextColor(), this._randomColorAlpha);
		buffer.glVertex3f(-w, +h, -d);
		if(this._useRandomColors) buffer.glColoruia(this._randomColor.nextColor(), this._randomColorAlpha);
		buffer.glVertex3f(-w, -h, -d);
		buffer.glVertex3f(-w, +h, +d);
		buffer.glVertex3f(-w, +h, +d);
		buffer.glVertex3f(-w, -h, -d);
		if(this._useRandomColors) buffer.glColoruia(this._randomColor.nextColor(), this._randomColorAlpha);
		buffer.glVertex3f(-w, -h, +d);

		buffer.glEnd();

		this._vertexBufferID = buffer.getVertexBufferID();
		this._colorBufferID = buffer.getColorBufferID();
		this._vertexCount = buffer.getVertexCount();
	}

	draw()
	{
		if(!this._visible)
			return;

		//GLboolean glLighting = GLUtil::GLEnable(GL_LIGHTING, false);
		//GLboolean blend = GLUtil::GLEnable(GL_BLEND, true);
		//GLboolean depthTest = GLUtil::GLEnable(GL_DEPTH_TEST, true);

		//glPushMatrix();
		//glMultMatrixf(m);

		//glScalef(_w, _h, _d);

		this._shaderProgram.begin();

		var projMatLoc = gl.getUniformLocation(this._shaderProgram.programID, "projMat");
		var modelMatLoc = gl.getUniformLocation(this._shaderProgram.programID, "modelMat");
		gl.uniformMatrix4fv(projMatLoc, false, cam.projMat.m);
		gl.uniformMatrix4fv(modelMatLoc, false, cam.modelMat.m);

		var colorID = gl.getAttribLocation(this._shaderProgram.programID, "color");
		gl.enableVertexAttribArray(colorID);
		gl.bindBuffer(gl.ARRAY_BUFFER, this._colorBufferID);
		gl.vertexAttribPointer( colorID, 4, gl.UNSIGNED_BYTE, gl.FALSE, 0, 0);

		var vertexID = gl.getAttribLocation(this._shaderProgram.programID, "vertex");
		gl.enableVertexAttribArray(vertexID);
		gl.bindBuffer(gl.ARRAY_BUFFER, this._vertexBufferID);
		gl.vertexAttribPointer(vertexID, 3, gl.FLOAT, gl.FALSE, 0, 0);

		gl.drawArrays(gl.TRIANGLES, 0, this._vertexCount);
		
		//gl.bindBuffer(gl.ARRAY_BUFFER, null);
		//gl.disableVertexAttribArray(vertexID);
		//gl.disableVertexAttribArray(colorID);

		this._shaderProgram.end();

		//glPopMatrix();

		//GLUtil::GLEnable(GL_LIGHTING, glLighting);
		//GLUtil::GLEnable(GL_BLEND, blend);
		//GLUtil::GLEnable(GL_DEPTH_TEST, depthTest);
	}
	
}