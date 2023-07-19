#version 460 core
out vec4 fragColor;

in vec2 texCoord;

uniform sampler2D texture1;

uniform bool drawBoxShadow;
uniform vec4 boxShadowColor;
uniform float boxShadowAlpha;

uniform bool drawNoise;
uniform int scaleCanvas;
uniform int noiseMode;
uniform float seedA;
uniform float widgetZoom;
uniform vec2 scale;
uniform float skew;
uniform float factorA;
uniform float factorB;
uniform float factorC;
uniform bool rotateNoise;
uniform float rotation;
uniform float brightness;
uniform float contrast;
uniform float rangeMin;
uniform float rangeMax;
uniform float rangeSmooth;
uniform float blackLevel;
uniform float whiteLevel;
uniform int invert;
uniform int flipX;
uniform int flipY;
uniform bool lockPivot;

uniform int useOverflow;
uniform vec4 overflowBox;

vec4 noiseFinal = vec4(1.0);

vec3 hash3(vec2 point)
{
	vec3 result = vec3( 
		dot(point, vec2(127.1, 311.7)), 
		dot(point, vec2(269.5, 183.3)), 
		dot(point, vec2(419.2, 371.9)));
	return fract(sin(result) * 43758.5453);
}

vec2 hash2(vec2 point)
{
    vec2 result = vec2( dot(point,vec2(127.1,311.7)),
			  dot(point,vec2(269.5,183.3)));
    return -1.0 + 2.0 * fract(sin(result)*43758.5453123);
}

float hash1(float point)
{
	float result = dot(vec2(point, point), vec2(127.1, 311.7));
	return -1.0 + 2.0 * fract(sin(result) * 43758.5453123);
}

float rand(vec2 coordinate)
{
	return fract(sin(dot(coordinate, vec2(12.9898, 78.233))) * 43758.5453);
}

vec2 rand2(vec2 coordinate) 
{
    return fract(sin(vec2(dot(coordinate, vec2(127.1,311.7)), dot(coordinate, vec2(269.5,183.3)))) * 43758.5453);
}

float brightContrast(float inputVal, float inputBright, float inputContrast)
{
	return (inputVal - 0.5) * (inputContrast + 1.0) + 0.5 + inputBright;
}

float insideBox2D(vec2 v, vec2 bottomLeft, vec2 topRight) {
    vec2 s = step(bottomLeft, v) - step(topRight, v);
    return s.x * s.y;   
}

float perlin2D(vec2 location, float varA, float varB, float varC)
{
	// Run Perlin Noise Function from location
	vec2 pi = floor(location);
	vec2 pf = location - pi;
    
	vec2 w = pf * pf * (3.0 - 2.0 * pf);
    
	float noise = mix(mix(dot(hash2(pi + vec2(0.0, 0.0)), pf - vec2(0.0, 0.0)), 
                   dot(hash2(pi + vec2(1.0, 0.0)), pf - vec2(1.0, 0.0)), w.x), 
               mix(dot(hash2(pi + vec2(0.0, 1.0)), pf - vec2(0.0, 1.0)), 
                   dot(hash2(pi + vec2(1.0, 1.0)), pf - vec2(1.0, 1.0)), w.x),
				  w.y);
	return noise + (0.5 * varA);
}

float simplex_noise2D(vec2 point, float varA, float varB, float varC)
{
    // Simplex 2D by Inigo Quilez
    const float K1 = 0.366025404; // (sqrt(3)-1)/2;
    const float K2 = 0.211324865; // (3-sqrt(3))/6;

	vec2  i = floor(point + (point.x + point.y) * K1);
    vec2  a = point - i + (i.x + i.y) * K2;
    float m = step(a.y, a.x); 
    vec2  o = vec2(m, 1.0 - m);
    vec2  b = a - o + K2;
	vec2  c = a - 1.0 + (pow(varB, 0.4) * 2.0) * K2;
    vec3  h = max(0.5 - vec3(dot(a, a), dot(b, b), dot(c, c)), 0.0);
	vec3  n = h * h * h * h * vec3(dot(a, hash2(i + 0.0)), dot(b, hash2(i + o)), dot(c, hash2(i + 1.0)));
    float noise = dot(n, vec3(70.0 * ((10.0 - varC) / 10.0), 70.0 * ((10.0 - varC) / 10.0), 70.0));
    return 0.5 + ((0.5 * varA) * noise);
}

float random_noise(vec2 location, float varA, float varB, float varC)
{
	vec2 ipos = floor(location);
	vec2 fpos = pow(fract(location), vec2(sqrt(varA)));
	vec2 upos = fpos * fpos * (3.0 - 2.0 * fpos);

	// Assign a random value based on the integer coord
	float r1 = rand(ipos + vec2(0.0, 0.0));
	float r2 = rand(ipos + vec2(1.0, 0.0));
	float r3 = rand(ipos + vec2(0.0, 1.0));
	float r4 = rand(ipos + vec2(1.0, 1.0));

	float a = pow(factorB, 2.0);
	return mix(mix(r1, r2, upos.x / a),
		mix(r3, r4, upos.x / a), upos.y / a);
}

float voronoi(vec2 location, float varA, float varB, float varC)
{
	vec2 pi = floor(location);
	vec2 pf = location - pi;

	
	float m_dist = (0.0 + ((11.0 - varA) / 10.0) * (1.0 - 0.0));  // minimum distance
	vec2 m_point = vec2(0, 0);        // minimum point

	for (int j = -1; j <= 1; j++) {
		for (int i = -1; i <= 1; i++) {
			vec2 neighbor = vec2(float(i), float(j));
			vec2 point = hash2(pi + neighbor);
			point = 0.5 + 0.5 * sin(varB + 6.2831 * point);
			vec2 diff = neighbor + point - pf;
			float dist = length(diff);

			if (dist < m_dist) {
				m_dist = dist;
				m_point = point;
			}
		}
	}

	// Assign a color using the closest point position
	return dot(m_point, vec2(.3, .6));
}

float vorotwist(vec2 location, float varA, float varB, float varC)
{
	vec2 loc = location;
    loc += vec2(cos(varA), sin(varA)) * perlin2D(loc.yx, 1.0, 1.0, 1.0);
    loc -= vec2(sin(varA), cos(varA)) * perlin2D(loc.xy, 1.0, 1.0, 1.0);
    
    float rotation = mix(-3.14, 3.14, fract(varB / 20.0));
    float uR = dot(loc, vec2(cos(rotation), -sin(rotation)));
    float vR = dot(loc, vec2(sin(rotation), cos(rotation)));
    
    //loc = mix(loc, vec2(uR, vR), perlin_noise(loc * sqrt(varA)));
    loc = mix(loc, vec2(uR, vR), perlin2D(loc * sqrt(varA), 1.0, 1.0, 1.0));
    
    vec2 pi = floor(loc);
	vec2 pf = loc - pi;

	
	float m_dist = (0.0 + ((11.0 - varA) / 10.0) * (1.0 - 0.0));  // minimum distance
	vec2 m_point = vec2(0, 0);        // minimum point

	for (int j = -1; j <= 1; j++) {
		for (int i = -1; i <= 1; i++) {
			vec2 neighbor = vec2(float(i), float(j));
			vec2 point = hash2(pi + neighbor);
			point = 0.5 + 0.5 * sin((varB / 20.0) + 6.2831 * point);
			vec2 diff = neighbor + point - pf;
			float dist = length(diff);

			if (dist < m_dist) {
				m_dist = dist;
				m_point = point;
			}
		}
	}

	// Assign a color using the closest point position 
	return dot(m_point, vec2(.3, .6));;
}

float voroblur(vec2 location, float varA, float varB, float varC)
{
	vec2 pi = floor(location);
	vec2 pf = location - pi;

	float k = 1.000;
    
    float va = 0.0;
    float wt = 0.0;
	for (int j =- 2; j <= 2; j++) {
        for (int i =- 2; i <= 2; i++) {
            vec2 g = vec2(float(i), float(j));
            vec3 o = hash3(pi + g);
            vec2 r = g - pf + o.xy;
            float d = dot(r, r);
            float ww = pow(1.0 - smoothstep(clamp((varA / 10.0), 0.0, varB / 5.0), varC / 5.0, sqrt(d)), k);
            va += o.z * ww;
            wt += ww;
        }
    }
    
    return va / wt;
}

float truchetPattern(vec2 pos, int mode, float varA, float varB, float varC){
    
    // Based on Truchet - 10 print by @patriciogv - 2015
    float noise = perlin2D(pos / 2.5, 1.0, 1.0, 1.0);
    vec2 rpos = pos + vec2(noise, noise) * (varB);
    
    
    vec2 ipos = floor(rpos);  // integer
    vec2 fpos = fract(rpos);  // fraction
    float select = rand(ipos);
    
    vec2 tile = vec2(fpos);
    
    select = fract(((select - 0.5) * 2.0));
    if (select > 0.75) {
        tile = vec2(1.0) - tile;
    } else if (select > 0.5) {
        tile = vec2(1.0 - tile.x, tile.y);
    } else if (select > 0.25) {
        tile = 1.0 - vec2(1.0 - tile.x, tile.y);
    }
    float maze = 
        (mode == 0) ? // maze
        	smoothstep((tile.x * ((10.0 - varC) / 10.0)) - varA, tile.x, tile.y) -
            smoothstep(tile.x , tile.x + varA, tile.y) : 
    	(mode == 1) ? // triangles
            step(tile.x * varC + (varA / 10.0), tile.y - (varA / 10.0)) : 
    	(mode == 2) ? // round
            (step(length(tile), (0.500 + (varC * 0.04)) + (varA / 10.0)) -
    			step(length(tile), (0.500 - (varC * 0.04)) - (varA / 10.0)) ) +
    		(step(length(tile - vec2(1.0)), (0.500 + (varC * 0.04)) + (varA / 10.0)) -
    			step(length(tile - vec2(1.0)), (0.500 - (varC * 0.04)) - (varA / 10.0)) ) : 1.0;
    
    return maze;
}

float halftoneDot2D(vec2 pos, float varA, float varB, float varC)
{
	vec2 pi = floor(pos);
    vec2 pf = fract(pos);
    vec2 pf_c = abs(pf - 0.5) * 2.0;
    
    float randA = mix(0.0, varC / 10.0, rand(floor(pos + 0.5)));
    float randB = rand(floor((pos) + 12.5));
    vec2 dotSize = vec2((varA / 10.0) - 0.1, (varA / 10.0) + 0.1);
    
    float dist = smoothstep(dotSize.x, dotSize.y, distance(pf_c, vec2(1.0)) / (1.0 - randA));
    
    return dist + (randB * (varB / 10.0));
}

float cellularA_2D(vec2 pos, float varA, float varB, float varC)
{
    // Based on 'CellularNoise' by @patriciogv
    vec2 pi = floor(pos);
    vec2 pf = fract(pos);
    
    float balanceA = ((10.0 - varA + 1.0) / 10.0);
    float balanceB = ((10.0 - varB + 1.0) / 10.0);

    float m_dist = balanceA;  // minimum distance

    for (int y= -1; y <= 1; y++) {
        for (int x= -1; x <= 1; x++) {
            // Neighbor place in the grid
            vec2 neighbor = vec2(float(x),float(y));

            // Random position from current + neighbor place in the grid
            vec2 point = rand2(pi + neighbor);

			// Vector between the pixel and the point
            //vec2 diff = neighbor + point - pf;
            vec2 diff = neighbor + point - pf;

            // Distance to the point
            float dist = length(diff) * varC;

            // Keep the closer distance
            m_dist = min(m_dist, dist);
        }
    }
    return (m_dist - (0.5 * balanceB)) + (0.5 * balanceA);
}

vec3 cellularB_2D(vec2 x, float varA, float varB, float varC) {
    
    // Created by inigo quilez - iq/2013
    vec2 n = floor(x);
    vec2 f = fract(x);

    // first pass: regular voronoi
    vec2 mg, mr;
    float md = 8.0;
    for (int j= -1; j <= 1; j++) {
        for (int i= -1; i <= 1; i++) {
            vec2 g = vec2(float(i), float(j));
            vec2 o = rand2(n + g);

            vec2 r = g + o - f;
            float d = dot(r, r);

            if( d<md ) {
                md = d;
                mr = r;
                mg = g;
    }}}

    // second pass: distance to borders
    md = 8.0;
    for (int j= -2; j <= 2; j++) {
        for (int i = -2; i <= 2; i++) {
            vec2 g = mg + vec2(float(i), float(j));
            vec2 o = rand2(n + g);

            vec2 r = g + o - f + (varC / 2.0);

            if (dot(mr - r, mr - r) > (varB / 2.5)) {
                md = min(md, dot(vec2(varA / 10.0) * (mr + r), normalize(r - mr)));
    }}}
    return vec3(md, mr);
}

////////////////////////////////////////////////////////////////
//
// Canvas Functions

float lines(float pos, float scale, float brightnessA)
{
    // Blank Space Between each line
    float contrastA = 0.0;

    float blank = mix(0.0, 1.0, abs(fract((pos) / scale) - 0.5) * 2.0);
    blank = clamp(blank, 0.0, 1.0);
    
    blank = clamp((blank - 0.5) * contrastA + 0.5 + brightnessA, 0.0, 1.0);
    blank = 1.0 - blank;
    
    float line = mix(0.0, 1.0, abs(fract((pos) / scale) - 0.5) * 2.0);
    line = clamp(line, 0.0, 1.0);
    
    return clamp(line - blank, 0.0, 1.0);;
}

float strips(vec2 pos, float scale, float brightnessA, float cutScale, float fcontrast, float width)
{
   // Crevices between strips
    float contrast = 2.4;
    float brightness = 0.9;
    
    // Right Side Crevice
    float cutLineA = pow(fract(pos.x / scale), cutScale);
   
    cutLineA = (cutLineA - 0.5) * contrast + 0.5 + brightness;
    cutLineA = clamp(cutLineA, 0.0, 1.0);
    // Left Side Crevice
    float cutLineB = pow(1.0 - fract(pos.x / scale), cutScale);
    
    cutLineB = (cutLineB - 0.5) * contrast + 0.5 + brightness;
    cutLineB = clamp(cutLineB, 0.0, 1.0);
    
    float cutLineFinal = smoothstep(0.0, 1.0, clamp((pow(pow(cutLineA, width) * pow(cutLineB, width), 2.8) * 2.4) + 0.1, 0.0, 1.0));
    
    // Smooth Strips
    float linePos = floor(pos.x / scale) * scale;
    float stripLine = lines(pos.y + linePos, (scale * 2.0), brightnessA);
    float r = (fract(floor((pos.y + (scale / 2.0)) / scale) / 2.0) <= 0.01) ? 1.0 : 0.0;
    
    stripLine = clamp(stripLine, 0.0, 1.0);
    
    
    return clamp(stripLine * ((pow(cutLineFinal, 0.25) - 0.5) * fcontrast + 0.5), 0.0, 1.0);
}

float canvasPockets(vec2 pos, float dotScalar)
{
    float radius = clamp(1.0 - pow(dotScalar + 0.1, 0.3), 0.35, 1.0) / 2.95;
    vec2 centered = mix(vec2(0.0), vec2(1.0), abs(fract(pos) - 0.5) * 2.0);
    float dist = distance(centered, vec2(1.0)) / (1.0 - radius);
    return clamp(pow((dist - 0.5) * 1.3 + 0.6, 0.12), 0.0, 0.5) + 0.5;
}

float canvas_typeA_2D(vec2 pos, float scale, float varA, float varB, float varC)
{
    vec2 uv = pos;
    float stripsScale_ = scale / 2.0;

    // Noise Values
    float perlinA = perlin2D(uv / (scale * 2.0), 1.0, 1.0, 1.0) * perlin2D(uv / (scale * 7.0), 1.0, 1.0, 1.0);
    float perlinB = perlin2D((uv * vec2(4.0, 1.0)) / (scale / 3.0), 1.0, 1.0, 1.0);
    float perlinC = perlin2D((uv * vec2(1.0, 4.5)) / (scale / 3.0), 1.0, 1.0, 1.0);
    float perlinD = perlin2D(uv.yx / (scale * 6.0), 1.0, 1.0, 1.0) * perlin2D(uv.xy / (scale * 16.0), 1.0, 1.0, 1.0);
    
    uv += vec2(perlinA, perlinA) * (scale / (varC));
    
    
    // Flood Fills
    float randomA = random_noise((uv + vec2(stripsScale_ / 2.0, stripsScale_ / 1.0)) / stripsScale_, 1.0, 1.0, 1.0);
    randomA = (randomA - 0.5) * 1.2 + ((varA / 10.0) * 2.0);
    
    float randomB = rand(floor((uv + vec2(0.0, stripsScale_ / 2.0)) / stripsScale_));
    randomB = (randomB - 0.5) * 0.4 + 0.1;
    
    float randomC = random_noise((uv + vec2(stripsScale_ / 1.0, stripsScale_ / 2.0)) / stripsScale_, 1.0, 1.0, 1.0);
    randomC = clamp((randomC - 0.5) * 2.1 - 0.1, 0.0, 1.0);
    
    float randomFlood = clamp(1.07 - (0.35 * (randomA + randomB)), 0.0, 1.0);
    
    // Canvas Pockets
    float pockets = canvasPockets((uv + vec2(stripsScale_ / 1.0, stripsScale_ / 2.0)) / stripsScale_, randomC);
    
    // Vertical Strips
    float strips_y = strips(uv, stripsScale_, 0.32, 1.0 / 1.9 * mix(1.2, 2.9, perlinA), 1.1 * mix(0.4, 1.1, perlinB), 0.7);
    float strips_yInterval = floor(fract(sin(floor(floor((uv.x + stripsScale_) / stripsScale_))) * 74613.412341) * clamp(varB * 6.0, 0.0, 60.0));
    float strips_yBoost = clamp(smoothstep(0.0, 1.0, pow(abs(fract(floor(uv.x / stripsScale_) / strips_yInterval) - 0.5) * 2.0, 0.1)) * (1.2 - perlinD), 0.0, 1.0);
    
    strips_y = clamp((strips_y - 0.5) * 2.3 + 0.5 + 0.1, 0.0, 1.0);
    strips_y = pow(strips_y, 1.0 / 3.0);
    
    vec2 weaveOffset = vec2(-(scale / 4.0), scale - (scale / 4.0));
    float strips_x = strips(uv.yx + weaveOffset, stripsScale_, 0.32, 1.0 / 2.1 * mix(0.9, 2.9, perlinA), 1.2 * mix(0.4, 1.2, perlinC), 1.7);
    float strips_xInterval = floor(fract(sin(floor(floor((uv.y + (stripsScale_ / 2.0)) / stripsScale_))) * 74613.412341) * clamp(varB * 8.0, 1.0, 60.0));
    float strips_xBoost = clamp(smoothstep(0.0, 1.0, pow(abs(fract(floor((uv.y + (stripsScale_ / 2.0)) / stripsScale_) / strips_xInterval) - 0.5) * 2.0, 0.1)) * (perlinD / 0.2), 0.0, 1.0);
    
    strips_x = clamp((strips_x - 0.5) * 2.3 + 0.5 + 0.1, 0.0, 1.0);
    strips_x = pow(strips_x, 1.0 / 2.0);

    // Output
    
    float final = randomFlood * (max(strips_x, strips_y) - (1.0 - pockets));
    final = mix((final - 0.5) * 0.9 + 0.55,  (final - 0.5) * 0.85 + 0.340, strips_xBoost);
    final = mix((final - 0.5) * 0.9 + 0.55, (final - 0.5) * 0.85 + 0.42, strips_yBoost);
    return clamp((final - 0.5) * 0.45 + 0.58, 0.0, 1.0);
}

//
//
////////////////////////////////////////////////////////////////



vec2 adjustLoc(vec2 inputCoord, vec2 scaleZoomed)
{
	float pivot = (lockPivot) ? 0.0 : 0.5;
	// Create the location point
	vec2 loc = inputCoord;
	// Flip the location if needed
	loc = vec2((flipX == 1) ? 1.0 - loc.x : loc.x, (flipY == 1) ? 1.0 - loc.y : loc.y);
	// Apply factor B (Stretching)
	loc = mix(loc, 
			vec2(
				dot(loc - pivot, vec2(sin(rotation), cos(rotation))), 
				dot(loc - pivot, vec2(cos(rotation), -sin(rotation)))) + pivot, 
			fract(skew / 20.0)) ;

	vec2 rotP = loc - pivot;
    
	float uR = dot(rotP, vec2(cos(rotation), -sin(rotation)));
    float vR = dot(rotP, vec2(sin(rotation), cos(rotation)));

	loc = mix(loc, vec2(uR, vR) + pivot, rotateNoise);

	return ((loc - pivot) / scaleZoomed) + pivot;
}

void main()
{
	vec4 col = vec4(0.0, 0.0, 0.0, 1.0);
	// Handling for rendering noise in the shader
	if (drawBoxShadow == true) {
		col = boxShadowColor;
	}
	else if (drawNoise == true) {
		float val = 1.0f;

        //vec2 scaleFinal = scale * (((1.0 - scaleCanvas) * 1.0) + (1 * scaleCanvas / 10.0));
        vec2 scaleFinal = (scale / 2.0) * widgetZoom;
		vec2 adjustedLoc = adjustLoc(texCoord, 0.02 * scaleFinal);

		switch (noiseMode)
		{
			case 0: val = perlin2D(adjustedLoc, factorA, factorB, factorC); break;
            case 1: val = simplex_noise2D(adjustedLoc, factorA, factorB, factorC); break;
			case 2: val = voronoi(adjustedLoc, factorA, factorB, factorC); break;
			case 3: val = random_noise(adjustedLoc, factorA, factorB, factorC); break;
			case 4: val = vorotwist(adjustedLoc, factorA, factorB, factorC); break;
			case 5: val = voroblur(adjustedLoc, factorA, factorB, factorC); break;
            case 6: val = canvas_typeA_2D(adjustedLoc, scaleFinal.x * scaleFinal.y, factorA, factorB, factorC); break;
		    case 7: val = cellularA_2D(adjustedLoc, factorA, factorB, factorC); break;
		    case 8: val = cellularB_2D(adjustedLoc, factorA, factorB, factorC).x; break;
		    case 9: val = halftoneDot2D(adjustedLoc, factorA, factorB, factorC); break;
		    case 10: val = truchetPattern(adjustedLoc, 0, factorA, factorB, factorC); break;
		    case 11: val = truchetPattern(adjustedLoc, 2, factorA, factorB, factorC); break;
		    case 12: val = truchetPattern(adjustedLoc, 1, factorA, factorB, factorC); break;
		}
		val = mix(val, smoothstep(rangeMin, rangeMax, val), 1.0 - rangeSmooth);
		val = clamp(val, rangeMin, rangeMax);
		val = brightContrast(val, brightness, contrast);
		val = (val < blackLevel) ? 0.0 : (val > whiteLevel) ? 1.0 : val;
		val = mix(val, 1 - val, invert);
		col.rgb = vec3(val);
	}
	// Handling for rendering a saved noise texture file
	else {
		col = vec4(0.0, 0.0, 0.0, 1.0);
	}

	float t = insideBox2D(gl_FragCoord.xy, vec2(overflowBox.x, overflowBox.y), vec2(overflowBox.z, overflowBox.w));
	fragColor = t * col + (1 - t) * (col * useOverflow);
}