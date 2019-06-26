#version 120

/*
attribute vec4 vertex;
attribute vec3 normal;

uniform vec3 lightPos;
uniform vec4 ambient;
uniform vec4 diffuse;
uniform vec4 specular;
uniform float shininess;

varying vec4 fragColor;

void CalcFragColor(vec3 V, vec3 N)
{
	vec3 L = normalize( lightPos - V );
	vec3 E = normalize(-V); // we are in Eye Coordinates, so EyePos is (0,0,0)  
	vec3 R = normalize(-reflect(L,N)); 
		
	vec4 Iamb = ambient;
	vec4 Idiff = diffuse * max(dot(N,L), 0.0);
	vec4 Ispec = specular * pow(max(dot(R,E),0.0), shininess*0.3);

	fragColor =  Iamb + Idiff + Ispec;
}

void main(void)
{
	vec3 V1 = vec3( gl_ModelViewMatrix * vertex );
	vec3 N1 = normalize( gl_NormalMatrix * normal );
	
	CalcFragColor(V1, N1);

	gl_Position = gl_ModelViewProjectionMatrix * vertex;
}
*/

// #version 120

attribute vec4 vertex;
attribute vec3 normal;

varying vec3 V;
varying vec3 N;

void main(void)
{
	V = vec3( gl_ModelViewMatrix * vertex );
	N = normalize( gl_NormalMatrix * normal );

	gl_Position = gl_ModelViewProjectionMatrix * vertex;
}
