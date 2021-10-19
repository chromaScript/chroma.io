#version 460 core
out vec4 fragColor;

in vec2 texCoord;
in vec2 screenCoord;

uniform sampler2D texture1;

uniform vec4 rgba;

// Flow/Opacity testing variables
uniform float flow;
uniform float opacity;
uniform vec2 flipXY;

void main()
{
	vec2 flipCoord = texCoord;
	flipCoord.x = (flipXY.x < 0.0) ? 1.0 - flipCoord.x : flipCoord.x;
	flipCoord.y = (flipXY.y < 0.0) ? 1.0 - flipCoord.y : flipCoord.y;
	vec4 brush = texture(texture1, flipCoord);
	brush.a *= (flow * opacity);
	brush.r = rgba.r;
	brush.g = rgba.g;
	brush.b = rgba.b;
	fragColor = brush;
}