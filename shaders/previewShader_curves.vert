#version 460 core
layout (location = 0) in vec2 aStartPos;
layout (location = 1) in vec2 aStartHandle;
layout (location = 2) in vec2 aEndHandle;
layout (location = 3) in vec2 aEndPos;
layout (location = 4) in vec2 aBoundsP1;
layout (location = 5) in vec2 aBoundsP2;
layout (location = 6) in vec2 aBoundsP3;
layout (location = 7) in vec2 aBoundsP4;

out vec2 texelCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out VS_OUT {
    vec2 startPos;
    vec2 startHandle;
    vec2 endPos;
    vec2 endHandle;
    vec2 boundsP1;
    vec2 boundsP2;
    vec2 boundsP3;
    vec2 boundsP4;
    vec2 texelCoord;
} vs_out;

vec4 encodeFloatRGBA(float v)
{
	vec4 kEncodeMul = vec4(1.0, 255.0, 65025.0, 160581375.0);
	float kEncodeBit = 1.0 / 255.0;
	vec4 enc = kEncodeMul * v;
	enc = fract(enc);
	enc -= enc.yzww * kEncodeBit;
	return enc;
}

void main()
{
    // Calculate the bounding box
    vs_out.boundsP1 = (projection * view * model * vec4(aBoundsP1, 0.0, 1.0)).xy;
    vs_out.boundsP2 = (projection * view * model * vec4(aBoundsP2, 0.0, 1.0)).xy;
    vs_out.boundsP3 = (projection * view * model * vec4(aBoundsP3, 0.0, 1.0)).xy;
    vs_out.boundsP4 = (projection * view * model * vec4(aBoundsP4, 0.0, 1.0)).xy;

    // Set remaining variables
    vs_out.startPos = (projection * view * model * vec4(aStartPos.x, aStartPos.y, 0.0, 1.0)).xy;
    vs_out.startHandle = (projection * view * model * vec4(aStartHandle.x, aStartHandle.y, 0.0, 1.0)).xy;
    vs_out.endPos = (projection * view * model * vec4(aEndPos.x, aEndPos.y, 0.0, 1.0)).xy;
    vs_out.endHandle = (projection * view * model * vec4(aEndHandle.x, aEndHandle.y, 0.0, 1.0)).xy;
    //vs_out.startPos = aStartPos;
    //vs_out.startHandle = aStartHandle;
    //vs_out.endPos = aEndPos;
    //vs_out.endHandle = aEndHandle;
    gl_Position = projection * view * model * vec4(aStartPos.x, aStartPos.y, 0.0, 1.0);
    vs_out.texelCoord = vec2(aStartPos.x, aStartPos.y);
}