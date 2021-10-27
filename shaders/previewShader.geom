#version 460 core
layout (points) in;
layout (triangle_strip) out;
layout (max_vertices = 4) out;

in VS_OUT {
    vec4 pos2;
    vec4 color;
    vec2 uv;
    float size;
    float shape;
} gs_in[];

out vec4 fColor;
out vec2 fUV;

void build_quad(vec4 position)
{
    fColor = gs_in[0].color;
    float fSize = gs_in[0].size;
    vec2 loc = vec2(0.0);
    float rotation = -atan(gs_in[0].pos2.y, gs_in[0].pos2.x);

    // 1:bottom-left
    fUV = vec2(0.0, 0.0);
    loc = vec2(-fSize, -fSize);
    gl_Position = position + vec4(vec2(dot(loc, vec2(sin(rotation), cos(rotation))), 
		dot(loc, vec2(cos(rotation), -sin(rotation)))), 0.0, 0.0);
    EmitVertex();   

    // 2:bottom-right
    fUV = vec2(1.0, 0.0);
    loc = vec2(-fSize, fSize);
    gl_Position = position + vec4(vec2(dot(loc, vec2(sin(rotation), cos(rotation))), 
		dot(loc, vec2(cos(rotation), -sin(rotation)))), 0.0, 0.0);
    EmitVertex();

    // 3:top-left
    fUV = vec2(0.0, 1.0);
    loc = vec2(fSize, -fSize);
    gl_Position = position + vec4(vec2(dot(loc, vec2(sin(rotation), cos(rotation))), 
		dot(loc, vec2(cos(rotation), -sin(rotation)))), 0.0, 0.0);
    EmitVertex();

    // 4:top-right
    fUV = vec2(1.0, 1.0);
    loc = vec2(fSize, fSize);
    gl_Position = position + vec4(vec2(dot(loc, vec2(sin(rotation), cos(rotation))), 
		dot(loc, vec2(cos(rotation), -sin(rotation)))), 0.0, 0.0);
    EmitVertex();
    EndPrimitive();
}

void build_line(float norm, vec3 pos1, vec3 pos2)
{
    fColor = gs_in[0].color;
    float fSize = gs_in[0].size;
    vec2 loc = vec2(0.0);
    vec3 lineDir = normalize(pos2 - pos1);
    lineDir.z = 0.0;
    float lineAngle = -atan(lineDir.y, lineDir.x);
    float lineNormal = lineAngle + (3.14159265 / 2.0);

    // Points A/B From Pos1
    fUV = vec2(0.0, 1.0);
    loc = vec2(0.0, fSize);
    gl_Position = vec4(pos1 + vec3(vec2(dot(loc, vec2(sin(lineNormal), cos(lineNormal))), 
		dot(loc, vec2(cos(lineNormal), -sin(lineNormal)))), 0.0), norm);
    EmitVertex();

    fUV = vec2(0.0, 0.0);
    loc = vec2(0.0, -fSize);
    gl_Position = vec4(pos1 + vec3(vec2(dot(loc, vec2(sin(lineNormal), cos(lineNormal))), 
		dot(loc, vec2(cos(lineNormal), -sin(lineNormal)))), 0.0), norm);
    EmitVertex();

    // Points C/D From Pos2
    fUV = vec2(1.0, 1.0);
    loc = vec2(0.0, fSize);
    gl_Position = vec4(pos2 + vec3(vec2(dot(loc, vec2(sin(lineNormal), cos(lineNormal))), 
		dot(loc, vec2(cos(lineNormal), -sin(lineNormal)))), 0.0), norm);
    EmitVertex();

    fUV = vec2(1.0, 0.0);
    loc = vec2(0.0, -fSize);
    gl_Position = vec4(pos2 + vec3(vec2(dot(loc, vec2(sin(lineNormal), cos(lineNormal))), 
		dot(loc, vec2(cos(lineNormal), -sin(lineNormal)))), 0.0), norm);
    EmitVertex();

    EndPrimitive();
}

void main() 
{
    int shapeSelect = int(gs_in[0].shape);

    switch (shapeSelect) {
        case 1:
            build_quad(gl_in[0].gl_Position);
        break;
        case 6:
            build_line(gl_in[0].gl_Position.w, gl_in[0].gl_Position.xyz, gs_in[0].pos2.xyz);
        break;
    }
}