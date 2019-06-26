
//Per vertex lighting
/*
#version 120

varying vec4 fragColor;

void main (void)  
 {     
	gl_FragColor = fragColor;
}
*/



//Per pixel lighting
#version 120

uniform vec3 lightPos;
uniform vec4 ambient;
uniform vec4 diffuse;
uniform vec4 specular;
uniform float shininess;

varying vec3 V;
varying vec3 N;

void main(void)
{
	vec3 L = normalize( lightPos - V );
	vec3 E = normalize(-V); // we are in Eye Coordinates, so EyePos is (0,0,0)  
	vec3 R = normalize(-reflect(L,N)); 
		
	vec4 Iamb = ambient;
	vec4 Idiff = diffuse * max(dot(N,L), 0.0);
	vec4 Ispec = specular * pow(max(dot(R,E),0.0), shininess*0.3);

	gl_FragColor =  Iamb + Idiff + Ispec;
}
