#ifndef Sphere_H
#define Sphere_H

#include "Shape.h"
#include "ShaderProgram.h"
#include "GLUtil/GLBuffer.h"

class Sphere : public Shape
{
private:
	float _r;

	GLuint _vertexBufferID;
	GLuint _normalBufferID;
	GLuint _colorBufferID;
	GLuint _vertexCount;

	ShaderProgram* _shaderProgram;

	void InitShader();

	void GenerateBufferID();
	void rot(int axis, float angleInDegrees, float x, float y, float z, float* newX, float* newY, float* newZ);

public:
	Sphere(float x, float y, float z, float r);
	Sphere(Sphere* sphere);
	Sphere(CVector3 pos, float r);
	Sphere(float* mat, float r);
	~Sphere();

	float GetRadius();
	void SetRadius(float r);
	static Sphere CalcBoundingSphere(float* vertexBuf, int arrSize);
	float Volume();

	void Draw();
};

#endif


	//varying vec3 N;
	//varying vec3 v;

	//v = vec3(gl_ModelViewMatrix * gl_Vertex);       
	//N = normalize(gl_NormalMatrix * gl_Normal);

	//gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;

	//vec3 L = normalize(gl_LightSource[0].position.xyz - v);   
	//vec3 E = normalize(-v); // we are in Eye Coordinates, so EyePos is (0,0,0)  
	//vec3 R = normalize(-reflect(L,N)); 

	//vec4 Iamb = gl_FrontLightProduct[0].ambient;															//calculate Ambient Term: 
	//vec4 Idiff = gl_FrontLightProduct[0].diffuse * max(dot(N,L), 0.0);										//calculate Diffuse Term:
	//vec4 Ispec = gl_FrontLightProduct[0].specular * pow(max(dot(R,E),0.0),0.3*gl_FrontMaterial.shininess);	//calculate Specular Term:

	//gl_FragColor = gl_FrontLightModelProduct.sceneColor + Iamb + Idiff + Ispec;		// write Total Color:  



	//vec3 L = normalize( lightPos - V );
	//vec3 E = normalize(-V); // we are in Eye Coordinates, so EyePos is (0,0,0)  
	//vec3 R = normalize(-reflect(L,N)); 
	//	
	//vec4 Iamb = ambient;
	//vec4 Idiff = diffuse * max(dot(N,L), 0.0);
	//vec4 Ispec = specular * pow(max(dot(R,E),0.0), shininess*0.3);

	//fragColor = Iamb + Idiff + Ispec;