attribute vec4 color;
attribute vec4 vertex;

varying vec4 colorVary;

void main(void)
{
	colorVary = color;
	gl_Position = gl_ModelViewProjectionMatrix * vertex;
}
