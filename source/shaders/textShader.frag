#version 460 core
out vec4 fragColor;

varying vec2 texCoord;

uniform sampler2D texture1;
uniform vec3 textColor;
uniform float textAlpha;

void main()
{
	vec4 sampled = vec4(1.0, 1.0, 1.0, texture(texture1, texCoord).r);
	fragColor = vec4(textColor, textAlpha) * sampled;
	//fragColor = vec4(0.0, 0.4, 0.0, texture(texture1, texCoord).r);
}