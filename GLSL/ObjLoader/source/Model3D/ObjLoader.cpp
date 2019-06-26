#include "ObjLoader.h"
#include "CFileReader.h"
#include "ImageBuffer.h"
#include "GLUtil.h"
#include "ShadersManager.h"
#include "GLBuffer.h"
#include "Vector3.h"

ObjLoader::ObjLoader(string folderPath)
{
	ReadObjFile(folderPath+"/objFile.obj");
	//LoadTextures(folderPath);

	_shaderProgram = ShadersManager::GetInstance()->CreateShaderProgram("shaders/NormalsAndMaterial/NormalsAndMaterial.vs",
		"shaders/NormalsAndMaterial/NormalsAndMaterial.fs");
}

void ObjLoader::ReadObjFile(string filePath)
{
	CFileReader fileReader(filePath.c_str(), "rb");
	
	char* line = NULL;

	float vx, vy, vz;
	float tx, ty;
	float nx, ny, nz;

	vector<CVector3> vertexVec;
	vector<CVector3> uvVec;
	vector<CVector3> normalVec;

	while( (line = fileReader.ReadLine())!= NULL)
	{
		if(line[0] == 'v' && line[1] == ' ')
		{
			sscanf(line, "v %f %f %f", &vx, &vy, &vz);
			vertexVec.push_back(CVector3(vx, vy, vz));
		}
		else if(line[0] == 'v' && line[1] == 't')
		{
			sscanf(line, "vt %f %f", &tx, &ty);
			uvVec.push_back(CVector3(tx, 1-ty, 0));
		}
		else if(line[0] == 'v' && line[1] == 'n')
		{
			sscanf(line, "vn %f %f %f", &nx, &ny, &nz);
			normalVec.push_back(CVector3(nx, ny, nz));
		}

		free(line);
	}

	fileReader.Reset();

	int v[3];
	int t[3];
	int n[3];
			
	for(int i=0; i<3; i++)
	{
		v[i] = 0;
		t[i] = 0;
		n[i] = 0;
	}

	bool uvsExist = uvVec.size() > 1;
	bool normalsExist = normalVec.size() > 1;

	GLBuffer* buffer = new GLBuffer(1024, false, uvsExist, normalsExist);

	buffer->glBegin(GL_TRIANGLES);


	while((line = fileReader.ReadLine())!= NULL)
	{
		if(line[0] == 'f' && line[1] == ' ')
		{
			if(uvsExist && normalsExist)
				sscanf(line, "f %d/%d/%d %d/%d/%d %d/%d/%d", &v[0],&t[0],&n[0], &v[1],&t[1],&n[1], &v[2],&t[2],&n[2]);
			else if(uvsExist && !normalsExist)
				sscanf(line, "f %d/%d %d/%d %d/%d", &v[0],&t[0], &v[1],&t[1], &v[2],&t[2]);
			else if(!uvsExist && normalsExist)
				sscanf(line, "f %d//%d %d//%d %d//%d", &v[0],&n[0], &v[1],&n[1], &v[2],&n[2]);
			else if(!uvsExist && !normalsExist)
				sscanf(line, "f %d %d %d", &v[0], &v[1], &v[2]);

			for(int i=0; i<3; i++)
			{
				v[i]--;
				t[i]--;
				n[i]--;
			}

			for(int i=0; i<3; i++)
			{
				buffer->glVertex3f(vertexVec[v[i]]);

				if(uvsExist)
				{
					buffer->glTexCoord2f(uvVec[t[i]]);
				}

				if(normalsExist)
				{
					buffer->glNormal3f(normalVec[n[i]]);	
				}
			}
		}

		free(line);
	}

	buffer->glEnd();

	_vertexBufferID = buffer->GetVertexBufferID();
	_normalBufferID = buffer->GetNormalBufferID();
	_uvBufferID = buffer->GetUVBufferID();

	_vertexCount = buffer->GetVertexCount();

	delete buffer;
}

void ObjLoader::Draw()
{
	//GLboolean glLighting = GLUtil::GLEnable(GL_LIGHTING, false);
	GLboolean blend = GLUtil::GLEnable(GL_BLEND, true);
	GLboolean depthTest = GLUtil::GLEnable(GL_DEPTH_TEST, true);

	//glPushMatrix();
	//glMultMatrixf(m);

	//glScalef(_w, _h, _d);

	_shaderProgram->Begin();

	//uniform vec3 lightPos;
	//uniform vec4 ambient;
	//uniform vec4 diffuse;
	//uniform vec4 specular;
	//uniform float shininess;

	glUniform3f(glGetUniformLocation(_shaderProgram->ProgramID(), "lightPos"), 0.0, 0.0, 0.0);
	glUniform4f(glGetUniformLocation(_shaderProgram->ProgramID(), "ambient"), 0.000000, 0.000000, 0.000000, 1.0);
	glUniform4f(glGetUniformLocation(_shaderProgram->ProgramID(), "diffuse"), 0.800000, 0.800000, 0.800000, 1.0);
	glUniform4f(glGetUniformLocation(_shaderProgram->ProgramID(), "specular"), 0.200000, 0.200000, 0.200000, 1.0);
	glUniform1f(glGetUniformLocation(_shaderProgram->ProgramID(), "shininess"), 0.3);

	//GLuint colorID = glGetAttribLocation(_shaderProgram->ProgramID(), "color");
	//glEnableVertexAttribArray(colorID);
	//glBindBuffer(GL_ARRAY_BUFFER, _colorBufferID);
	//glVertexAttribPointer(colorID, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, (void*)0);

	GLuint normalID = glGetAttribLocation(_shaderProgram->ProgramID(), "normal");
	glEnableVertexAttribArray(normalID);
	glBindBuffer(GL_ARRAY_BUFFER, _normalBufferID);
	glVertexAttribPointer(normalID, 3, GL_FLOAT, GL_TRUE, 0, (void*)0);

	GLuint vertexID = glGetAttribLocation(_shaderProgram->ProgramID(), "vertex");
	glEnableVertexAttribArray(vertexID);
	glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferID);
	glVertexAttribPointer(vertexID, 3, GL_FLOAT, GL_TRUE, 0, (void*)0);

	glDrawArrays(GL_TRIANGLES, 0, _vertexCount);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDisableVertexAttribArray(vertexID);
	//glDisableVertexAttribArray(colorID);

	_shaderProgram->End();

	//glPopMatrix();

	//GLUtil::GLEnable(GL_LIGHTING, glLighting);
	GLUtil::GLEnable(GL_BLEND, blend);
	GLUtil::GLEnable(GL_DEPTH_TEST, depthTest);
}


void ObjLoader::LoadTextures(string folderPath)
{
	ImageBuffer* imgBuf = new ImageBuffer(folderPath + "/texture.png");

	_baseTexID = GLUtil::GenerateGLTextureID(imgBuf->GetWidth(), imgBuf->GetHeight(),
		imgBuf->GetBytesPerPixel(), imgBuf->GetBuffer());

	delete imgBuf;
}