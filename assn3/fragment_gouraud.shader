#version 330

out vec4 FragColor;
in vec4 color;
uniform vec4 ourColor; 

void main()
{
    FragColor = color * ourColor;
}