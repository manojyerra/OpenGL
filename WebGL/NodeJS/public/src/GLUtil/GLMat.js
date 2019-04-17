var DEG_RAD = 0.0174532925194;

class GLMat
{
	constructor() {
		
		this.m = Array(16).fill(0.0);
		this.m[0] = this.m[5] = this.m[10] = this.m[15] = 1.0;
	}
	
	glLoadIdentity() {
		
		for(var i=0; i<16; i++) {
			this.m[i] = 0.0;
		}
		
		this.m[0] = this.m[5] = this.m[10] = this.m[15] = 1.0;
	}
	
	glTranslatef(x, y, z) {
		
		var trans = [
			1.0, 0.0, 0.0, 0.0,
			0.0, 1.0, 0.0, 0.0,
			0.0, 0.0, 1.0, 0.0,
			x,   y,   z,   1.0
		];
		
		var result = this.multMat(this.m, trans);

		for(var i=0; i<16; i++) {
			this.m[i] = result[i];
		}
	}
	
	glRotatef(angleInDegrees, isX, isY, isZ)
	{
		var rot;

		if(isX == 1)
			rot = this.getXRotArray(angleInDegrees);
		else if(isY == 1)
			rot = this.getYRotArray(angleInDegrees);
		else if(isZ == 1)
			rot = this.getZRotArray(angleInDegrees);

		var result = this.multMat(this.m, rot);

		for(var i=0; i<16; i++) {
			this.m[i] = result[i];
		}
	}
	
	glScalef(x, y, z)
	{
		var scaleMat = [
		x,   0.0, 0.0, 0.0,
		0.0, y,   0.0, 0.0, 
		0.0, 0.0, z,   0.0, 
		0.0, 0.0, 0.0, 1.0
		];
		
		var result = this.multMat(scaleMat, m);

		for(var i=0; i<16; i++) {
			this.m[i] = result[i];
		}
	}

	glMultMatrixf(mat)
	{
		var result = this.multMat(m, mat);

		for(var i=0; i<16; i++) {
			this.m[i] = result[i];
		}
	}
	
	getXRotArray(angleInDegrees)
	{
		var result = Array(16).fill(0.0);
		result[0] = result[5] = result[10] = result[15] = 1.0;
		
		var angleInRad = angleInDegrees * DEG_RAD;
		
		result[5] = Math.cos(angleInRad);
		result[6] = Math.sin(angleInRad);
		result[9] = -Math.sin(angleInRad);
		result[10] = Math.cos(angleInRad);
		
		return result;
	}

	getYRotArray(angleInDegrees)
	{
		var result = Array(16).fill(0.0);
		result[0] = result[5] = result[10] = result[15] = 1.0;
		
		var angleInRad = angleInDegrees * DEG_RAD;

		result[0] = Math.cos(angleInRad);
		result[2] = -Math.sin(angleInRad);
		result[8] = Math.sin(angleInRad);
		result[10] = Math.cos(angleInRad);
		
		return result;
	}

	getZRotArray(angleInDegrees)
	{
		var result = Array(16).fill(0.0);
		result[0] = result[5] = result[10] = result[15] = 1.0;
		
		var angleInRad = angleInDegrees * DEG_RAD;

		result[0] = Math.cos(angleInRad);
		result[1] = Math.sin(angleInRad);
		result[4] = -Math.sin(angleInRad);
		result[5] = Math.cos(angleInRad);
		
		return result;
	}
	
	multMat(a, b, result)
	{
		var result = Array(16).fill(0.0);
		
		for(var i=0;i<4;i++)
		{
			var j = i*4;
			result[j+0] = a[0]*b[j] + a[4]*b[j+1] + a[8] *b[j+2] + a[12]*b[j+3];
			result[j+1] = a[1]*b[j] + a[5]*b[j+1] + a[9] *b[j+2] + a[13]*b[j+3];
			result[j+2] = a[2]*b[j] + a[6]*b[j+1] + a[10]*b[j+2] + a[14]*b[j+3];
			result[j+3] = a[3]*b[j] + a[7]*b[j+1] + a[11]*b[j+2] + a[15]*b[j+3];
		}
		
		return result;
	}
	
	glFrustum(left, right, bottom, top, zNear, zFar)
	{ 
		var a = (right + left) / (right - left);
		var b = (top + bottom) / (top - bottom);
		var c = - (zFar + zNear) / (zFar - zNear);
		var d = - (2.0*zFar*zNear) / (zFar - zNear);		
		
		var x = 2.0 * zNear / (right - left) ;
		var y = 2.0 * zNear / (top - bottom) ;
		var e = -1.0;
		
		this.m = [
			x, 0, 0, 0,
			0, y, 0, 0,
			a, b, c, e,
			0, 0, d, 0
		];
	}
	
	glOrtho(left, right, bottom, top, near, far)
	{ 
		var a = 2.0 / (right - left);
		var b = 2.0 / (top - bottom);
		var c = -2.0 / (far - near);

		var x = - (right + left)/(right - left);
		var y = - (top + bottom)/(top - bottom);
		var z = - (far + near)/(far - near);

		this.m = [
			a, 0, 0, 0,
			0, b, 0, 0,
			0, 0, c, 0,
			x, y, z, 1
		];
	}
	
}