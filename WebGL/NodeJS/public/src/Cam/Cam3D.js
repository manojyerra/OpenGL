var cam3DInstanceCount = 0;

class Cam3D {
	
	constructor()
	{
		cam3DInstanceCount++;
		
		if(cam3DInstanceCount > 1) {
			throw "More than one object creation is not allowed for Cam Class";
		}		
	}
	
	init(screenW, screenH, zNear, zFar, zNearPlaneW) {
			
		this.projMat = new GLMat();
		this.modelMat = new GLMat();
		this.normalMat = Array(9).fill(0.0);
		this.normalMat[0] = this.normalMat[4] = this.normalMat[8] = 1.0;
		
		this.sw = screenW;
		this.sh = screenH;
				
		this._zNear  = zNear;
		this._zFar = zFar;
		this._zNearPlaneHalfW = zNearPlaneW/2.0;

		this._pivot = new CVector3(0, 0, 0);
		this._trans = new CVector3(0, 0, -270.0);
		this._angle = new CVector3(30, 0, 0);

		this.setPerspectiveView();
		this.setModelViewMatrix();
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
		

		this.normalMat[0] = this.modelMat.m[0];
		this.normalMat[1] = this.modelMat.m[1];
		this.normalMat[2] = this.modelMat.m[2];
		
		this.normalMat[3] = this.modelMat.m[4];
		this.normalMat[4] = this.modelMat.m[5];
		this.normalMat[5] = this.modelMat.m[6];

		this.normalMat[6] = this.modelMat.m[8];
		this.normalMat[7] = this.modelMat.m[9];
		this.normalMat[8] = this.modelMat.m[10];
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
			
			this.setModelViewMatrix();
			return true;
		}
		else if(input.IsKeyPressed(input.ctrl) && input.IsMiddleMousePressed())
		{
			this._trans.z += (input.mouseY.prev - input.mouseY.curr) * 2.0;
			
			this.setModelViewMatrix();
			return true;
		}
		else if(input.IsMiddleMousePressed())
		{
			var dx = (input.mouseX.curr - input.mouseX.prev);
			var dy = (input.mouseY.curr - input.mouseY.prev);

			this._angle.y += dx * 180.0 / (this.sw*0.5);
			this._angle.x += dy * 180.0 / (this.sw*0.5);

			this.setModelViewMatrix();
			return true;
		}

		if(input.IsScrollDown())
		{
			this._trans.z -= 45.0;
			
			this.setModelViewMatrix();
			return true;
		}
		else if(input.IsScrollUp())
		{
			this._trans.z += 45.0;
			
			this.setModelViewMatrix();
			return true;
		}

		return false;
	}
	
}

var cam3D = new Cam3D();



