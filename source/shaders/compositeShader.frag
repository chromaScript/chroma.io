#version 460 core
out vec4 fragColor;

in vec2 texelCoord;

uniform sampler2D texture1;
uniform float strokeOpacity;

void main()
{
	vec4 color = texelFetch(texture1, ivec2(texelCoord), 0);
	color.a *= strokeOpacity;
	fragColor = color;
}
