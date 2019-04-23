class ProjValues
{
	constructor(l, r, b, t, n, f)
	{
		this.l = l;
		this.r = r;
		this.b = b;
		this.t = t;
		this.n = n;
		this.f = f;
	}
}

class MinMax
{
	constructor(min, max)
	{
		this.min = new CVector3(min.x, min.y, min.z);
		this.max = new CVector3(max.x, max.y, max.z);
	}
}


class GLUtils
{
	constructor()
	{
	}

	static clearColor(r, g, b, a)
	{
		var prevColor = gl.getParameter(gl.COLOR_CLEAR_VALUE);
		gl.clearColor(r, g, b, a);
		return prevColor;
	}
	
	static enable(cap, enable)
	{
		var prevIsEnabled = gl.isEnabled(cap);

		if(enable)	gl.enable(cap);
		else		gl.disable(cap);

		return prevIsEnabled;
	}

	static lineWidth(width)
	{
		var prevLineWidth = gl.getParameter(gl.LINE_WIDTH);
		gl.lineWidth(width);
		return prevLineWidth;
	}

	static depthFunc(val)
	{	
		var prevDepthFunc = gl.getParameter(gl.DEPTH_FUNC);
		gl.depthFunc(val);
		return prevDepthFunc;
	}
	
	static generateGLTexureID(img)
	{
		var textureID = gl.createTexture();
		gl.bindTexture(gl.TEXTURE_2D, textureID);
		
		gl.pixelStorei(gl.UNPACK_FLIP_Y_WEBGL, true);
		gl.texImage2D(gl.TEXTURE_2D, 0, gl.RGBA, gl.RGBA, gl.UNSIGNED_BYTE, img);
		gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MAG_FILTER, gl.LINEAR);
		gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.LINEAR);
		gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_S, gl.CLAMP_TO_EDGE);
		gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_T, gl.CLAMP_TO_EDGE);
		
		return textureID;
	}

	static generateEmptyTexture(w, h)
	{
		var textureID = gl.createTexture();
		gl.bindTexture(gl.TEXTURE_2D, textureID);
		
		gl.pixelStorei(gl.UNPACK_FLIP_Y_WEBGL, true);
		gl.texImage2D(gl.TEXTURE_2D, 0, gl.RGBA, w, h, 0, gl.RGBA, gl.UNSIGNED_BYTE, null);
		gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MAG_FILTER, gl.LINEAR);
		gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.LINEAR);
		gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_S, gl.CLAMP_TO_EDGE);
		gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_T, gl.CLAMP_TO_EDGE);
		
		return textureID;
	}
	
	static getProjectionValues( projMat )
	{
		var m = projMat;

		var C1 = (m[10] + 1.0) / (m[10] - 1.0);
		var n = (m[14] * C1 - m[14]) / 2.0;
		var f = n * (m[10] - 1.0) / (m[10] + 1.0);
		var C2 = 2 * n / m[0];
		var l = ( m[8] * C2 - C2 ) / 2.0;
		var r = l * ( m[8] + 1.0 ) / ( m[8] - 1.0 );
		var C3 = 2.0 * n / m[5];
		var b = ( m[9] * C3 - C3 ) / 2.0;
		var t = b * ( m[9] + 1.0 ) / ( m[9] - 1.0 );
		
		return new ProjValues(l, r, b, t, n, f);
	}

	static get2DPosOnScreenFrom3DPos(pos3D, modelMat, projMat, sw, sh)
	{
		var x = pos3D.x;
		var y = pos3D.y;
		var z = pos3D.z;

		var a = modelMat;

		var xWPos = a[0]*x + a[4]*y + a[8]*z + a[12];
		var yWPos = a[1]*x + a[5]*y + a[9]*z + a[13];
		var zWPos = a[2]*x + a[6]*y + a[10]*z + a[14];

		var projValues = this.getProjectionValues( projMat );
		
		var l = projValues.l;
		var r = projValues.r;
		var t = projValues.t;
		var b = projValues.b;
		var n = projValues.n;
		var f = projValues.f;

		var xOnZNear = -n * xWPos / zWPos;
		var yOnZNear = -n * yWPos / zWPos;

		if(f < 0)
		{
			xOnZNear = xWPos;
			yOnZNear = yWPos;
		}
		
		var zNearW = Math.abs(r-l);
		var zNearH = Math.abs(t-b);

		var pos2DX = (( xOnZNear - l ) / zNearW) * sw;
		var pos2DY = sh - ((( yOnZNear - b ) / zNearH) * sh);
		
		return new CVector3(pos2DX, pos2DY, 0.0);
	}
		
	static get2DPosArrOnScreenFrom3DPosArr(vertexArr, numVertex, modelMat, projMat, sw, sh)
	{
		var vec2d = Array(0);

		var projValues = this.getProjectionValues( projMat );
		
		var l = projValues.l;
		var r = projValues.r;
		var t = projValues.t;
		var b = projValues.b;
		var n = projValues.n;
		var f = projValues.f;

		for(var i=0; i<numVertex*3; i+=3)
		{
			var x = vertexArr[i+0];
			var y = vertexArr[i+1];
			var z = vertexArr[i+2];

			var a = modelMat;

			var xWPos = a[0]*x + a[4]*y + a[8]*z + a[12];
			var yWPos = a[1]*x + a[5]*y + a[9]*z + a[13];
			var zWPos = a[2]*x + a[6]*y + a[10]*z + a[14];

			var xOnZNear = -n * xWPos / zWPos;
			var yOnZNear = -n * yWPos / zWPos;

			if(f < 0)
			{
				xOnZNear = xWPos;
				yOnZNear = yWPos;
			}
		
			var zNearW = Math.abs(r-l);
			var zNearH = Math.abs(t-b);

			var x2D = (( xOnZNear - l ) / zNearW) * sw;
			var y2D = sh - ((( yOnZNear - b ) / zNearH) * sh);

			vec2d.push(x2D);
			vec2d.push(y2D);
		}

		return vec2d;
	}
		
	static getMinMaxPoints(points3D)
	{
		var minX = points3D[0].x;
		var minY = points3D[0].y;
		var minZ = points3D[0].z;

		var maxX = points3D[0].x;
		var maxY = points3D[0].y;
		var maxZ = points3D[0].z;

		var size = points3D.length;

		for(var i=1; i<size; i++)
		{
			if(points3D[i].x < minX) minX = points3D[i].x;
			if(points3D[i].y < minY) minY = points3D[i].y;
			if(points3D[i].z < minZ) minZ = points3D[i].z;

			if(points3D[i].x > maxX) maxX = points3D[i].x;
			if(points3D[i].y > maxY) maxY = points3D[i].y;
			if(points3D[i].z > maxZ) maxZ = points3D[i].z;
		}

		return new MinMax( new CVector3(minX, minY, minZ), new CVector3(maxX, maxY, maxZ) );
	}
	
	static getVerticesOnRect(vertexArr, numVertex, modelMat, projMat, objMat, x, y, w, h, sw, sh)
	{
		var modelAndOriMat = new GLMat();
		modelAndOriMat.copy(modelMat);
		
		if(objMat != null)
			modelAndOriMat.glMultMatrixf(objMat);
		
		var vertex2DArr = this.get2DPosArrOnScreenFrom3DPosArr(vertexArr, numVertex, modelAndOriMat.m, projMat, sw, sh)
		
		var verArrOnRect = Array(0);
		
		for(var i=0; i<vertex2DArr.length; i+=2)
		{
			var xx = vertex2DArr[i+0];
			var yy = vertex2DArr[i+1];
			
			if(xx >= x && xx <= x+w && yy >= y && yy <= y+h)
			{
				verArrOnRect.push(xx);
				verArrOnRect.push(yy);
			}
		}

		return verArrOnRect;
	}	
}

































