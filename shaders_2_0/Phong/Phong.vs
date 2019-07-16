#version 120

uniform mat4 projMat;
uniform mat4 modelMat;
uniform mat3 normalMat;
uniform mat4 oriMat;

attribute vec4 vertex;
attribute vec3 normal;

varying vec3 V;
varying vec3 N;

void main(void)
{
	V = vec3( modelMat * vertex );
	N = normalize( normalMat * normal );

	gl_Position = projMat * modelMat * oriMat * vertex;
}
