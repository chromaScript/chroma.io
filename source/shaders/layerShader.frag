#version 460 core
out vec4 fragColor;

varying vec2 texCoord;

uniform sampler2D texture1;

void main()
{
	fragColor = texture(texture1, texCoord);
	//fragColor = vec4(0.6, 0.5, 0.8, 1.0);
}