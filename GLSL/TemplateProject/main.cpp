#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include "glsl/glsl.h"

int CreateGlutWindow(char* title, int x, int y, int w, int h);
void Display();

glShaderManager shaderMgr;
glShader *shader;
GLuint shaderVarLoc;

GLuint vertexbuffer;

static const GLfloat g_vertex_buffer_data[] = {
   -1.0f, -1.0f, 0.0f,
   1.0f, -1.0f, 0.0f,
   0.0f,  1.0f, 0.0f,
};

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	int windowID = CreateGlutWindow("First Window", 0,0,500,500);

	shader = shaderMgr.loadfromFile("shaders/vertexshader.txt","shaders/fragmentshader.txt"); // load (and compile, link) from file

	if(shader == 0)
		std::cout << "Error Loading, compiling or linking shader\n";

	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	GLuint programObject = shader->GetProgramObject();
	shaderVarLoc = glGetUniformLocation(programObject, "shaderVarLoc");

	glutDisplayFunc(Display);
	glutMainLoop();

	return 0;
}

int CreateGlutWindow(char* title, int x, int y, int w, int h)
{
	glutInitWindowPosition(x,y);
	glutInitWindowSize(w,h);
	return glutCreateWindow(title);
}

void Display()
{
	glClearColor(0,0,0,1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if(shader) 
		shader->begin();

    glUniform1f(shaderVarLoc, 0.5f);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glVertexAttribPointer(
	   0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
	   3,                  // size
	   GL_FLOAT,           // type
	   GL_FALSE,           // normalized?
	   0,                  // stride
	   (void*)0            // array buffer offset
	);
	glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle
	glDisableVertexAttribArray(0);


	if(shader)
		shader->end();

	glutSwapBuffers();
}
