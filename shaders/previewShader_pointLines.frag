#version 460 core
out vec4 FragColor;

in vec2 fUV;
in vec4 fColor;

void main()
{  
    FragColor = fColor;
}