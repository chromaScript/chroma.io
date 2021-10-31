#version 460 core
layout (points) in;
layout (triangle_strip) out;
layout (max_vertices = 4) out;

in VS_OUT {
    vec2 startPos;
    vec2 startHandle;
    vec2 endPos;
    vec2 endHandle;
    vec2 boundsP1;
    vec2 boundsP2;
    vec2 boundsP3;
    vec2 boundsP4;
    vec2 texelCoord;
} gs_in[];

out vec2 control_points[4];
out vec2 fUV;
out vec2 fTexelCoord;

void build_quad()
{
    float fSize = 0.005;
    vec2 loc = vec2(0.0);

    // 1:bottom-left
    fUV = gs_in[0].boundsP1;
    gl_Position = vec4(gs_in[0].boundsP1, 0.0, 1.0);
    EmitVertex();   

    // 2:bottom-right
    fUV = gs_in[0].boundsP2;
    gl_Position = vec4(gs_in[0].boundsP2, 0.0, 1.0);
    EmitVertex();

    // 3:top-left
    fUV = gs_in[0].boundsP3;
    gl_Position = vec4(gs_in[0].boundsP3, 0.0, 1.0);
    EmitVertex();

    // 4:top-right
    fUV = gs_in[0].boundsP4;
    gl_Position = vec4(gs_in[0].boundsP4, 0.0, 1.0);
    EmitVertex();
    EndPrimitive();
}

void main() 
{
    control_points[0] = gs_in[0].startPos;
    control_points[1] = gs_in[0].startHandle;
    control_points[2] = gs_in[0].endHandle;
    control_points[3] = gs_in[0].endPos;
    build_quad();
    fTexelCoord = gs_in[0].texelCoord;
}