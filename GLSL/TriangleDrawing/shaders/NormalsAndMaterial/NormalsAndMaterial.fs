#version 120

/*
varying vec4 fragColor;

void main (void)  
{     
	gl_FragColor = fragColor;
}
*/


uniform vec3 lightpos;
uniform vec4 ambient;
uniform vec4 diffuse;
uniform vec4 specular;
uniform float shininess;

varying vec3 V;
varying vec3 N;

void main(void)
{
	vec3 l = normalize( lightpos - V );
	vec3 e = normalize(-V);  //we are in eye coordinates, so eyepos is (0,0,0)  
	vec3 r = normalize(-reflect(l,N)); 
		
	vec4 iamb = ambient;
	vec4 idiff = diffuse * max(dot(N,l), 0.0);
	vec4 ispec = specular * pow(max(dot(r,e),0.0), shininess*0.3);

	gl_FragColor =  iamb + idiff + ispec;
}
