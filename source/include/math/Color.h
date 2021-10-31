#ifndef COLOR_H
#define COLOR_H

#include "math.h"

#include <string>
#include <algorithm>
#include <glm.hpp>
#include <iostream>

struct CColor_hsl
{
	double h = 0.0;
	double s = 0.0;
	double l = 0.0;
	double a = 1.0;
	CColor_hsl();
	CColor_hsl(double hueDegree, double saturationPercent, double luminancePercent);
	CColor_hsl(double hueDegree, double saturationPercent, double luminancePercent, double alpha);
	void mixColor(CColor_hsl* second, float amount);
};

struct CColor_uc
{
	unsigned char r = 0;
	unsigned char g = 0;
	unsigned char b = 0;
	unsigned char a = 255;
	CColor_uc();
	CColor_uc(unsigned char red, unsigned char green, unsigned char blue);
	CColor_uc(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha);
};

struct CColor
{
	float r = 0.0f;
	float g = 0.0f;
	float b = 0.0f;
	float a = 1.0f;
	CColor();
	CColor(float fill);
	CColor(float red, float green, float blue);
	CColor(float red, float green, float blue, float alpha);
	CColor(glm::vec4 rgba);
	void setUniformly(float value);
	CColor_uc makeCColor_uc();
	std::string makeString();
	glm::vec3 makeVec3();
	glm::vec4 makeVec4();
	void mixColor(CColor* second, float amount);
	float makeGreyscale();
};


static const CColor nullColor(-5.0f, -5.0f, -5.0f);

// Keep this alphabetized
static const CColor black(0.0f, 0.0f, 0.0f);
static const CColor blue(0.0f, 0.0f, 1.0f);

static const CColor green(0.0f, 1.0f, 0.0f);

static const CColor magenta(1.0f, 0.0f, 1.0f);

static const CColor orange(1.0f, 0.498f, 0.0f);

static const CColor red(1.0f, 0.0f, 0.0f);

static const CColor white(1.0f, 1.0f, 1.0f);

static const CColor yellow(1.0f, 1.0f, 0.0f);

// Return a color by name
CColor findColor(std::string name);

// Validate whether color is nullColor
bool isNullColor(CColor query);

// Convertsion test Utility for HSL_toRGB
double HSL_conversionTest(double tempC, double temp1, double temp2);

// Convert vec3 RGB as 0-1 double, to HSL, with HSL output as Degree, Percent, Percent
CColor_hsl RGB_toHSL(CColor* input);
CColor_hsl RGB_toHSL(glm::dvec3 input);
// Convert vec3 HSL to RGB, with HSL as Degree, Percent, Percent
CColor HSL_toRGB(CColor_hsl* input);
CColor HSL_toRGB(glm::dvec3 input);
// Convert a Percent value to RGB
CColor percentValue_toRGB(double percent);

// Encode / Decode RGBA to single float channel
// Encoding/decoding [0..1) floats into 8 bit/channel RGBA. Note that 1.0 will not be encoded properly.
CColor encodeFloatRGBA(float v);
float decodeFloatRGBA(CColor enc);
// Gradient Structs (Move to New File?)
struct GradientNode
{
	float position = 0.0f;
	CColor color = black;
};

struct Gradient
{
	std::vector<GradientNode> nodes;
	unsigned int textureID = 0;
	int textureResolution = 64;
	unsigned char texture[64 * 4] = { 0 };
	void generateTexture();
};

#endif // !COLOR_H


