#pragma once
#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/vec3.hpp>

#include "textReader.h"

GLuint v_shader;
GLuint f_shader;
GLuint program_shader;
bool gouraud = true;
void setShaders();

GLuint MatrixID;
GLuint vertexPosition_modelspaceID;

//GLuint v_shader, f_shader, program_shader
void setShaders()
{
	char* vs = NULL, * fs = NULL, * fs2 = NULL;

	v_shader = glCreateShader(GL_VERTEX_SHADER);
	f_shader = glCreateShader(GL_FRAGMENT_SHADER);

	if (gouraud) {
		vs = textFileRead("vertex_gouraud.shader");
		fs = textFileRead("fragment_gouraud.shader");
	}
	else {
		vs = textFileRead("vertex_phong.shader");
		fs = textFileRead("fragment_phong.shader");
	}
	//vs = textFileRead("vertex.shader");
	//fs = textFileRead("fragment.shader");

	const char* vv = vs;
	const char* ff = fs;

	glShaderSource(v_shader, 1, &vv, NULL);
	glShaderSource(f_shader, 1, &ff, NULL);

	free(vs); free(fs);

	glCompileShader(v_shader);
	glCompileShader(f_shader);

	program_shader = glCreateProgram();
	glAttachShader(program_shader, v_shader);
	glAttachShader(program_shader, f_shader);

	glLinkProgram(program_shader);

	glUseProgram(program_shader);
}