var camInstanceCount = 0;

class Cam {
	
	constructor()
	{
	}
	
	init(screenW, screenH, zNear, zFar, zNearPlaneW) {
		
		camInstanceCount++;
		
		if(camInstanceCount > 1) {
			throw "More than one object creation is not allowed for Cam Class";
		}
		
		this.projMat = new GLMat();
		this.modelMat = new GLMat();
		
		this.sw = screenW;
		this.sh = screenW;
				
		this._zNear  = zNear;
		this._zFar = zFar;
		this._zNearPlaneHalfW = zNearPlaneW/2.0;

		this._pivot = new CVector3(0, 0, 0);
		this._trans = new CVector3(0, 0, -70.0);
		this._angle = new CVector3(30, 0, 0);

		this.setPerspectiveView();		
	}

	setPerspectiveView()
	{
		this._left = -this._zNearPlaneHalfW;
		this._right = this._zNearPlaneHalfW;
		this._bottom = -this._zNearPlaneHalfW*this.sh/this.sw;
		this._top = this._zNearPlaneHalfW*this.sh/this.sw;

		this.projMat.glLoadIdentity();
		this.projMat.glFrustum(this._left, this._right, this._bottom, this._top, this._zNear, this._zFar);
	}
	
	setModelViewMatrix()
	{
		this.modelMat.glTranslatef(this._trans.x, this._trans.y, this._trans.z);
		this.modelMat.glRotatef(this._angle.x,1,0,0);
		this.modelMat.glRotatef(this._angle.y,0,1,0);
		this.modelMat.glTranslatef(-this._pivot.x, -this._pivot.y, -this._pivot.z);
	}
	
}

var cam = new Cam();



