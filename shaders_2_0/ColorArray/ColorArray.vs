uniform mat4 projMat;
uniform mat4 modelMat;
uniform mat4 oriMat;

attribute vec4 color;
attribute vec4 vertex;

varying vec4 fragColor;

void main(void)
{
	fragColor = color;
	gl_Position = projMat * modelMat * oriMat * vertex;
}
