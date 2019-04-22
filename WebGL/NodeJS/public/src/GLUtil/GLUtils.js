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
		
	static get2DPosVecOnScreenFrom3DPosVec(pos3DVec, modelMat, projMat, sw, sh)
	{
		var vec2d = Array(0);

		var projValues = this.getProjectionValues( projMat );
		
		var l = projValues.l;
		var r = projValues.r;
		var t = projValues.t;
		var b = projValues.b;
		var n = projValues.n;
		var f = projValues.f;

		for(var i=0; i<pos3DVec.length; i++)
		{
			var x = pos3DVec[i].x;
			var y = pos3DVec[i].y;
			var z = pos3DVec[i].z;

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

			vec2d.push( new CVector3(x2D, y2D, 0) );
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
	
}

































