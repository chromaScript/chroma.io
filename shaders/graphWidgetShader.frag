// Bezier shader partially sourced from shadertoy.com/view/NlfGDX by maras (2021-06-08);

#version 460 core
out vec4 fragColor;

in vec2 texCoord;

uniform sampler2D texture1;
uniform vec3 backgroundColor;
uniform float backgroundAlpha;

uniform vec2 curveStart;
uniform vec2 curveEnd;

uniform int pointsCount;
uniform float curvePoints[32];

uniform int useOverflow;
uniform vec4 overflowBox;

vec2 grid_fract(vec2 x, vec2 i) {
    return (fract(x / i + 0.5) - 0.5) * i;
}

vec3 color_dist_mix(vec3 bg, vec3 fg, float dist, float alpha) {
    float d = smoothstep(0.0, 0.75, dist); 
    return mix(bg, fg, alpha * (1.0-d));
}

float distanceToLineSegment(vec2 point, vec2 start, vec2 end) {
    float A = point.x - start.x;
    float B = point.y - start.y;
    float C = end.x - start.x;
    float D = end.y - start.y;

    float dot1 = A * C + B * D;
    float len_sq = C * C + D * D;
    float param = -1.0;
    if (len_sq != 0.0) {  param = dot1 / len_sq; }
       
    float xx, yy;

    if (param < 0.0) {
        xx = start.x;
        yy = start.y;
    }

    else if (param > 1.0) {
        xx = end.x;
        yy = end.y;
    }

    else {
        xx = start.x + param * C;
        yy = start.y + param * D;
    }

    float dx = point.x - xx;
    float dy = point.y - yy;
    return sqrt(dx * dx + dy * dy);
}

float insideBox2D(vec2 v, vec2 bottomLeft, vec2 topRight) {
    vec2 s = step(bottomLeft, v) - step(topRight, v);
    return s.x * s.y;   
}

void main()
{
	vec4 col = vec4(backgroundColor, backgroundAlpha);
    vec4 curveColor = vec4(1.0, 0.0, 1.0, 1.0);
    vec4 lineColor = vec4(0.0, 0.0, 0.0, 0.5);
    vec4 gridColor = vec4(1.0, 1.0, 1.0, 0.15);

    float edge = 0.0;
    float smoothing = 0.0;

    // Draw the Grid lines
    vec2 grid = abs(grid_fract(texCoord, vec2(0.125))) / (4.0 / 120.0);
    float d_grid = min(grid.x, grid.y);

    col.rgb = mix(col.rgb, gridColor.rgb, gridColor.a * (1.0 - smoothstep(0.0, 0.2, d_grid)));

    grid = abs(grid_fract(texCoord, vec2(0.5))) / (4.0 / 120.0);
    d_grid = min(grid.x, grid.y);

    col.rgb = mix(col.rgb, gridColor.rgb, (gridColor.a * 2.0) * (1.0 - smoothstep(0.0, 0.2, d_grid)));

    edge = 0.007;
    smoothing = 0.008;

    // Draw connecting lines between points
    for (int i = 0; i < pointsCount; i++)
    {
        float dist = distanceToLineSegment(
            texCoord, 
            vec2(curvePoints[(i * 2)], curvePoints[(i * 2) + 1]), 
            vec2(curvePoints[((i * 2) + 2)], curvePoints[((i * 2) + 2) + 1]));
        if (dist < edge + smoothing)
        {
            dist = max(dist, 0.0);
            dist = smoothstep(edge - smoothing, edge + smoothing, dist);
            col =  mix(lineColor, col, dist);
        }
    }

    edge = 0.012;
    smoothing = 0.0120;

    // Draw the Bezier curve
    float end_algorithm = 1.001;
    float drawing_steps = 160.0;
    float increment = end_algorithm / drawing_steps;
                                
    vec2 prev_point = vec2(vec2(curvePoints[(0 * 2)], curvePoints[(0 * 2) + 1]));
    
    if (pointsCount > 1)
    {
        for (float i = 0.0; i <= end_algorithm; i += increment) 
        {
            float h = 1.0;
            float u = (1.0 - i);
            vec2 Q = vec2(vec2(curvePoints[(0 * 2)], curvePoints[(0 * 2) + 1]));
            
            if (i != 0.0) {
                if (i <= 0.5) {
                    u = i / u;
                    for (int k = 1; k <= pointsCount; ++k) {
                        float prev_h = h;
                        h = h * u * (float(pointsCount + 1) - float(k));
                        h = h / (float(k) + h);
                        float h1 = (h / prev_h) * ((float(k) * (1.0 - i)) / (i * float(pointsCount - k + 1)));
                        Q = h1 * Q + h * vec2(vec2(curvePoints[(k * 2)], curvePoints[(k * 2) + 1]));
                    }
                }
                else {
                    u = u / i;
                    for (int k = 1; k <= pointsCount; ++k) {
                        float prev_h = h;
                        h = h * (float(pointsCount + 1) - float(k));
                        h = h / (float(k) * u + h);
                        float h1 = (h / prev_h) * ((float(k) * (1.0 - i)) / (i * float(pointsCount - k + 1)));
                        Q = h1 * Q + h * vec2(vec2(curvePoints[(k * 2)], curvePoints[(k * 2) + 1]));
                    }
                }
            }
            
            vec2 point = Q;
            
            float dist = distanceToLineSegment(texCoord, prev_point, point) * 2.0;
            
            if (dist < edge + smoothing)
            {
                dist = max(dist, 0.0);
                dist = smoothstep(edge - smoothing, edge + smoothing, dist);
                col =  mix(curveColor, col, dist);
            }
            
            prev_point = point;
        }
    }
    else
    {
        float dist = distanceToLineSegment(texCoord, vec2(curvePoints[0], curvePoints[1]), vec2(curvePoints[2], curvePoints[3])) * 2.0;
         
        if (dist < edge + smoothing)
        {
            dist = max(dist, 0.0);
            dist = smoothstep(edge - smoothing, edge + smoothing, dist);
            col =  mix(curveColor, col, dist);
        }
    }

	float t = insideBox2D(gl_FragCoord.xy, vec2(overflowBox.x, overflowBox.y), vec2(overflowBox.z, overflowBox.w));
	fragColor = t * col + (1 - t) * (col * useOverflow);
}