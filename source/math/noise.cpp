#include "../include/math/noise.h"
#include "../include/math/math.h"
#include "../include/math/blend.h"

#include <glm.hpp>
#include <gtc/quaternion.hpp>
#include <vector>
#include <random>
#include <ctime>

float srandNormalized(unsigned int seed)
{
	static unsigned int lastSeed_srN = UINT32_MAX;
	if (seed != lastSeed_srN) { srand(seed); }
	return static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
}

glm::vec3 hash3(glm::vec3 point)
{
	glm::vec2 xy = glm::vec2(point.x, point.y);
	glm::vec2 yz = glm::vec2(point.y, point.z);
	glm::vec2 zx = glm::vec2(point.z, point.x);
	glm::vec3 result = glm::vec3(
		glm::dot(xy, glm::vec2(127.1f, 311.7f)),
		glm::dot(yz, glm::vec2(269.5f, 183.3f)),
		glm::dot(zx, glm::vec2(419.2f, 371.9f)));
	return fract(sin(result) * 43758.5453f);
}

glm::vec3 hash3(glm::vec2 point)
{
	glm::vec3 result = glm::vec3(
		dot(point, glm::vec2(127.1f, 311.7f)),
		dot(point, glm::vec2(269.5f, 183.3f)),
		dot(point, glm::vec2(419.2f, 371.9f)));
	return fract(sin(result) * 43758.5453f);
}


glm::vec2 hash2(glm::vec2 point)
{
	glm::vec2 result = glm::vec2(glm::dot(point, glm::vec2(127.1f, 311.7f)),
		glm::dot(point, glm::vec2(269.5f, 183.3f)));
	return -1.0f + 2.0f * fract(sin(result) * 43758.5453123f);
}

float hash1(float point)
{
	float result = glm::dot(glm::vec2(point), glm::vec2(127.1f, 311.7f));
	return -1.0f + 2.0f * fract(sin(result) * 43758.5453123f);
}

float rand1(glm::vec2 coordinate)
{
	return fract(sin(dot(coordinate, glm::vec2(12.9898f, 78.233f))) * 43758.5453f);
}
glm::vec2 rand2(glm::vec2 coordinate)
{
	return fract(sin(glm::vec2(dot(coordinate, glm::vec2(127.1f, 311.7f)), dot(coordinate, glm::vec2(269.5f, 183.3f)))) * 43758.5453f);
}

float perlin2D(glm::vec2 inPos, float factorA, float factorB, float factorC)
{
	glm::vec2 pi = floor(inPos);
	glm::vec2 pf = inPos - pi;

	glm::vec2 w = pf * pf * (3.0f - 2.0f * pf);

	float noise = mix(mix(glm::dot(hash2(pi + glm::vec2(0.0f, 0.0f)), pf - glm::vec2(0.0f, 0.0f)),
		glm::dot(hash2(pi + glm::vec2(1.0f, 0.0f)), pf - glm::vec2(1.0f, 0.0f)), w.x),
		mix(glm::dot(hash2(pi + glm::vec2(0.0f, 1.0f)), pf - glm::vec2(0.0f, 1.0f)),
			glm::dot(hash2(pi + glm::vec2(1.0f, 1.0f)), pf - glm::vec2(1.0f, 1.0f)), w.x),
		w.y);
	return noise + (0.5f * factorA);
}

float voroni2D(glm::vec2 inPos, float factorA, float factorB, float factorC)
{
	// Tile the space
	glm::vec2 pi = floor(inPos);
	glm::vec2 pf = inPos - pi;

	float m_dist = (0.0f + ((11.0f - factorA) / 10.0f) * (1.0f - 0.0f));  // minimum distance
	glm::vec2 m_point = glm::vec2(0.0f, 0.0f);        // minimum point

	float u_time = factorB;

	for (int j = -1; j <= 1; j++) {
		for (int i = -1; i <= 1; i++) {
			glm::vec2 neighbor = glm::vec2(float(i), float(j));
			glm::vec2 point = hash2(pi + neighbor);
			point = 0.5f + 0.5f * sin(u_time + 6.2831f * point);
			glm::vec2 diff = neighbor + point - pf;
			float dist = length(diff);

			if (dist < m_dist) {
				m_dist = dist;
				m_point = point;
			}
		}
	}

	// Assign a color using the closest point position
	return dot(m_point, glm::vec2(0.3f, 0.6f));
}

float vorotwist2D(glm::vec2 inPos, float factorA, float factorB, float factorC)
{
	glm::vec2 loc = inPos / 20.0f;
	loc += glm::vec2(cos(factorA), sin(factorA)) * perlin2D(glm::vec2(loc.x, loc.y), 1.0f, 1.0f, 1.0f);
	loc -= glm::vec2(sin(factorA), cos(factorA)) * perlin2D(glm::vec2(loc.y, loc.x), 1.0f, 1.0f, 1.0f);

	float rotation = mix(-MATH_RAD_180, MATH_RAD_180, fract((factorB / 20.0f)));
	float uR = dot(loc, glm::vec2(cos(rotation), -sin(rotation)));
	float vR = dot(loc, glm::vec2(sin(rotation), cos(rotation)));

	loc = mix(loc, glm::vec2(uR, vR), perlin2D(loc * sqrt(factorA), 1.0f, 1.0f, 1.0f));

	glm::vec2 pi = floor2(loc);
	glm::vec2 pf = loc - pi;


	float m_dist = (0.0f + ((11.0f - factorA) / 10.0f) * (1.0f - 0.0f));  // minimum distance
	glm::vec2 m_point = glm::vec2(0.0f, 0.0f);        // minimum point

	for (int j = -1; j <= 1; j++) {
		for (int i = -1; i <= 1; i++) {
			glm::vec2 neighbor = glm::vec2(float(i), float(j));
			glm::vec2 point = hash2(pi + neighbor);
			point = 0.5f + 0.5f * sin((factorB / 20.0f) + 6.2831f * point);
			glm::vec2 diff = neighbor + point - pf;
			float dist = length(diff);

			if (dist < m_dist) {
				m_dist = dist;
				m_point = point;
			}
		}
	}

	// Assign a color using the closest point position
	return dot(m_point, glm::vec2(0.3f, 0.6f));
}

float voroblur2D(glm::vec2 inPos, float factorA, float factorB, float factorC)
{
	glm::vec2 pi = floor2(inPos);
	glm::vec2 pf = inPos - pi;

	float k = 1.000f;

	float va = 0.0f;
	float wt = 0.0f;
	for (int j = -2; j <= 2; j++) {
		for (int i = -2; i <= 2; i++) {
			glm::vec2 g = glm::vec2(float(i), float(j));
			glm::vec3 o = hash3(pi + g);
			glm::vec2 r = g - (pf + glm::vec2(o.x, o.y));
 			float d = dot(r, r);
			float ww = pow(1.0f - smoothstep(clampf((factorA / 10.0f), 0.0f, factorB / 5.0f), factorC / 5.0f, sqrt(d)), k);
			va += o.z * ww;
			wt += ww;
		}
	}

	return va / wt;
}


float random2D(glm::vec2 inPos, float factorA, float factorB, float factorC)
{
	glm::vec2 ipos = floor2(inPos);
	glm::vec2 fpos = pow(fract(inPos), glm::vec2(sqrt(factorA))); // SQRT provides some additional blur option.
	glm::vec2 upos = fpos * fpos * (3.0f - 2.0f * fpos);

	// Assign a random value based on the integer coord
	float r1 = rand1(ipos  + glm::vec2(0.0f, 0.0f));
	float r2 = rand1(ipos + glm::vec2(1.0f, 0.0f));
	float r3 = rand1(ipos + glm::vec2(0.0f, 1.0f));
	float r4 = rand1(ipos + glm::vec2(1.0f, 1.0f));

	float a = pow(factorB, 2.0f);
	return mix(mix(r1, r2, upos.x / a),
		mix(r3, r4, upos.x / a), upos.y / a);
}

float truchetPattern(glm::vec2 pos, int mode, float varA, float varB, float varC)
{

	// Based on Truchet - 10 print by @patriciogv - 2015
	float noise = perlin2D(pos / 2.5f, 1.0f, 1.0f, 1.0f);
	glm::vec2 rpos = pos + glm::vec2(noise, noise) * (varB);


	glm::vec2 ipos = floor(rpos);  // integer
	glm::vec2 fpos = fract(rpos);  // fraction
	float select = rand1(ipos);

	glm::vec2 tile = glm::vec2(fpos);

	select = fract(((select - 0.5f) * 2.0f));
	if (select > 0.75f) {
		tile = glm::vec2(1.0f) - tile;
	}
	else if (select > 0.5f) {
		tile = glm::vec2(1.0f - tile.x, tile.y);
	}
	else if (select > 0.25f) {
		tile = 1.0f - glm::vec2(1.0f - tile.x, tile.y);
	}
	float maze =
		(mode == 0) ?
		smoothstep((tile.x * ((10.0f - varC) / 10.0f)) - varA, tile.x, tile.y) -
		smoothstep(tile.x, tile.x + varA, tile.y) :
		(mode == 1) ?
		step(tile.x * varC + (varA / 10.0f), tile.y - (varA / 10.0f)) :
		(mode == 2) ?
		(step(length(tile), (0.500f + (varC * 0.04f)) + (varA / 10.0f)) -
			step(length(tile), (0.500f - (varC * 0.04f)) - (varA / 10.0f))) +
		(step(length(tile - glm::vec2(1.0f)), (0.500f + (varC * 0.04f)) + (varA / 10.0f)) -
			step(length(tile - glm::vec2(1.0f)), (0.500f - (varC * 0.04f)) - (varA / 10.0f))) : 1.0f;

	return maze;
}

float halftoneDot2D(glm::vec2 pos, float varA, float varB, float varC)
{
	glm::vec2 pi = floor(pos);
	glm::vec2 pf = fract(pos);
	//glm::vec2 pf_c = abs(pf - 0.5f) * 2.0f;
	glm::vec2 pf_c = glm::vec2(abs(pf.x - 0.5f) * 2.0f, abs(pf.y - 0.5f) * 2.0f);

	float randA = mix(0.0f, varC / 10.0f, rand1(floor(pos + 0.5f)));
	float randB = rand1(floor(pos + 12.5f));
	glm::vec2 dotSize = glm::vec2((varA / 10.0f) - 0.1f, (varA / 10.0f) + 0.1f);

	float dist = smoothstep(dotSize.x, dotSize.y, glm::length(pf_c - glm::vec2(1.0f)) / (1.0f - randA));

	return dist + (randB * (varB / 10.0f));
}

float cellularA_2D(glm::vec2 pos, float varA, float varB, float varC)
{
	// Based on 'CellularNoise' by @patriciogv
	glm::vec2 pi = floor(pos);
	glm::vec2 pf = fract(pos);

	float balanceA = ((10.0f - varA + 1.0f) / 10.0f);
	float balanceB = ((10.0f - varB + 1.0f) / 10.0f);

	float m_dist = balanceA;  // minimum distance

	for (int y = -1; y <= 1; y++) {
		for (int x = -1; x <= 1; x++) {
			// Neighbor place in the grid
			glm::vec2 neighbor = glm::vec2(float(x), float(y));

			// Random position from current + neighbor place in the grid
			glm::vec2 point = rand2(pi + neighbor);

			// Vector between the pixel and the point
			//vec2 diff = neighbor + point - pf;
			glm::vec2 diff = neighbor + point - pf;

			// Distance to the point
			float dist = length(diff) * varC;

			// Keep the closer distance
			m_dist = fmin(m_dist, dist);
		}
	}
	return (m_dist - (0.5f * balanceB)) + (0.5f * balanceA);
}

glm::vec3 cellularB_2D(glm::vec2 x, float varA, float varB, float varC) {

	// Created by inigo quilez - iq/2013
	glm::vec2 n = floor(x);
	glm::vec2 f = fract(x);

	// first pass: regular voronoi
	glm::vec2 mg, mr;
	float md = 8.0;
	for (int j = -1; j <= 1; j++) {
		for (int i = -1; i <= 1; i++) {
			glm::vec2 g = glm::vec2(float(i), float(j));
			glm::vec2 o = rand2(n + g);

			glm::vec2 r = g + o - f;
			float d = glm::dot(r, r);

			if (d < md) {
				md = d;
				mr = r;
				mg = g;
			}
		}
	}

	// second pass: distance to borders
	md = 8.0;
	for (int j = -2; j <= 2; j++) {
		for (int i = -2; i <= 2; i++) {
			glm::vec2 g = mg + glm::vec2(float(i), float(j));
			glm::vec2 o = rand2(n + g);

			glm::vec2 r = g + o - f + (varC / 2.0f);

			if (dot(mr - r, mr - r) > (varB / 2.5f)) {
				md = fmin(md, dot(glm::vec2(varA / 10.0f) * (mr + r), normalize(r - mr)));
			}
		}
	}
	return glm::vec3(md, mr);
}

////////////////////////////////////////////////////////////////
//
// Canvas Functions

float lines(float pos, float scale, float brightnessA)
{
	// Blank Space Between each line
	float contrastA = 0.0f;

	float blank = mix(0.0, 1.0, abs(fract((pos) / scale) - 0.5f) * 2.0f);
	blank = clampf(blank, 0.0f, 1.0f);

	blank = clampf((blank - 0.5f) * contrastA + 0.5 + brightnessA, 0.0f, 1.0f);
	blank = 1.0f - blank;

	float line = mix(0.0f, 1.0f, abs(fract((pos) / scale) - 0.5f) * 2.0f);
	line = clampf(line, 0.0f, 1.0f);

	return clampf(line - blank, 0.0f, 1.0f);;
}

float strips(glm::vec2 pos, float scale, float brightnessA, float cutScale, float fcontrast, float width)
{
	// Crevices between strips
	float contrast = 2.4f;
	float brightness = 0.9f;

	// Right Side Crevice
	float cutLineA = pow(fract(pos.x / scale), cutScale);

	cutLineA = (cutLineA - 0.5f) * contrast + 0.5f + brightness;
	cutLineA = clampf(cutLineA, 0.0f, 1.0f);
	// Left Side Crevice
	float cutLineB = pow(1.0f - fract(pos.x / scale), cutScale);

	cutLineB = (cutLineB - 0.5f) * contrast + 0.5f + brightness;
	cutLineB = clampf(cutLineB, 0.0f, 1.0f);

	float cutLineFinal = smoothstep(0.0f, 1.0f, clampf((pow(pow(cutLineA, width) * pow(cutLineB, width), 2.8f) * 2.4f) + 0.1f, 0.0f, 1.0f));

	// Smooth Strips
	float linePos = floor(pos.x / scale) * scale;
	float stripLine = lines(pos.y + linePos, (scale * 2.0f), brightnessA);
	float r = (fract(floor((pos.y + (scale / 2.0f)) / scale) / 2.0f) <= 0.01f) ? 1.0f : 0.0f;

	stripLine = clampf(stripLine, 0.0f, 1.0f);


	return clampf(stripLine * ((pow(cutLineFinal, 0.25f) - 0.5f) * fcontrast + 0.5f), 0.0f, 1.0f);
}

float canvasPockets(glm::vec2 pos, float dotScalar)
{
	float radius = clampf(1.0f - pow(dotScalar + 0.1f, 0.3f), 0.35f, 1.0f) / 2.95f;
	glm::vec2 centered = mix(glm::vec2(0.0f), glm::vec2(1.0f), abs(fract(pos) - 0.5f) * 2.0f);
	float dist = glm::distance(centered, glm::vec2(1.0f)) / (1.0f - radius);
	return clampf(pow((dist - 0.5f) * 1.3f + 0.6f, 0.12f), 0.0f, 0.5f) + 0.5f;
}

float canvas_typeA_2D(glm::vec2 pos, float scale, float varA, float varB, float varC)
{
	glm::vec2 uv = pos;
	float stripsScale_ = scale / 2.0;

	// Noise Values
	float perlinA = perlin2D(uv / (scale * 2.0f), 1.0f, 1.0f, 1.0f) * perlin2D(uv / (scale * 7.0f), 1.0f, 1.0f, 1.0f);
	float perlinB = perlin2D((uv * glm::vec2(4.0, 1.0)) / (scale / 3.0f), 1.0f, 1.0f, 1.0f);
	float perlinC = perlin2D((uv * glm::vec2(1.0, 4.5)) / (scale / 3.0f), 1.0f, 1.0f, 1.0f);
	float perlinD = perlin2D(glm::vec2(uv.y, uv.x) / (scale * 6.0f), 1.0f, 1.0f, 1.0f) * perlin2D(glm::vec2(uv.y, uv.x) / (scale * 16.0f), 1.0f, 1.0f, 1.0f);

	uv += glm::vec2(perlinA, perlinA) * (scale / (varC));


	// Flood Fills
	float randomA = random2D((uv + glm::vec2(stripsScale_ / 2.0f, stripsScale_ / 1.0f)) / stripsScale_, 1.0f, 1.0f, 1.0f);
	randomA = (randomA - 0.5f) * 1.2f + ((varA / 10.0f) * 2.0f);

	float randomB = rand1(floor((uv + glm::vec2(0.0f, stripsScale_ / 2.0f)) / stripsScale_));
	randomB = (randomB - 0.5) * 0.4 + 0.1;

	float randomC = random2D((uv + glm::vec2(stripsScale_ / 1.0f, stripsScale_ / 2.0f)) / stripsScale_, 1.0f, 1.0f, 1.0f);
	randomC = clampf((randomC - 0.5f) * 2.1f - 0.1f, 0.0f, 1.0f);

	float randomFlood = clampf(1.07f - (0.35f * (randomA + randomB)), 0.0f, 1.0f);

	// Canvas Pockets
	float pockets = canvasPockets((uv + glm::vec2(stripsScale_ / 1.0f, stripsScale_ / 2.0f)) / stripsScale_, randomC);

	// Vertical Strips
	float strips_y = strips(uv, stripsScale_, 0.32f, 1.0f / 1.9f * mix(1.2f, 2.9f, perlinA), 1.1f * mix(0.4f, 1.1f, perlinB), 0.7f);
	float strips_yInterval = floor(fract(sin(floor(floor((uv.x + stripsScale_) / stripsScale_))) * 74613.412341f) * clampf(varB * 6.0f, 0.0f, 60.0f));
	float strips_yBoost = clampf(smoothstep(0.0f, 1.0f, pow(abs(fract(floor(uv.x / stripsScale_) / strips_yInterval) - 0.5f) * 2.0f, 0.1f)) * (1.2f - perlinD), 0.0f, 1.0f);

	strips_y = clampf((strips_y - 0.5f) * 2.3f + 0.5f + 0.1f, 0.0f, 1.0f);
	strips_y = pow(strips_y, 1.0 / 3.0);

	glm::vec2 weaveOffset = glm::vec2(-(scale / 4.0f), scale - (scale / 4.0f));
	float strips_x = strips(glm::vec2(uv.y, uv.x) + weaveOffset, stripsScale_, 0.32f, 1.0f / 2.1f * mix(0.9f, 2.9f, perlinA), 1.2f * mix(0.4f, 1.2f, perlinC), 1.7f);
	float strips_xInterval = floor(fract(sin(floor(floor((uv.y + (stripsScale_ / 2.0f)) / stripsScale_))) * 74613.412341f) * clampf(varB * 8.0f, 1.0f, 60.0f));
	float strips_xBoost = clampf(smoothstep(0.0f, 1.0f, pow(abs(fract(floor((uv.y + (stripsScale_ / 2.0f)) / stripsScale_) / strips_xInterval) - 0.5f) * 2.0f, 0.1f)) * (perlinD / 0.2f), 0.0f, 1.0f);

	strips_x = clampf((strips_x - 0.5f) * 2.3f + 0.5f + 0.1f, 0.0f, 1.0f);
	strips_x = pow(strips_x, 1.0f / 2.0f);

	// Output

	float final = randomFlood * (fmax(strips_x, strips_y) - (1.0f - pockets));
	final = mix((final - 0.5f) * 0.9f + 0.55f, (final - 0.5f) * 0.85f + 0.340f, strips_xBoost);
	final = mix((final - 0.5f) * 0.9f + 0.55f, (final - 0.5f) * 0.85f + 0.42f, strips_yBoost);
	return clampf((final - 0.5f) * 0.45f + 0.58f, 0.0f, 1.0f);
}

//
//
////////////////////////////////////////////////////////////////