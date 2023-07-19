#version 460 core
out vec4 fragColor;

in vec2 texCoord;

uniform sampler2D texture1;
uniform vec3 backgroundColor;
uniform float backgroundAlpha;
uniform bool useBackgroundColor;
uniform vec3 fillColor;
uniform float fillAlpha;
uniform bool useFillColor;

uniform int useOverflow;
uniform vec4 overflowBox;

float insideBox2D(vec2 v, vec2 bottomLeft, vec2 topRight) {
    vec2 s = step(bottomLeft, v) - step(topRight, v);
    return s.x * s.y;   
}

void main()
{
	vec4 col = vec4(0);
	if (useBackgroundColor == true)
	{
		if (useFillColor == true)
		{
			col = vec4(mix(backgroundColor, fillColor, fillAlpha), backgroundAlpha);
		}
		else
		{
			col = vec4(backgroundColor, backgroundAlpha);
		}
	}
	else
	{
		vec4 sampled = texture(texture1, texCoord);
		if (useFillColor == true)
		{
			col = vec4(mix(vec3(sampled.r, sampled.g, sampled.b), fillColor, fillAlpha), sampled.a);
		}
		else
		{
			col = sampled;
		}
	}
	float t = insideBox2D(gl_FragCoord.xy, vec2(overflowBox.x, overflowBox.y), vec2(overflowBox.z, overflowBox.w));
	fragColor = t * col + (1 - t) * (col * useOverflow);
}