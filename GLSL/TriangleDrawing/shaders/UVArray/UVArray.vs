attribute vec2 uv;
attribute vec4 vertex;

varying vec2 uvVary;

void main(void)
{
	uvVary = uv;
	gl_Position = gl_ModelViewProjectionMatrix * vertex;
}
