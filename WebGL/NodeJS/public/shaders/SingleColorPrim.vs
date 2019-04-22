uniform mat4 projMat;
uniform mat4 modelMat;
uniform mat4 oriMat;

attribute highp vec4 vertex;

void main(void)
{
	gl_Position = projMat * modelMat * oriMat * vertex;
}
