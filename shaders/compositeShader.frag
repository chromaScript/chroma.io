#version 460 core
out vec4 fragColor;

in vec2 texelCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;

uniform float strokeOpacity;

// Global Variables
vec4 color;
vec4 fxColor;
vec4 grey;
vec2 pointOnAxis;
const float eps = 0.0000001;

// FX Generic Variables
uniform vec4 fx_gen_area;
uniform float fx_totalBlend;
uniform float fx_totalMask_alphaCenter;
uniform float fx_totalMask_alphaRange;
uniform int fx_count;

uniform vec3 fx_avgDir;
uniform vec3 fx_dirPointA;
uniform vec3 fx_dirPointB;
uniform float fx_isPortrait;

uniform int fx_slots[16];

// FX Fill
uniform bool fx_doFill_byCenter;
uniform bool fx_doFill_byAngle;

uniform float fx_fill_XMask;
uniform float fx_fill_XMask_invert;
uniform float fx_fill_YMask;
uniform float fx_fill_YMask_invert;

uniform float fx_fill_XMaskVariance;
uniform float fx_fill_YMaskVariance;
uniform float fx_fill_XChunkSize;
uniform float fx_fill_YChunkSize;

uniform float fx_fill_angle;

uniform vec2 fx_fill_center;
uniform float fx_fill_center_invert;
uniform float fx_fill_combineCenter;

uniform vec4 fx_fillColor;
uniform int fx_fillChannelMask; // 0 - Null // 1 - R // 2 - G // 3 - B // 4 - A // 5 - Value

// FX Gradient
uniform float fx_gradient_mixAmount;

// FX BrightContrast
uniform float fx_bright_mixAmount;
uniform float fx_bright_brightness;
uniform float fx_bright_brightMix;
uniform float fx_bright_contrast;
uniform float fx_bright_contrastMix;

// FX HSV
uniform float fx_hsv_mixAmount;
uniform vec3 fx_hsv_color; // Where W value is Bool-DoColorize

// FX Posterize
uniform float fx_poster_mixAmount;
uniform float fx_poster_gamma;
uniform float fx_poster_levels;
uniform float fx_poster_levelModifier;

// FX Threshold
uniform float fx_threshold_level;
uniform vec3 fx_threshold_mixAmount;
uniform vec3 fx_threshold_lowColor;
uniform vec3 fx_threshold_upperColor;

// FX Invert
uniform float fx_invert_mixAmount;
uniform float fx_invert_red;
uniform float fx_invert_green;
uniform float fx_invert_blue;

// FX Power
uniform float fx_power_mixAmount;
uniform vec3 fx_power_gamma;

// FX Modulo
uniform float fx_modulo_mixAmount;
uniform float fx_modulo_valueA;
uniform float fx_modulo_valueB;
uniform float fx_modulo_useX;
uniform float fx_modulo_useY;
uniform float fx_modulo_useDir;
uniform vec3 fx_modulo_XChannels;
uniform vec3 fx_modulo_YChannels;
uniform vec3 fx_modulo_DirChannels;

// FX Blur
uniform float fx_blur_mixAmount;
uniform float fx_blur_quality;
uniform float fx_blur_direction;
uniform float fx_blur_radius;
uniform vec4 fx_blur_mixChannels;

// SHADER BODY

// USEFUL FUNCTIONS SAVER
// ((0.48) - (atan(64 * (x - 0.5)) / 3.14)) * 1.04 - Produces Cauchy survival function, 
// where 512 is the "tightness" and (x - 0.5) determines inflexion point - Useful for threshold with softness
// By changing 0.5 to 0.45 and multiplying by 1.04, the fall-off can be pushed outside 0 and 1, useful to prevent trailing falloff

vec2 createDirVec2D(vec2 pointA, vec2 pointB)
{
	return normalize(vec2(pointA.x - pointB.x, pointA.y - pointB.y));
}

float distancePointLine2D(vec2 point, vec4 line)
{
	float a = line.y - line.w;
	float b = line.z - line.x;
	float c = (line.x * line.w) - (line.z * line.y);
	float d = abs((a * point.x) + (b * point.y) + c) / sqrt((a * a) + (b * b));
	return d;
}

vec2 projectPointToLine2D(vec2 point, vec4 line)
{
	float d = distancePointLine2D(point, line);
	vec2 dir = vec2(createDirVec2D(vec2(line.x, line.y), vec2(line.z, line.w)));
	float angle = atan(dir.y, dir.x) + (3.1412 / 2.0);
	dir = vec2(cos(angle), sin(angle));
	return point + (dir * d);
}


vec3 RGB_toHSL(vec3 inColor)
{
    float minc = min(inColor.r, min(inColor.g, inColor.b));
    float maxc = max(inColor.r, max(inColor.g, inColor.b));
    vec3  mask = step(inColor.grr, inColor.rgb) * step(inColor.bbg, inColor.rgb);
    vec3 h = mask * (vec3(0.0, 2.0, 4.0) + (inColor.gbr - inColor.brg) / (maxc-minc + eps)) / 6.0;
    return vec3(fract(1.0 + h.x + h.y + h.z),              // H
                 (maxc - minc) / (1.0 - abs(minc + maxc - 1.0) + eps),  // S
                 (minc + maxc) * 0.5);                           // L
}

vec3 HSL_toRGB(vec3 inColor)
{
    vec3 rgb = clamp(abs(mod(inColor.x * 6.0 + vec3(0.0, 4.0, 2.0), 6.0) - 3.0) - 1.0, 0.0, 1.0);
    return inColor.z + inColor.y * (rgb - 0.5) * (1.0 - abs(2.0 * inColor.z - 1.0));
}


float greyscale(vec3 color)
{
	return clamp((color.r * 0.21) + (color.g * 0.72) + (color.b * 0.07), 0, 1);
}

vec3 hash3(vec2 point)
{
	vec3 result = vec3( 
		dot(point, vec2(127.1,311.7)), 
		dot(point, vec2(269.5,183.3)), 
		dot(point, vec2(419.2,371.9)));
	return fract(sin(result)*43758.5453);
}

vec2 hash22(vec2 p)
{
    p = vec2( dot(p,vec2(127.1,311.7)),
			  dot(p,vec2(269.5,183.3)));
    return -1.0 + 2.0 * fract(sin(p)*43758.5453123);
}

float rand(vec2 coordinate)
{
	return fract(sin(dot(coordinate, vec2(12.9898, 78.233))) * 43758.5453);
}

void fill_simple()
{
	float positionMaskAmount = 1.0;
	float centerMaskAmount = 1.0;
	if (fx_doFill_byCenter)
	{
		float maxXDist = (fx_gen_area.y - fx_gen_area.x) / 2.2;
		float maxYDist = (fx_gen_area.w - fx_gen_area.z) / 2.2;
		float dist = distance(texelCoord, fx_fill_center);
		centerMaskAmount = clamp((dist / (((1 - fx_isPortrait) * maxXDist) + ((fx_isPortrait) * maxYDist))), 0, 1);
		centerMaskAmount = (fx_fill_center_invert * (1 - centerMaskAmount)) + ((fx_fill_center_invert - 1) * centerMaskAmount);
	}
	
	float XMaskAmount = clamp((texelCoord.x / (fx_gen_area.y - fx_gen_area.x)) * (1 - 0), 0, 1);
	XMaskAmount = (rand(floor(texelCoord / fx_fill_XChunkSize)) * fx_fill_XMaskVariance) + (fx_fill_XMask_invert * (1 - XMaskAmount)) + ((fx_fill_XMask_invert - 1.0) * XMaskAmount);
	float YMaskAmount = clamp((texelCoord.y / (fx_gen_area.w - fx_gen_area.z)) * (1 - 0), 0, 1);
	YMaskAmount = (rand(floor(texelCoord / fx_fill_YChunkSize)) * fx_fill_YMaskVariance) + (fx_fill_YMask_invert * (1 - YMaskAmount)) + ((fx_fill_YMask_invert - 1.0) * YMaskAmount);
	positionMaskAmount = positionMaskAmount * (1 - (YMaskAmount * fx_fill_YMask)) * (1 - (XMaskAmount * fx_fill_XMask));
	positionMaskAmount = clamp(positionMaskAmount - (centerMaskAmount * fx_fill_combineCenter), 0, 1);

	// Calculate channel mask value
	float channelMaskAmount = 1.0f;
	if (fx_fillChannelMask == 0) { channelMaskAmount = 1.0f; }
	else if (fx_fillChannelMask == 1) { channelMaskAmount = fxColor.r; }
	else if (fx_fillChannelMask == 2) { channelMaskAmount = fxColor.g; }
	else if (fx_fillChannelMask == 3) { channelMaskAmount = fxColor.b; }
	else if (fx_fillChannelMask == 4) { channelMaskAmount = fxColor.a; }
	else if (fx_fillChannelMask == 5) { channelMaskAmount = (0.2126 * fxColor.r) + (0.7152 * fxColor.g) + (0.0722 * fxColor.b); }
	fxColor.rgb = mix(fxColor.rgb, fx_fillColor.rgb, fx_fillColor.a * channelMaskAmount * positionMaskAmount);
}

void fill_direction()
{
	float maskX = clamp(distance(pointOnAxis, fx_dirPointB.xy) / distance(fx_dirPointB, fx_dirPointA), 0.0, 1.0);
	fxColor.rgb = mix(fxColor.rgb, fx_fillColor.rgb, smoothstep(0.0, 1.0, maskX));
}

void color_ramp()
{

}

void gradient()
{
	vec4 gradientColor = texture(texture2, 
		vec2(clamp(texelCoord.x / (fx_gen_area.y - fx_gen_area.x), 0, 1), 0.5));

	fxColor.rgb = mix(fxColor.rgb, gradientColor.rgb, fx_gradient_mixAmount);
}

void textureSimple()
{

}

void textureComplex()
{

}

void toon()
{
	// Make an alternative filter called Cell-Shade which does this posterize function which effects
	// value only
	//float greyscale = max(fxColor.r, max(fxColor.g, fxColor.b));
	float greylevel = greyscale(fxColor.rgb);
	float lower = floor(greylevel * fx_poster_levels) / fx_poster_levels;
	float lowerDiff = abs(greylevel - lower);
	float upper = ceil(greylevel * fx_poster_levels) / fx_poster_levels;
	float upperDiff = abs(upper - greylevel);
	float level = lowerDiff <= upperDiff ? lower : upper;
	float adjustment = level / greylevel;
	fxColor.rgb *= adjustment;
}

void posterize()
{
	vec3 c = fxColor.rgb;
	float greylevel = greyscale(c);
	c = pow(c, vec3(fx_poster_gamma, fx_poster_gamma, fx_poster_gamma));
	c = c * (fx_poster_levels * fx_poster_levelModifier);
	c = floor(c);
	c = c / (fx_poster_levels * fx_poster_levelModifier);
	c = pow(c, vec3(1.0/fx_poster_gamma));
	fxColor.rgb = mix(fxColor.rgb, c, fx_poster_mixAmount);
}

void threshold()
{
	float greyLevel = greyscale(fxColor.rgb);
	fxColor.rgb = (greyLevel > fx_threshold_level) ? 
		mix(fxColor.rgb, fx_threshold_upperColor, fx_threshold_mixAmount.x * fx_threshold_mixAmount.y) : 
		mix(fxColor.rgb, fx_threshold_lowColor, fx_threshold_mixAmount.x * fx_threshold_mixAmount.z);
}

void invert()
{
	vec3 invertColor = vec3(
	(fx_invert_red * (1 - fxColor.r)) + (fxColor.r * (1 - fx_invert_red)), 
	(fx_invert_green * (1 - fxColor.g)) + (fxColor.g * (1 - fx_invert_green)), 
	(fx_invert_blue * (1 - fxColor.b)) + (fxColor.b * (1 - fx_invert_blue)));
	fxColor.rgb = mix(fxColor.rgb, invertColor, fx_invert_mixAmount);
}

void brightContrast()
{
	vec3 contrastColor = vec3(clamp(((fx_bright_contrast + 2.0) - 1.0) * (fxColor.r), 0, 1),
		clamp(((fx_bright_contrast + 2.0) - 1.0) * (fxColor.g), 0, 1),
		clamp(((fx_bright_contrast + 2.0) - 1.0) * (fxColor.b), 0, 1));
	fxColor.rgb = mix(fxColor.rgb, contrastColor, fx_bright_mixAmount * fx_bright_contrastMix);
	fxColor.rgb = mix(fxColor.rgb, fxColor.rgb += fx_bright_brightness, fx_bright_mixAmount *  fx_bright_brightMix);
}

void hsv()
{
	vec3 fxColorHSV = RGB_toHSL(fxColor.rgb);

	fxColorHSV.x = fxColorHSV.x + fx_hsv_color.x;
	if (fxColorHSV.x > 1.0) { fxColorHSV.x = 0.0 + (fxColorHSV.x - 1.0); }
	if (fxColorHSV.x < 0.0) { fxColorHSV.x = 1.0 - -fxColorHSV.x; }
	fxColorHSV.y = clamp(fxColorHSV.y + fx_hsv_color.y, 0.0, 1.0);
	fxColorHSV.z = clamp(fxColorHSV.z + fx_hsv_color.z, 0.0, 1.0);

	if (fxColorHSV.x > 1.0) { fxColorHSV.x = 0 + (fxColorHSV.x - 1); }
	if (fxColorHSV.x < 0.0) { fxColorHSV.x = 1 - (1 - (-1 * fxColorHSV.x)); }
	fxColor.rgb = mix(fxColor.rgb, HSL_toRGB(fxColorHSV), fx_hsv_mixAmount);
}

void colorize()
{

}

void voronoise()
{
	float blurValue = 4;
	vec2 uv = 0.5 - 0.5 * cos(blurValue * vec2(1.0, 0.5));
	uv = uv * uv * (3.0 - 2.0 * uv);
	uv = uv * uv * (3.0 - 2.0 * uv);
	uv = uv * uv * (3.0 - 2.0 * uv);

	float k = 1.0 + 63.0 * pow(1.0 - uv.y, 6.0);

	float size = 64;
    vec2 i = floor(texelCoord / size);
    vec2 f = fract(texelCoord / size);
    
	vec2 a = vec2(0.0, 0.0);
    for(int y = -2; y <= 2; y++)
	{
		for(int x =- 2; x <= 2; x++)
		{
			vec2 g = vec2(x, y);
			vec3 o = hash3(i + g) * vec3(uv.x, uv.x, 1.0);
			vec2 d = g - f + o.xy;
			float w = pow(1.0 - smoothstep(0.0, 1.414, length(d)), k);
			a += vec2(o.z * w, w);
		}
	}

	float gamma = a.x/a.y;
	fxColor.rgb = mix(fxColor.rgb, vec3(gamma, gamma, gamma), 1.0);

}

void perlin_noise()
{
	float scale = 64;
	vec2 pi = floor(texelCoord.xy / scale);
	vec2 pf = (texelCoord.xy / scale) - pi;
    
	vec2 w = pf * pf * (3.0 - 2.0 * pf);
    
	float noise = mix(mix(dot(hash22(pi + vec2(0.0, 0.0)), pf - vec2(0.0, 0.0)), 
                   dot(hash22(pi + vec2(1.0, 0.0)), pf - vec2(1.0, 0.0)), w.x), 
               mix(dot(hash22(pi + vec2(0.0, 1.0)), pf - vec2(0.0, 1.0)), 
                   dot(hash22(pi + vec2(1.0, 1.0)), pf - vec2(1.0, 1.0)), w.x),
               w.y);
	fxColor.rgb = mix(fxColor.rgb, vec3(noise), fxColor.r / 2);
}

void fractal_noise()
{

}

void fracture_noise()
{

}

void sine_noise()
{

}

void smooth_noise()
{

}

void grunge_noise()
{

}

void gaussianBlur()
{
	float Pi = 6.28318530718; // Pi*2
    // Note - This is a standard Gaussian type filter, but because the effects are single-pass, it
	// can only blur the alpha channel while using the correct output fxColor after applying effects.
	// In order to use a true gaussian effect while painting, a second-pass has to be used, and preferably
	// optimize by running the second pass only once, saving it to the brush-stroke composite texture.
		// Second-Pass effects that require multi-sampling should be placed in a new Tool Setting called
		// "Filter" which can apply multi-sample effects.
   
	vec2 useRadius = vec2((fx_blur_radius / 1), (fx_blur_radius / 1));

	vec4 blurColor = fxColor;
	// Blur calculations
    for(float d = 0.0; d < Pi; d+= Pi / fx_blur_direction)
    {
		for(float i = 1.0 / fx_blur_quality; i <= 1.0; i += 1.0 / fx_blur_quality)
        {
			vec4 blurSample = texelFetch(texture1, ivec2(texelCoord + vec2(cos(d), sin(d)) * useRadius * i), 0);
			blurColor += blurSample;
        }
    }

	blurColor /= fx_blur_quality * fx_blur_direction - 15.0;
	fxColor.r = mix(fxColor.r, blurColor.r, fx_blur_mixAmount * fx_blur_mixChannels.r);
	fxColor.g = mix(fxColor.g, blurColor.g, fx_blur_mixAmount * fx_blur_mixChannels.g);
	fxColor.b = mix(fxColor.b, blurColor.b, fx_blur_mixAmount * fx_blur_mixChannels.b);
	fxColor.a = mix(fxColor.a, blurColor.a, fx_blur_mixAmount * fx_blur_mixChannels.a);
	color = mix(color, fxColor, fx_blur_mixAmount * fx_blur_mixChannels.a);
	fxColor = color;
}

void hatching_lines()
{
	float luminosity = 1.1;
	vec3 col = pow(fxColor.rgb, vec3(luminosity));
	float width = 24;
	float c = mod(float(texelCoord.x + texelCoord.y), width);
	fxColor.rgb = clamp(col * width - c, vec3(0.0), vec3(1.0));
}

void clampHSV()
{

}

void clampRGB()
{
	
}

void power()
{
	vec3 powCol = pow(fxColor.rgb, fx_power_gamma);
	fxColor.rgb = mix(fxColor.rgb, powCol, fx_power_mixAmount);
}

void modulo()
{
	vec2 point = vec2(mod(texelCoord.x / fx_modulo_valueA, 2.0), mod(texelCoord.y / fx_modulo_valueA, 2.0));
	point.x = mod(point.x * fx_modulo_valueB, 10.0) / fx_modulo_valueB;
	point.y = mod(point.y * fx_modulo_valueB, 10.0) / fx_modulo_valueB;
	// Calculate by X Distance
	fxColor.r = mix(fxColor.r, point.x, fx_modulo_XChannels.r * fx_modulo_useX * fx_modulo_mixAmount);
	fxColor.g = mix(fxColor.g, point.x, fx_modulo_XChannels.g * fx_modulo_useX * fx_modulo_mixAmount);
	fxColor.b = mix(fxColor.b, point.x, fx_modulo_XChannels.b * fx_modulo_useX * fx_modulo_mixAmount);
	// Calculate by Y Distance
	fxColor.r = mix(fxColor.r, point.y, fx_modulo_YChannels.r * fx_modulo_useY * fx_modulo_mixAmount);
	fxColor.g = mix(fxColor.g, point.y, fx_modulo_YChannels.g * fx_modulo_useY * fx_modulo_mixAmount);
	fxColor.b = mix(fxColor.b, point.y, fx_modulo_YChannels.b * fx_modulo_useY * fx_modulo_mixAmount);
	// Calculate by direction
	float maskX = clamp(distance(pointOnAxis, fx_dirPointB.xy) / distance(fx_dirPointB, fx_dirPointA), 0.0, 1.0) * 1000.0;
	maskX = mod(maskX / fx_modulo_valueA, 2.0);
	fxColor.r = mix(fxColor.r, mod(maskX * fx_modulo_valueB, 10.0) / fx_modulo_valueB, fx_modulo_DirChannels.r * fx_modulo_useDir * fx_modulo_mixAmount);
	fxColor.g = mix(fxColor.g, mod(maskX * fx_modulo_valueB, 10.0) / fx_modulo_valueB, fx_modulo_DirChannels.g * fx_modulo_useDir * fx_modulo_mixAmount);
	fxColor.b = mix(fxColor.b, mod(maskX * fx_modulo_valueB, 10.0) / fx_modulo_valueB, fx_modulo_DirChannels.b * fx_modulo_useDir * fx_modulo_mixAmount);
}

void pixellate()
{

}

void effectSwitch(int switchID)
{
	switch (switchID)
	{
		case 0: return; break;
		case 81:
			fill_simple(); break;
		case 82:
			gradient(); break;
		case 83:
			posterize(); break;
		case 84:
			invert(); break;
		case 85:
			threshold(); break;
		case 86:
			brightContrast(); break;
		case 87:
			hsv(); break;
		case 90:
			power(); break;
		case 91:
			modulo(); break;
		case 95:
			gaussianBlur(); break;
	}
}

void main()
{
	color = texelFetch(texture1, ivec2(texelCoord), 0);
	color.a *= strokeOpacity;

	pointOnAxis = projectPointToLine2D(texelCoord.xy, vec4(fx_dirPointA.xy, fx_dirPointB.xy));

	fxColor = color;

	for (int i = 0; i < fx_count; i++)
	{
		effectSwitch(fx_slots[i]);
	}

	// Final Output
	// Apply alpha masking
	fragColor = mix(color, fxColor, fx_totalBlend * smoothstep(
		fx_totalMask_alphaCenter - (fx_totalMask_alphaRange / 2), 
		fx_totalMask_alphaCenter + (fx_totalMask_alphaRange / 2), 
		color.a));
}

/*

case 88:
			clampHSV(); break;
		case 89:
			clampRGB(); break;
		
		
		case 92:
			toon(); break;
		case 93:
			color_ramp(); break;
		case 94:
			fill_direction(); break;
		case 95:
			gaussianBlur(); break;
		case 96:
			colorize(); break;
		case 97:
			textureSimple(); break;
		case 98:
			textureComplex(); break;
		case 110:
			voronoise(); break;
		case 111:
			perlin_noise(); break;
		case 112:
			fractal_noise(); break;
		case 113:
			fracture_noise(); break;
		case 114:
			sine_noise(); break;
		case 115:
			smooth_noise(); break;
		case 115:
			grunge_noise(); break;
		case 120:
			hatching_lines(); break;
		case 121:
			pixellate(); break;

*/