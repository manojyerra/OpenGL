
class Box extends Shape
{
	constructor()
	{
		super(Shape.BOX);
		
		this._w = 0;
		this._h = 0;
		this._d = 0;
	}
	
	initWithPosAndSize(x, y, z, w, h, d)
	{
		this.m[12] = x;
		this.m[13] = y;
		this.m[14] = z;

		this._w = w;
		this._h = h;
		this._d = d;

		this.initCommon();
	}

	initWithMatAndSize(mat, size)
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

	initWithBox(box)
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

	initCommon()
	{
		this._vertexBufferID = 0;
		this._colorBufferID = 0;
		this._vertexCount = 0;

		this._shaderProgram = new ShaderProgram("shaders/ColorArray/ColorArray.vs", "shaders/ColorArray/ColorArray.fs");

		//this.generateBufferID();
	}

	generateBufferID()
	{
		var buffer = new GLBuffer(true, false, false);

		buffer.glBegin(gl.TRIANGLES);

		if(this._useRandomColors)
			_randomColor.Reset();

		var w = 1.0/2.0;
		var h = 1.0/2.0;
		var d = 1.0/2.0;

		/*
		//Front face
		if(_useRandomColors) buffer->glColoruia(_randomColor.NextColor(), _randomColorAlpha);
		buffer->glVertex3f(+w, -h, +d);
		if(_useRandomColors) buffer->glColoruia(_randomColor.NextColor(), _randomColorAlpha);
		buffer->glVertex3f(+w, +h, +d);
		buffer->glVertex3f(-w, -h, +d);
		buffer->glVertex3f(-w, -h, +d);
		buffer->glVertex3f(+w, +h, +d);
		if(_useRandomColors) buffer->glColoruia(_randomColor.NextColor(), _randomColorAlpha);
		buffer->glVertex3f(-w, +h, +d);

		//Back face
		if(_useRandomColors) buffer->glColoruia(_randomColor.NextColor(), _randomColorAlpha);
		buffer->glVertex3f(+w, -h, -d);
		if(_useRandomColors) buffer->glColoruia(_randomColor.NextColor(), _randomColorAlpha);
		buffer->glVertex3f(-w, -h, -d);
		buffer->glVertex3f(+w, +h, -d);
		buffer->glVertex3f(+w, +h, -d);
		buffer->glVertex3f(-w, -h, -d);
		if(_useRandomColors) buffer->glColoruia(_randomColor.NextColor(), _randomColorAlpha);
		buffer->glVertex3f(-w, +h, -d);

		//Top face
		if(_useRandomColors) buffer->glColoruia(_randomColor.NextColor(), _randomColorAlpha);
		buffer->glVertex3f(+w, +h, -d);
		if(_useRandomColors) buffer->glColoruia(_randomColor.NextColor(), _randomColorAlpha);
		buffer->glVertex3f(-w, +h, -d);
		buffer->glVertex3f(+w, +h, +d);
		buffer->glVertex3f(+w, +h, +d);
		buffer->glVertex3f(-w, +h, -d);
		if(_useRandomColors) buffer->glColoruia(_randomColor.NextColor(), _randomColorAlpha);
		buffer->glVertex3f(-w, +h, +d);

		//Bottom face
		if(_useRandomColors) buffer->glColoruia(_randomColor.NextColor(), _randomColorAlpha);
		buffer->glVertex3f(+w, -h, -d);
		if(_useRandomColors) buffer->glColoruia(_randomColor.NextColor(), _randomColorAlpha);
		buffer->glVertex3f(+w, -h, +d);
		buffer->glVertex3f(-w, -h, -d);
		buffer->glVertex3f(-w, -h, -d);
		buffer->glVertex3f(+w, -h, +d);
		if(_useRandomColors) buffer->glColoruia(_randomColor.NextColor(), _randomColorAlpha);
		buffer->glVertex3f(-w, -h, +d);

		//Right face
		if(_useRandomColors) buffer->glColoruia(_randomColor.NextColor(), _randomColorAlpha);
		buffer->glVertex3f(+w, +h, -d);
		if(_useRandomColors) buffer->glColoruia(_randomColor.NextColor(), _randomColorAlpha);
		buffer->glVertex3f(+w, +h, +d);
		buffer->glVertex3f(+w, -h, -d);
		buffer->glVertex3f(+w, -h, -d);
		buffer->glVertex3f(+w, +h, +d);
		if(_useRandomColors) buffer->glColoruia(_randomColor.NextColor(), _randomColorAlpha);
		buffer->glVertex3f(+w, -h, +d);

		//Left face
		if(_useRandomColors) buffer->glColoruia(_randomColor.NextColor(), _randomColorAlpha);
		buffer->glVertex3f(-w, +h, -d);
		if(_useRandomColors) buffer->glColoruia(_randomColor.NextColor(), _randomColorAlpha);
		buffer->glVertex3f(-w, -h, -d);
		buffer->glVertex3f(-w, +h, +d);
		buffer->glVertex3f(-w, +h, +d);
		buffer->glVertex3f(-w, -h, -d);
		if(_useRandomColors) buffer->glColoruia(_randomColor.NextColor(), _randomColorAlpha);
		buffer->glVertex3f(-w, -h, +d);

		*/

		buffer.glEnd();

		this._vertexBufferID = buffer.getVertexBufferID();
		this._colorBufferID = buffer.getColorBufferID();
		this._vertexCount = buffer.getVertexCount();
	}

}