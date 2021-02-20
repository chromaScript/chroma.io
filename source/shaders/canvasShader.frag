#version 460 core
out vec4 fragColor;

varying vec2 texCoord;

uniform sampler2D texture1;
uniform int checkerSize;
uniform float camZoom;

void main()
{
	int xVal, yVal;
	float factor = 80.0 / (camZoom / 10000.0);
	xVal = (int(texCoord.x * factor) / checkerSize) % 2;
	yVal = (int(texCoord.y * factor) / checkerSize) % 2;
	vec4 color;
	if (xVal == yVal)
	{
		color = vec4(0.65f, 0.65f, 0.65f, 1.0f);
	}
	else
	{
		color = vec4(0.85f, 0.85f, 0.85f, 1.0f);
	}
	fragColor = color;
}