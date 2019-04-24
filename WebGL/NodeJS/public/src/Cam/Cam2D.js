var cam2DInstanceCount = 0;

class Cam2D {
	
	constructor()
	{
		cam2DInstanceCount++;
		
		if(cam2DInstanceCount > 1) {
			throw "More than one object creation is not allowed for Cam Class";
		}
	}
	
	//public methods...
	
	init(screenW, screenH) {
		
		this.sw = screenW;
		this.sh = screenH;

		this.projMat = new GLMat();	
		this.projMat.glOrtho(0, this.sw, this.sh, 0, 0, 1);
		
		this.modelMat = new GLMat();
	}
}

var cam2D = new Cam2D();



