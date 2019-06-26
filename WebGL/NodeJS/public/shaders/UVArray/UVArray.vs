uniform mat4 projMat;
uniform mat4 modelMat;
uniform mat4 oriMat;

attribute highp vec2 uv;
attribute highp vec4 vertex;

varying highp vec2 uv_varying;

void main(void)
{
	uv_varying = uv;

	gl_Position = projMat * modelMat * oriMat * vertex;
}
