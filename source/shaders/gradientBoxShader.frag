#version 460 core
out vec4 fragColor;

in vec4 vertColor;
in vec2 texCoord;

uniform sampler2D texture1;
uniform vec3 backgroundColor;
uniform float backgroundAlpha;
uniform bool useBackgroundColor;
uniform vec3 fillColor;
uniform float fillAlpha;
uniform bool useFillColor;

void main()
{
	if (useBackgroundColor == true)
	{
		if (useFillColor == true)
		{
			fragColor = vec4(mix(backgroundColor, fillColor, fillAlpha), backgroundAlpha);
		}
		else
		{
			fragColor = vec4(vertColor);
		}
	}
	else
	{
		vec4 sampled = texture(texture1, texCoord);
		if (useFillColor == true)
		{
			fragColor = vec4(mix(vec3(sampled.r, sampled.g, sampled.b), fillColor, fillAlpha), sampled.a);
		}
		else
		{
			fragColor = sampled;
		}
	}
}