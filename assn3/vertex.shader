#version 120

in vec3 vPosition;
uniform mat4 MVP;

void main(void)
{
	gl_Position = MVP * vec4(vPosition, 1.0f);
}

