#version 120

attribute vec4 vertex;
attribute vec3 normal;

uniform mat4 projMat;
uniform mat4 viewMat;
uniform mat4 oriMat;
uniform mat3 normalMat;

varying vec3 viewNormal;
varying vec4 viewPosition;

void main()
{
	gl_Position = projMat * viewMat * oriMat * vertex;
	viewNormal = normalMat * normal;
	viewPosition = viewMat * oriMat * vertex;
}