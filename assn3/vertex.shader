#version 120

in vec3 vPosition;
uniform mat4 MV;
uniform mat4 P;

void main(void)
{
	gl_Position = P * MV * vec4(vPosition, 1.0f);
}

