uniform mat4 projMat;
uniform mat4 modelMat;

attribute vec4 vertex;

void main(void)
{
	gl_Position = projMat * modelMat * vertex;
	//gl_Position = vertex;
}
