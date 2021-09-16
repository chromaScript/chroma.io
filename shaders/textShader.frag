#version 460 core
out vec4 fragColor;

varying vec2 texCoord;

uniform sampler2D texture1;
uniform vec4 textColor;
uniform vec4 highlightColor;
uniform float textAlpha;
uniform bool renderText;
uniform bool renderSelection;

uniform int useOverflow;
uniform vec4 overflowBox;

float insideBox2D(vec2 v, vec2 bottomLeft, vec2 topRight) {
    vec2 s = step(bottomLeft, v) - step(topRight, v);
    return s.x * s.y;   
}

void main()
{
	vec4 col = vec4(0);

	if (renderText)
	{
		vec4 sampled = vec4(1.0, 1.0, 1.0, texture(texture1, texCoord).r);
		col = vec4(textColor.xyz, textAlpha) * sampled;
	}
	else
	{
		if (renderSelection)
		{
			col = highlightColor;
			col.a *= 0.5;
		}
		else
		{
			col = highlightColor;
		}
	}

	float t = insideBox2D(gl_FragCoord.xy, vec2(overflowBox.x, overflowBox.y), vec2(overflowBox.z, overflowBox.w));

	fragColor = t * col + (1 - t) * (col * useOverflow);
}