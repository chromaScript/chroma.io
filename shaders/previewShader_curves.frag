#version 460 core
out vec4 FragColor;

in vec2 fTexelCoord;

in vec2 control_points[4];
in vec2 startPos;
in vec2 startHandle;
in vec2 endPos;
in vec2 endHandle;
in vec2 fUV;

uniform float sizeLine;
uniform vec3 colorCurve;

vec4 encodeFloatRGBA(float v)
{
    vec4 kEncodeMul = vec4(1.0, 255.0, 65025.0, 160581375.0);
    float kEncodeBit = 1.0/255.0;
    vec4 enc = kEncodeMul * v;
    enc = fract(enc);
    enc -= enc.yzww * kEncodeBit;
    return enc;
}

float binomialCoefficient(int n, int k)
{
    // Using the multiplicative formula
    // en.wikipedia.org/wiki/Binomial_coefficient#Multiplicative_formula
    float c = 1.;
    for(int i = 1; i <= k; ++i)
        c *= float(n + 1 - i) / float(i);
    return c;
}

// en.wikipedia.org/wiki/B%C3%A9zier_curve
vec2 explicitBezier(in vec2 control_points[4], float t)
{
    vec2 sum = vec2(0);
    for(int i = 0; i <= 3; ++i)
    {
        sum += float(binomialCoefficient(3, i)) *
            	pow(1. - t, float(3 - i)) * pow(t, float(i)) * control_points[i];
    }

    return sum;
}

// Exact BBox to a cubic bezier
// From IQ: shadertoy.com/view/XdVBWd
vec4 bboxBezier(in vec2 p0, in vec2 p1, in vec2 p2, in vec2 p3 )
{
    // extremes
    vec2 mi = min(p0,p3);
    vec2 ma = max(p0,p3);

    vec2 k0 = -1.0*p0 + 1.0*p1;
    vec2 k1 =  1.0*p0 - 2.0*p1 + 1.0*p2;
    vec2 k2 = -1.0*p0 + 3.0*p1 - 3.0*p2 + 1.0*p3;

    vec2 h = k1*k1 - k0*k2;

    if( h.x>0.0 )
    {
        h.x = sqrt(h.x);
        //float t = (-k1.x - h.x)/k2.x;
        float t = k0.x/(-k1.x-h.x);
        if( t>0.0 && t<1.0 )
        {
            float s = 1.0-t;
            float q = s*s*s*p0.x + 3.0*s*s*t*p1.x + 3.0*s*t*t*p2.x + t*t*t*p3.x;
            mi.x = min(mi.x,q);
            ma.x = max(ma.x,q);
        }
        //t = (-k1.x + h.x)/k2.x;
        t = k0.x/(-k1.x+h.x);
        if( t>0.0 && t<1.0 )
        {
            float s = 1.0-t;
            float q = s*s*s*p0.x + 3.0*s*s*t*p1.x + 3.0*s*t*t*p2.x + t*t*t*p3.x;
            mi.x = min(mi.x,q);
            ma.x = max(ma.x,q);
        }
    }

    if( h.y>0.0)
    {
        h.y = sqrt(h.y);
        //float t = (-k1.y - h.y)/k2.y;
        float t = k0.y/(-k1.y-h.y);
        if( t>0.0 && t<1.0 )
        {
            float s = 1.0-t;
            float q = s*s*s*p0.y + 3.0*s*s*t*p1.y + 3.0*s*t*t*p2.y + t*t*t*p3.y;
            mi.y = min(mi.y,q);
            ma.y = max(ma.y,q);
        }
        //t = (-k1.y + h.y)/k2.y;
        t = k0.y/(-k1.y+h.y);
        if( t>0.0 && t<1.0 )
        {
            float s = 1.0-t;
            float q = s*s*s*p0.y + 3.0*s*s*t*p1.y + 3.0*s*t*t*p2.y + t*t*t*p3.y;
            mi.y = min(mi.y,q);
            ma.y = max(ma.y,q);
        }
    }
   
    return vec4( mi, ma );
}

bool computeTransformation(in vec2 control_points[4], out vec2 translation, out mat2 rotation)
{
    // Set p0 to zero, to simplify computation
    vec2 p1 = control_points[1] - control_points[0];
    vec2 p2 = control_points[2] - control_points[0];
    vec2 p3 = control_points[3] - control_points[0];
    
    // Polynomial coefficients of Bezier curve B(t)
    vec2 a = 3. * (p1 - p2) + p3;
    vec2 c = 3. * p1;
    vec2 b = 3. * p2 - 2. * c;
   
    // Expected value, E[B] = mean of points along Bezier curve
    vec2 mu = a / 4. + b / 3. + c / 2.;

    // Polynomial coefficients of Bx(t)By(t)
    float axy = a.x * a.y;
    float bxy = a.x * b.y + b.x * a.y;
    float cxy = a.x * c.y + b.x * b.y + c.x * a.y;
    float dxy = b.x * c.y + c.x * b.y;
    float exy = c.x * c.y;
    
    // Polynomial coefficients of Bx(t)Bx(t) and By(t)By(t)
    vec2 aB = a * a;
    vec2 bB = a * b * 2.;
    vec2 cB = a * c * 2. + b * b;
    vec2 dB = b * c * 2.;
    vec2 eB = c * c;
    
    // Definite integrals of polynomials p(1) - p(0)
    float xy =  axy / 7. +  bxy / 6. +  cxy / 5. +  dxy / 4. +  exy / 3.; // E[BxBy]
    
    float xx = aB.x / 7. + bB.x / 6. + cB.x / 5. + dB.x / 4. + eB.x / 3.; // E[BxBx]
    float yy = aB.y / 7. + bB.y / 6. + cB.y / 5. + dB.y / 4. + eB.y / 3.; // E[ByBy]

    // Covariance matrix
    mat2 covariance;
    covariance[1][0] = covariance[0][1] = xy - mu.x * mu.y;
    covariance[0][0] = xx - mu.x * mu.x;
    covariance[1][1] = yy - mu.y * mu.y;
    
    // Characteristic polynomial of covariance matrix
    float eigen_a = 1.;
    float eigen_b = -(covariance[0][0] + covariance[1][1]);
    float eigen_c = covariance[0][0] * covariance[1][1] - covariance[1][0] * covariance[0][1];
 
    float discr = eigen_b * eigen_b - 4. * eigen_a * eigen_c;
    
    if(discr > 0.)
    {
        float sqrt_discr = sqrt(discr);
        
        // Eigenvalues of covariance matrix
        float lambda0 = (-eigen_b - sqrt_discr) / (2. * eigen_a);
        float lambda1 = (-eigen_b + sqrt_discr) / (2. * eigen_a);

        // Eigenvectors of covariance matrix
        vec2 eigenvector0 = vec2(covariance[1][0], lambda0 - covariance[0][0]);
        vec2 eigenvector1 = vec2(covariance[1][0], lambda1 - covariance[0][0]);

        rotation[0] = normalize(eigenvector0);
        rotation[1] = normalize(eigenvector1);        

        translation = mu + control_points[0];
        return true;
    }
    
    return false;
}

bool getCubicBezierOBB(in vec2 control_points[4], out vec2 box_points[4])
{
    vec2 translation;
    mat2 rotation;
    
    bool res = computeTransformation(control_points, translation, rotation);
    
    if(!res)
        return false;
        
    // Transform the Bezier control points into the local space of the OBB
    vec2 control_points_xfrm[4];
    for(int i = 0; i <= 3; ++i)
    {
        control_points_xfrm[i] = control_points[i] - translation;
        control_points_xfrm[i] = transpose(rotation) * control_points_xfrm[i];
    }
    
    // Get axis-aligned bounding box in local space
    vec4 b2_xfrm = bboxBezier(control_points_xfrm[0], control_points_xfrm[1],
                              control_points_xfrm[2], control_points_xfrm[3]);
    
    // Transform AABB back to worldspace
    box_points[0] = b2_xfrm.xy;
    box_points[1] = vec2(b2_xfrm.x, b2_xfrm.w);
    box_points[2] = b2_xfrm.zw;
    box_points[3] = vec2(b2_xfrm.z, b2_xfrm.y);
    
    for(int i = 0; i < 4; ++i)
        box_points[i] = rotation * box_points[i] + translation;
    
    return true;
}


// Distance to line segment
float segment(vec2 p, vec2 a, vec2 b)
{
    return distance(p, mix(a, b, clamp(dot(p - a, b - a) / dot(b - a, b - a), 0., 1.)));
}

float sdBox( in vec2 p, in vec2 b ) 
{
    vec2 q = abs(p) - b;
    vec2 m = vec2( min(q.x,q.y), max(q.x,q.y) );
    return (m.x > 0.0) ? length(q) : m.y; 
}

void main()
{
    vec2 uv = fUV;
    vec2 obb_points[4];
    
    // Get the Bezier OBB
    getCubicBezierOBB(control_points, obb_points);

    
    
    const int m = 48;
    
    float dist = 1e4;
    
    vec2 prevp = control_points[0];
    //vec2 p = explicitBezier(control_points, 0.1);
    //dist = min(dist, segment(uv, prevp, p));
    //prevp = control_points[3];
    //p = explicitBezier(control_points, 0.9);
    //dist = min(dist, segment(uv, prevp, p));
    for(int i = 1; i <= m; ++i)
    {
        float t = float(i) / float(m);
        vec2 p;

        p = (i < m) ? explicitBezier(control_points, t) : control_points[3];
        dist = min(dist, segment(uv, prevp, p));

        prevp = p;
    }
        
    // Draw control edges
    //for(int i = 0; i < 3; ++i)
    //{
    //    col = mix(vec4(.25, .25, .25, 1), col, smoothstep(.002, .004,
    //            segment(uv, control_points[i], control_points[i + 1])));
    //}

    vec4 col = vec4(0.0);
    // Draw Bezier curve
	col = mix(vec4(colorCurve, 1.0), vec4(colorCurve, 0.0), smoothstep(sizeLine, sizeLine, dist));
    
    // Draw control points
    //for(int i = 0; i <= 3; ++i)
       //col = mix(vec4(1), col, smoothstep(.02, .023, distance(uv, control_points[i])));
    
    // Draw OBB
    //col = mix(vec4(1,0,1,1), col, smoothstep(sizeLine, sizeLine, segment(uv, obb_points[0], obb_points[1])));
    //col = mix(vec4(1,0,1,1), col, smoothstep(sizeLine, sizeLine, segment(uv, obb_points[1], obb_points[2])));
    //col = mix(vec4(1,0,1,1), col, smoothstep(sizeLine, sizeLine, segment(uv, obb_points[2], obb_points[3])));
    //col = mix(vec4(1,0,1,1), col, smoothstep(sizeLine, sizeLine, segment(uv, obb_points[3], obb_points[0])));

    FragColor = col;
}