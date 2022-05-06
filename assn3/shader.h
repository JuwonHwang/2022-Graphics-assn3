#pragma once
#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/vec3.hpp>

#include "textReader.h"

//OpenGL
void initGL();					//opengl 초기화

//GLUT
void changeSize(int w, int h);	//윈도우 크기 변경 시 호출되는 callback

//GLEW
void initGLEW();			//GLEW 초기화

//Shader
GLuint v_shader;			//vertex shader handle
GLuint f_shader;			//fragment shader handle
GLuint program_shader;		//shader program handle
void setShaders();			//Shader 설정

GLuint MatrixID;
GLuint vertexPosition_modelspaceID;

//Logging
#define printOpenGLError() printOglError(__FILE__, __LINE__)
int printOglError(char* file, int line);
void printShaderInfoLog(GLuint obj);
void printProgramInfoLog(GLuint obj);

//////////////////////////
void changeSize(int w, int h) {
	if (h == 0)	h = 1;
	float ratio = 1.f * w / h;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, w, h);
	glOrtho(-1.f, 1.f, -1.f, 1.f, -1.f, 1.f);
	glMatrixMode(GL_MODELVIEW);
}


void initGL()
{
	//Enable/Disable
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);

	//Rendering
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glFrontFace(GL_CCW);
	glColor3f(0.0f, 0.5f, 1.0f);

	//Modelview and projection
	glMatrixMode(GL_PROJECTION);
}

void initGLEW()
{
	//Initialize GLEW
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		exit(0);
	}
	fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));

	//Check Shader
	//ARB
	if (GLEW_ARB_vertex_shader && GLEW_ARB_fragment_shader)
		printf("Ready for GLSL (ARB)\n");
	else
	{
		printf("No GLSL support\n");
		exit(0);
	}

}

//GLuint v_shader, f_shader, program_shader
void setShaders()
{
	char* vs = NULL, * fs = NULL, * fs2 = NULL;

	v_shader = glCreateShader(GL_VERTEX_SHADER);
	f_shader = glCreateShader(GL_FRAGMENT_SHADER);

	vs = textFileRead("vertex.shader");
	fs = textFileRead("fragment.shader");

	const char* vv = vs;
	const char* ff = fs;

	glShaderSource(v_shader, 1, &vv, NULL);
	glShaderSource(f_shader, 1, &ff, NULL);

	free(vs); free(fs);

	glCompileShader(v_shader);
	glCompileShader(f_shader);

	printShaderInfoLog(v_shader);
	printShaderInfoLog(f_shader);

	program_shader = glCreateProgram();
	glAttachShader(program_shader, v_shader);
	glAttachShader(program_shader, f_shader);

	glLinkProgram(program_shader);
	printProgramInfoLog(program_shader);

	glUseProgram(program_shader);
}


#define printOpenGLError() printOglError(__FILE__, __LINE__)
int printOglError(char* file, int line)
{
	GLenum glErr;
	int    retCode = 0;

	glErr = glGetError();
	while (glErr != GL_NO_ERROR)
	{
		printf("glError in file %s @ line %d: %s\n", file, line, gluErrorString(glErr));
		retCode = 1;
		glErr = glGetError();
	}
	return retCode;
}

void printShaderInfoLog(GLuint obj)
{
	int infologLength = 0;
	int charsWritten = 0;
	char* infoLog;

	glGetShaderiv(obj, GL_INFO_LOG_LENGTH, &infologLength);

	if (infologLength > 0)
	{
		infoLog = (char*)malloc(infologLength);
		glGetShaderInfoLog(obj, infologLength, &charsWritten, infoLog);
		printf("%s\n", infoLog);
		free(infoLog);
	}
}

void printProgramInfoLog(GLuint obj)
{
	int infologLength = 0;
	int charsWritten = 0;
	char* infoLog;

	glGetProgramiv(obj, GL_INFO_LOG_LENGTH, &infologLength);

	if (infologLength > 0)
	{
		infoLog = (char*)malloc(infologLength);
		glGetProgramInfoLog(obj, infologLength, &charsWritten, infoLog);
		printf("%s\n", infoLog);
		free(infoLog);
	}
}
