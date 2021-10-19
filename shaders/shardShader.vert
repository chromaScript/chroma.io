#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 texCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec2 posVec0;
uniform vec2 posVec1;
uniform vec2 posVec2;
uniform vec2 posVec3;
uniform vec2 scaleXY;

void main()
{
	vec3 modPos = aPos;
	switch (gl_VertexID)
	{
		case 0: modPos.xy += posVec0; modPos.xy += vec2(scaleXY.y, scaleXY.x); break;
		case 1: modPos.xy += posVec1; modPos.xy += vec2(scaleXY.y, -scaleXY.x); break;
		case 2: modPos.xy += posVec2; modPos.xy += vec2(-scaleXY.y, -scaleXY.x); break;
		case 3: modPos.xy += posVec3; modPos.xy += vec2(-scaleXY.y, scaleXY.x); break;
	}
	gl_Position = projection * view * model * vec4(modPos, 1.0f);
	texCoord = vec2(aTexCoord.x, aTexCoord.y);
}