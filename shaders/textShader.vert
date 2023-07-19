#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 texCoord;

uniform bool renderText;

void main()
{
	gl_Position = vec4(aPos.xy, 0, 1);
	texCoord = vec2(aTexCoord.x, aTexCoord.y);
}