#version 460 core
out vec4 fragColor;

varying vec2 texCoord;

uniform sampler2D texture1;
uniform vec4 textColor;
uniform vec4 highlightColor;
uniform float textAlpha;
uniform bool renderText;
uniform bool renderSelection;

void main()
{
	if (renderText)
	{
		vec4 sampled = vec4(1.0, 1.0, 1.0, texture(texture1, texCoord).r);
		fragColor = vec4(textColor.xyz, textAlpha) * sampled;
	}
	else
	{
		if (renderSelection)
		{
			fragColor = highlightColor;
			fragColor.a *= 0.5;
		}
		else
		{
			fragColor = highlightColor;
		}
	}
}