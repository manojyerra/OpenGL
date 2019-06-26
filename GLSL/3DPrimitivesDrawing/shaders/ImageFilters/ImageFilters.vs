
attribute vec2 uv;
attribute vec4 vertex;

varying vec2 uv_varying;

void main(void)
{
	uv_varying = uv;

	gl_Position = gl_ModelViewProjectionMatrix * vertex;
}
