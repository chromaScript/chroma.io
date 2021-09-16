#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aTex;

out vec4 vertColor;
out vec2 texCoord;

uniform vec2 screenLocation;

void main()
{
	gl_Position = vec4(aPos.xy + screenLocation, aPos.z, 1);
	texCoord = vec2(aTex.x, aTex.y);
	vertColor = aColor;
}