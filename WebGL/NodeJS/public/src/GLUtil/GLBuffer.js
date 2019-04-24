
class GLBuffer
{
	constructor(colorDataPresent, uvDataPresent, normalsDataPresent)
	{
		this.colorDataPresent = colorDataPresent;
		this.uvDataPresent = uvDataPresent;
		this.normalsDataPresent = normalsDataPresent;
		
		this._vertexArr = null;
		this._uvArr = null;
		this._colorArr = null;
		this._normalArr = null;

		this._r = 255;
		this._g = 255;
		this._b = 255;
		this._a = 255;

		this._u = 0;
		this._v = 0;

		this._nx = 0;
		this._ny = 0;
		this._nz = 0;

		this._count = 0;
		this._mode = 0;

		this._vertexBufferID = 0;
		this._colorBufferID = 0;
		this._uvBufferID = 0;
		this._normalBufferID = 0;
	}
	
	//public methods...
	
	glBegin(mode)
	{
		this._mode = mode;
		this._count = 0;

		this._vertexArr = Array(0);
		if(this.colorDataPresent)	this._colorArr = Array(0); //new Uint8List(32);
		if(this.uvDataPresent)		this._uvArr = Array(0);
		if(this.normalsDataPresent)	this._normalArr = Array(0);
	}

	glColor4ub(r, g, b, a)
	{
		this._r = r;
		this._g = g;
		this._b = b;
		this._a = a;
	}
	
	glColoruia(color, alpha)
	{
		this._r	= (color >> 24) & 255;
		this._g	= (color >> 16) & 255;
		this._b	= (color >> 8) & 255;
		this._a = alpha;		
	}

	glTexCoord2f(u, v)
	{
		this._u = u;
		this._v = v;
	}

	glNormal3f(x, y, z)
	{
		this._nx = x;
		this._ny = y;
		this._nz = z;
	}

	glVertex3f(x, y, z)
	{
		if(this._colorArr)
		{
			this._colorArr.push(this._r);
			this._colorArr.push(this._g);
			this._colorArr.push(this._b);
			this._colorArr.push(this._a);
		}

		if(this._uvArr)
		{
			this._uvArr.push(this._u);
			this._uvArr.push(this._v);
		}

		if(this._normalArr)
		{
			this._normalArr.push(this._nx);
			this._normalArr.push(this._ny);
			this._normalArr.push(this._nz);
		}

		this._vertexArr.push(x);
		this._vertexArr.push(y);
		this._vertexArr.push(z);
		
		this._count++;
	}
	
	getVertexBufferID()	{	return this._vertexBufferID;		}
	getColorBufferID()	{	return this._colorBufferID;			}
	getUVBufferID()		{	return this._uvBufferID;			}
	getNormalBufferID()	{	return this._normalBufferID;		}
	
	getVertexCount() 
	{
		return this._count;
	}

	glEnd()
	{		
		if(this._vertexBufferID == 0)
		{
			this._vertexBufferID = gl.createBuffer();
			gl.bindBuffer(gl.ARRAY_BUFFER, this._vertexBufferID);
			gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(this._vertexArr), gl.STATIC_DRAW);

			if(this._colorArr)
			{
				this._colorBufferID = gl.createBuffer();
				gl.bindBuffer(gl.ARRAY_BUFFER, this._colorBufferID);				
				gl.bufferData(gl.ARRAY_BUFFER, new Uint8Array(this._colorArr), gl.STATIC_DRAW);
			}

			if(this._uvArr)
			{
				this._uvBufferID = gl.createBuffer();
				gl.bindBuffer(gl.ARRAY_BUFFER, this._uvBufferID);
				gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(this._uvArr), gl.STATIC_DRAW);
			}

			if(this._normalArr)
			{
				this._normalBufferID = gl.createBuffer();
				gl.bindBuffer(gl.ARRAY_BUFFER, this._normalBufferID);
				gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(this._normalArr), gl.STATIC_DRAW);
			}

			this._vertexArr = null;
			this._colorArr = null;
			this._uvArr = null;
			this._normalArr = null;
		}
	}

}





























