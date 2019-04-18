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
		this.modelMat.glLoadIdentity();
		this.modelMat.glTranslatef(this._trans.x, this._trans.y, this._trans.z);
		this.modelMat.glRotatef(this._angle.x,1,0,0);
		this.modelMat.glRotatef(this._angle.y,0,1,0);
		this.modelMat.glTranslatef(-this._pivot.x, -this._pivot.y, -this._pivot.z);
	}
		
	updateCamera()
	{
		if(input.IsKeyPressed(input.shift) && input.IsMiddleMousePressed())
		{
			var dx = input.mouseX.curr - input.mouseX.prev;
			var dy = input.mouseY.curr - input.mouseY.prev;

			var z = this._trans.z;
			if(z < 0)
				z = -z;

			z /= 3000.0;

			this._trans.x += dx*z;
			this._trans.y += -dy*z;

			return true;
		}
		else if(input.IsKeyPressed(input.ctrl) && input.IsMiddleMousePressed())
		{
			this._trans.z += (input.mouseY.prev - input.mouseY.curr) * 2.0;
			return true;
		}
		else if(input.IsMiddleMousePressed())
		{
			var dx = (input.mouseX.curr - input.mouseX.prev);
			var dy = (input.mouseY.curr - input.mouseY.prev);

			this._angle.y += dx * 180.0 / (this.sw*0.5);
			this._angle.x += dy * 180.0 / (this.sw*0.5);

			return true;
		}

		if(input.IsScrollDown())
		{
			this._trans.z -= 45.0;
			return true;
		}
		else if(input.IsScrollUp())
		{
			this._trans.z += 45.0;
			return true;
		}

		return false;
	}
	
}

var cam = new Cam();



