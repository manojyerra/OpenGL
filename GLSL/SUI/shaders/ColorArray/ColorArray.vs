#version 120

attribute vec4 color;
attribute vec4 vertex;

varying vec4 fragColor;

void main(void)
{
	fragColor = color;

	gl_Position = gl_ModelViewProjectionMatrix * vertex;
}
