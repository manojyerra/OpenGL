#version 120

uniform mat4 projMat;
uniform mat4 viewMat;
uniform mat4 modelMat;

attribute vec4 vertex;

void main (void)  
{     
   gl_Position = projMat * viewMat * modelMat * vertex;
}
