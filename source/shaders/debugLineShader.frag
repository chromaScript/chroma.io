#version 460 core
out vec4 fragColor;

uniform vec4 lineColor;

void main()
{
	fragColor = vec4(lineColor);
}