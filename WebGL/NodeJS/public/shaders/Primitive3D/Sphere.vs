uniform mat4 projMat;
uniform mat4 modelMat;
uniform mat3 normalMat;
uniform mat4 oriMat;
uniform mat4 scaleMat;


uniform vec3 lightPos;
uniform vec4 ambient;
uniform vec4 diffuse;
uniform vec4 specular;
uniform float shininess;

attribute highp vec4 vertex;
attribute highp vec3 normal;
attribute highp vec2 uv;

varying highp vec4 lightColor;


void CalcLightColor(vec3 V, vec3 N)
{
	vec3 L = normalize( lightPos - V );
	vec3 E = normalize(-V); // we are in Eye Coordinates, so EyePos is (0,0,0)  
	vec3 R = normalize(-reflect(L,N)); 
		
	vec4 Iamb = ambient;
	vec4 Idiff = diffuse * max(dot(N,L), 0.0);
	vec4 Ispec = specular * pow(max(dot(R,E),0.0), shininess*0.3);

	lightColor =  Iamb + Idiff + Ispec;
}


void main(void)
{
	vec3 V1 = vec3( modelMat * vertex );
	vec3 N1 = normalize(normalMat * normal);
	
	CalcLightColor(V1, N1);

	gl_Position = projMat * modelMat * oriMat * scaleMat * vertex;
}
