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
	CColor(std::string hexStr);
	CColor(std::string hexStr, float alpha);
	CColor(float red, float green, float blue);
	CColor(float red, float green, float blue, float alpha);
	CColor(unsigned char red, unsigned char green, unsigned char blue, bool uchar);
	CColor(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha, bool uchar);
	CColor(glm::vec4 rgba);
	void setUniformly(float value);
	CColor_uc makeCColor_uc();
	std::string makeString();
	glm::vec3 makeVec3();
	glm::vec4 makeVec4();
	CColor mixColor(CColor* second, float amount);
	float makeGreyscale();
};


static const CColor nullColor(-5.0f, -5.0f, -5.0f);

// CSS Colors
static const CColor black("#000000");
static const CColor silver("#c0c0c0");
static const CColor gray("#808080");
static const CColor white("#ffffff");
static const CColor maroon("#800000");
static const CColor red("#ff0000");
static const CColor orange("#ffa500");
static const CColor purple("#800080");
static const CColor fuchsia("#ff00ff");
static const CColor magenta("#ff00ff");
static const CColor green("#008000");
static const CColor lime("#00ff00");
static const CColor olive("#808000");
static const CColor yellow("#ffff00");
static const CColor navy("#000080");
static const CColor blue("#0000ff");
static const CColor teal("#008080");
static const CColor aqua("#00ffff");

// Pantone Colors
static const CColor pantone_green(28, 172, 120, true);

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


