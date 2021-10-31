#version 460 core
layout (location = 0) in vec3 aPos1;
layout (location = 1) in vec3 aPos2;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec4 aColor;
layout (location = 4) in float aSize;
layout (location = 5) in float aShape;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out VS_OUT {
    vec4 pos2;
    vec4 color;
    vec2 uv;
    float size;
    float shape;
} vs_out;

void main()
{
    int shapeSelect = int(aShape);
    switch (shapeSelect) {
        case 1:
            vs_out.pos2 = vec4(aPos2, 1.0);
        break;
        case 6:
            vs_out.pos2 = projection * view * model * vec4(aPos2.x, aPos2.y, 0.0, 1.0);
        break;
    }
    vs_out.uv = aTexCoord;
    vs_out.color = aColor;
    vs_out.size = aSize;
    vs_out.shape = aShape;
    gl_Position = projection * view * model * vec4(aPos1.x, aPos1.y, 0.0, 1.0);
}