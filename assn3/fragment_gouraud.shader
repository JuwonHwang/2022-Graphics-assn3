#version 330

out vec4 FragColor;
in vec4 LightingColor;
uniform vec4 ourColor; 

void main()
{
    FragColor = LightingColor * ourColor;
}