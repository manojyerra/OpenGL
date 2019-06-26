uniform mat4 projMat;
uniform mat4 modelMat;
uniform mat4 oriMat;
uniform mat4 scaleMat;

attribute highp vec4 color;
attribute highp vec4 vertex;

varying highp vec4 fragColor;

void main(void)
{
	fragColor = color;

	gl_Position = projMat * modelMat * oriMat * scaleMat * vertex;
}
