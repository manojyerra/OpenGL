class CVector3
{
	constructor(x, y, z)
	{
		this.x = x;
		this.y = y;
		this.z = z;
	}
	
	length()
	{
		return Math.sqrt( this.x*this.x + this.y*this.y + this.z*this.z );
	}

	normalize()
	{
		var length = this.length();
		
		if( length == 0.0 )
			return;
		
		var factor = 1.0/length;
		
		this.x *= factor;
		this.y *= factor;
		this.z *= factor;
	}
}
