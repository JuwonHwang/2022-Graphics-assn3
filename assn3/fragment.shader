#version 120

out vec4 FragColor;

uniform vec4 ourColor; // OpenGL �ڵ忡�� �� ������ ������ ���Դϴ�.

void main()
{
    FragColor = ourColor;
}