#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

varying vec2 texCoord;

uniform vec2 screenLocation;

void main()
{
	gl_Position = vec4(aPos.xy + screenLocation, aPos.z, 1);
	//gl_Position = vec4(aPos.xy, aPos.z, 1);
	texCoord = vec2(aTexCoord.x, aTexCoord.y);
}