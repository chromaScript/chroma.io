#version 460 core
out vec4 fragColor;

varying vec2 texCoord;
varying vec2 screenCoord;

uniform sampler2D texture1;

uniform vec4 rgba;

// Flow/Opacity testing variables
uniform float flow;

void main()
{
	vec4 brush = texture(texture1, texCoord);
	brush.a *= flow;
	fragColor = brush;
}