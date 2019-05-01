uniform mat4 projMat;
uniform mat4 modelMat;
uniform mat3 normalMat;

attribute highp vec4 vertex;
attribute highp vec3 normal;
attribute highp vec3 tangent;
attribute highp vec2 uv;

varying highp vec4 vertex_vary;
varying highp vec2 uv_vary;

varying highp mat3 TBN;


void main(void)
{
	gl_Position = projMat * modelMat * vertex;
	uv_vary = uv;	
	vertex_vary = modelMat * vertex;
	
	vec3 NOR = normalize(normalMat * normal);
	vec3 TAN = normalize(normalMat * tangent);
	
    TAN = normalize(TAN - dot(TAN, NOR) * NOR);
	
	TBN = mat3(TAN, cross(TAN, NOR), NOR);
}
