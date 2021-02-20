#version 460 core
out vec4 fragColor;

in vec2 texCoord;

uniform sampler2D texture1;

void main()
{
	fragColor = texture(texture1, texCoord);
	// fragColor = vec4(0.5, 0.2, 0.1, 1.0);
}