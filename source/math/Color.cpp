#include "../include/math/Color.h"
#include "../include/math/math.h"

#include <glm.hpp>
#include <gtc/quaternion.hpp>

// CColor_hsl
CColor_hsl::CColor_hsl() {};
CColor_hsl::CColor_hsl(double hueDegree, double saturationPercent, double luminancePercent)
{
	h = hueDegree;
	s = saturationPercent;
	l = luminancePercent;
	a = 1.0;
}
CColor_hsl::CColor_hsl(double hueDegree, double saturationPercent, double luminancePercent, double alpha)
{
	h = hueDegree;
	s = saturationPercent;
	l = luminancePercent;
	a = alpha;
}
void CColor_hsl::mixColor(CColor_hsl* second, float amount)
{
	if (abs(this->h - second->h) > 180.0) {
		this->h = clampAngle_360d(lerpd(this->h + 360.0, second->h, (double)amount));
	}
	else {
		this->h = lerpd(this->h, second->h, (double)amount);
	}
	this->s = lerpd(this->s, second->s, (double)amount);
	this->l = lerpd(this->l, second->l, (double)amount);
}

// CColor_uc
CColor_uc::CColor_uc() {};
CColor_uc::CColor_uc(unsigned char red, unsigned char green, unsigned char blue)
{
	r = red;
	g = green;
	b = blue;
	a = 255;
}
CColor_uc::CColor_uc(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha)
{
	r = red;
	g = green;
	b = blue;
	a = alpha;
}

// CColor
CColor::CColor() {};
CColor::CColor(float fill)
{
	r = g = b = a = fill;
}
CColor::CColor(float red, float green, float blue)
{
	r = red;
	g = green;
	b = blue;
	a = 1.0f;
}
CColor::CColor(float red, float green, float blue, float alpha)
{
	r = red;
	g = green;
	b = blue;
	a = alpha;
}
void CColor::setUniformly(float value)
{
	r = value;
	g = value;
	b = value;
	a = 1.0f;
}
CColor_uc CColor::makeCColor_uc()
{
	return CColor_uc((unsigned char)(r * 255), (unsigned char)(g * 255), (unsigned char)(b * 255), (unsigned char)(a * 255));
}
std::string CColor::makeString()
{
	return std::to_string(r) + "," + std::to_string(g) + "," + std::to_string(b) + "," + std::to_string(a);
}
glm::vec3 CColor::makeVec3()
{
	return glm::vec3(r, g, b);
}
glm::vec4 CColor::makeVec4()
{
	return glm::vec4(r, g, b, a);
}
void CColor::mixColor(CColor* second, float amount)
{
	this->r = lerpf(this->r, second->r, amount);
	this->g = lerpf(this->g, second->g, amount);
	this->b = lerpf(this->b, second->b, amount);
}
float CColor::makeGreyscale()
{
	return clampf((this->r * 0.21f) + (this->g * 0.72f) + (this->b * 0.07f), 0.0f, 1.0f);
}

// Return a color by name
CColor findColor(std::string name)
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
bool isNullColor(CColor query)
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
double HSL_conversionTest(double tempC, double temp1, double temp2)
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
CColor_hsl RGB_toHSL(CColor* input)
{
	return RGB_toHSL(glm::dvec3(input->r, input->g, input->b));
}

// Convert vec3 RGB as 0-1 double, to HSL, with HSL output as Degree, Percent, Percent
CColor_hsl RGB_toHSL(glm::dvec3 input)
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
		if (hue < 0.0) { hue = 360 + hue; }
		else if (hue < -60.0) { hue = 360; }
	}
	return CColor_hsl(hue, saturation, luminance);
}

CColor HSL_toRGB(CColor_hsl* input)
{
	return HSL_toRGB(glm::dvec3(input->h / 360.0, input->s, input->l));
}

// Convert vec3 HSL to RGB, with HSL as Degree, Percent, Percent
CColor HSL_toRGB(glm::dvec3 input)
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

CColor percentValue_toRGB(double percent)
{
	float clampPercent = clampf((float)percent, 0.0f, 1.0f);
	return CColor(clampPercent, clampPercent, clampPercent);
}

void Gradient::generateTexture()
{
	for (int i = 0; i < textureResolution; i++)
	{
		// Generate rainbow sample
		CColor sampleRGB = HSL_toRGB(glm::dvec3((double)i / 64.0, 1.0, 0.5));
		CColor_uc sample = sampleRGB.makeCColor_uc();
		texture[(i * 4)] = sample.r;
		texture[(i * 4) + 1] = sample.g;
		texture[(i * 4) + 2] = sample.b;
		texture[(i * 4) + 3] = 255;
	}
}

