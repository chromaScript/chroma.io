#ifndef COLOR_H
#define COLOR_H

#include "math.h"

#include <string>
#include <algorithm>
#include <glm.hpp>

struct CColor_hsl
{
	double h = 0.0;
	double s = 0.0;
	double l = 0.0;
	double a = 1.0;
	CColor_hsl() {};
	CColor_hsl(double hueDegree, double saturationPercent, double luminancePercent)
	{
		h = hueDegree;
		s = saturationPercent;
		l = luminancePercent;
		a = 1.0;
	}
	CColor_hsl(double hueDegree, double saturationPercent, double luminancePercent, double alpha)
	{
		h = hueDegree;
		s = saturationPercent;
		l = luminancePercent;
		a = alpha;
	}
};

struct CColor_uc
{
	unsigned char r = 0;
	unsigned char g = 0;
	unsigned char b = 0;
	unsigned char a = 255;
	CColor_uc() {};
	CColor_uc(unsigned char red, unsigned char green, unsigned char blue)
	{
		r = red;
		g = green;
		b = blue;
		a = 255;
	}
	CColor_uc(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha)
	{
		r = red;
		g = green;
		b = blue;
		a = alpha;
	}
};

struct CColor
{
	float r = 0.0f;
	float g = 0.0f;
	float b = 0.0f;
	float a = 1.0f;
	CColor() {};
	CColor(float fill)
	{
		r = g = b = a = fill;
	}
	CColor(float red, float green, float blue)
	{
		r = red;
		g = green;
		b = blue;
		a = 1.0f;
	}
	CColor(float red, float green, float blue, float alpha)
	{
		r = red;
		g = green;
		b = blue;
		a = alpha;
	}
	void setUniformly(float value)
	{
		r = value;
		g = value;
		b = value;
		a = 1.0f;
	}
	CColor_uc makeCColor_uc()
	{
		return CColor_uc((unsigned char)(r * 255), (unsigned char)(g * 255), (unsigned char)(b * 255), (unsigned char)(a * 255));
	}
	std::string makeString()
	{
		return std::to_string(r) + "," + std::to_string(g) + "," + std::to_string(b) + "," + std::to_string(a);
	}
	glm::vec3 makeVec3()
	{
		return glm::vec3(r, g, b);
	}
	glm::vec4 makeVec4()
	{
		return glm::vec4(r, g, b, a);
	}
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
inline CColor findColor(std::string name)
{
	// Kick bad-calls
	if (name.size() == 0) { return black; }
	switch (name[0])
	{
	case 'b':
		if (name == "black") { return black; }
		if (name == "blue") { return blue; }
		else { return nullColor; }
	case 'g':
		if (name == "green") { return green; }
		else { return nullColor; }
	case 'm':
		if (name == "magenta") { return magenta; }
		else { return nullColor; }
	case 'o':
		if (name == "orange") { return orange; }
		else { return nullColor; }
	case 'r':
		if (name == "red") { return red; }
		else { return nullColor; }
	case 'w':
		if (name == "white") { return white; }
		else { return nullColor; }
	case 'y':
		if (name == "yellow") { return yellow; }
		else { return nullColor; }
	default:
		return nullColor;
	}
}
// Validate whether color is nullColor
inline bool isNullColor(CColor query)
{
	if (query.r - nullColor.r <= 0.001f ||
		query.g - nullColor.g <= 0.001f ||
		query.b - nullColor.b <= 0.001f)
	{
		return true;
	}
	return false;
}

// Convertsion test Utility for HSL_toRGB
inline double HSL_conversionTest(double tempC, double temp1, double temp2)
{
	if (6.0 * tempC < 1.0)
	{
		return temp2 + (temp1 - temp2) * 6.0 * tempC;
	}
	else
	{
		if (2 * tempC < 1)
		{
			return temp1;
		}
		else
		{
			if (3 * tempC < 2)
			{
				return temp2 + (temp1 - temp2) * (0.6666 - tempC) * 6;
			}
			else
			{
				return temp2;
			}
		}
	}
}
// Convert vec3 RGB as 0-1 double, to HSL, with HSL output as Degree, Percent, Percent
inline CColor_hsl RGB_toHSL(glm::dvec3 input)
{
	double r = input.x;
	double g = input.y;
	double b = input.z;
	// Make an array and sort it
	double arr[] = { r, g, b };
	std::sort(arr, arr + 3);
	// Get the minimum and maximum values
	double min = arr[0];
	double max = arr[2];
	// Calculate luminance
	double luminance = (min + max) / 2;
	// Calculate saturation and luminance
	double saturation = 0.0;
	double hue = 0.0;
	// If min == max, then saturation is 0, and hue is also then 0
	if (min == max)
	{
		hue = 0.0;
		saturation = 0.0;
	}
	else
	{
		// Calculate luminance value
		if (luminance <= 0.5)
		{
			saturation = (max - min) / (max + min);
		}
		else
		{
			saturation = (max - min) / (2.0 - max - min);
		}
		// Calculate hue value
		if (max == r)
		{
			hue = (g - b) / (max - min);
		}
		else if (max == g)
		{
			hue = 2.0 + ((b - r) / (max - min));
		}
		else
		{
			hue = 4.0 + ((r - g) / (max - min));
		}
		hue *= 60.0;
		if (hue < 0.0) { hue = 360; }
	}
	return CColor_hsl(hue, saturation, luminance);
}

// Convert vec3 HSL to RGB, with HSL as Degree, Percent, Percent
inline CColor HSL_toRGB(glm::dvec3 input)
{
	double degree = input.x;
	double saturation = input.y;
	double luminance = input.z;
	// Calculate First Step Values
	double t1 = (luminance < 0.5) ? 
		luminance * (1.0 + saturation) : 
		(luminance + saturation) - (luminance * saturation);
	double t2 = (2.0 * luminance) - t1;
	double tr = degree + 0.333f;
	if (tr > 1.0f) { tr = tr - 1.0f; }
	else if (tr < 0.0f) { tr = tr + 1.0f; }
	double tg = degree;
	double tb = degree - 0.333f;
	if (tb > 1.0f) { tb = tb - 1.0f; }
	else if (tb < 0.0f) { tb = tb + 1.0f; }

	tr = HSL_conversionTest(tr, t1, t2);
	tg = HSL_conversionTest(tg, t1, t2);
	tb = HSL_conversionTest(tb, t1, t2);

	return CColor((float)tr, (float)tg, (float)tb);
}
// Convert a Percent value to RGB
inline CColor percentValue_toRGB(double percent)
{
	float clampPercent = clampf((float)percent, 0.0f, 1.0f);
	return CColor(clampPercent, clampPercent, clampPercent);
}

#endif // !COLOR_H


