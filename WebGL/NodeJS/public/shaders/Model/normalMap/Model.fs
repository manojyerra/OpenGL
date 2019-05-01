uniform highp vec3 lightPos;
uniform highp vec4 ambient;
uniform highp vec4 diffuse;
uniform highp vec4 specular;
uniform highp float shininess;

uniform sampler2D baseTexture;
uniform sampler2D normalMap;
uniform sampler2D metallic;
//uniform sampler2D unityGloss;

varying lowp vec4 vertex_vary;
varying lowp vec2 uv_vary;

varying highp mat3 TBN;

highp vec4 CalcLightColor(highp vec3 V, highp vec3 N)
{
	highp vec3 L = normalize( lightPos - V );
	highp vec3 E = normalize(-V); // we are in Eye Coordinates, so EyePos is (0,0,0)  
	highp vec3 R = normalize(-reflect(L,N)); 
		
	highp vec4 Iamb = ambient;
	highp vec4 Idiff = diffuse * max(dot(N,L), 0.0);
	highp vec4 Ispec = specular * pow(max(dot(R,E),0.0), shininess*0.3);

	return ( Iamb + Idiff + Ispec );	
}

void main (void)  
{
    highp vec3 normalMapVal = texture2D(normalMap, uv_vary).xyz;
	
	highp vec3 N = normalize( TBN * normalize(2.0 * normalMapVal - 1.0) );
	highp vec4 lightColor = CalcLightColor(vertex_vary.xyz, N);
		
	gl_FragColor = lightColor * texture2D(baseTexture, uv_vary); //* texture2D(metallic, uv_vary);	
}

