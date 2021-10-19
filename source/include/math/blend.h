#ifndef BLEND_H
#define BLEND_H

#include <vector>
#include <map>
#include <string>

// Blend modes
// Blend modes
enum class BlendMode
{
	null,
	//
	normal,
	erase,
	//
	multiply,
	darken,
	colorburn,
	linearburn,
	darkercolor,
	//
	lighten,
	lightercolor,
	screen,
	colordodge,
	lineardodge,
	glowdodge,
	//
	overlay,
	softlight,
	hardlight,
	vividlight,
	linearlight,
	pinlight,
	hardmix,
	//
	add,
	modulo,
	difference,
	exclusion,
	subtract,
	divide,
	intersect,
	//
	hue,
	color,
	saturation,
	luminosity,
	//
	dissolve,
	shuffle,
	scramble,
	noise,
	redshift,
	greenshift,
	blueshift,
	randomize,
};

// String name array for blend modes, must match the above Enum
inline std::map<std::string, BlendMode> BlendModeMap =
{
	{"null", BlendMode::null },
	{ "normal", BlendMode::normal },
	{ "erase", BlendMode::erase },
	//
	{ "multiply", BlendMode::multiply },
	{ "darken", BlendMode::darken },
	{ "colorburn", BlendMode::colorburn },
	{ "linearburn", BlendMode::linearburn },
	{ "darkercolor", BlendMode::darkercolor },
	//
	{ "lighten", BlendMode::lighten },
	{ "lightercolor", BlendMode::lightercolor },
	{ "screen", BlendMode::screen },
	{ "colordodge", BlendMode::colordodge },
	{ "lineardodge", BlendMode::lineardodge },
	{ "glowdodge", BlendMode::glowdodge },
	//
	{ "overlay", BlendMode::overlay },
	{ "softlight", BlendMode::softlight },
	{ "hardlight", BlendMode::hardlight },
	{ "vividlight", BlendMode::vividlight },
	{ "linearlight", BlendMode::linearlight },
	{ "pinlight", BlendMode::pinlight },
	{ "hardmix", BlendMode::hardmix },
	//
	{ "add", BlendMode::add },
	{ "modulo", BlendMode::modulo },
	{ "difference", BlendMode::difference },
	{ "exclusion", BlendMode::exclusion },
	{ "subtract", BlendMode::subtract },
	{ "divide", BlendMode::divide },
	{ "intersect", BlendMode::intersect },
	//
	{ "hue", BlendMode::hue },
	{ "color", BlendMode::color },
	{ "saturation", BlendMode::saturation },
	{ "luminosity", BlendMode::luminosity },
	//
	{ "dissolve", BlendMode::dissolve },
	{ "shuffle", BlendMode::shuffle },
	{ "scramble", BlendMode::scramble },
	{ "noise", BlendMode::noise },
	{ "redshift", BlendMode::redshift },
	{ "greenshift", BlendMode::greenshift },
	{ "blueshift", BlendMode::blueshift },
	{ "randomize", BlendMode::randomize },
};

float brightContrast(float input, float brightness, float contrast);

std::string BlendModeToString(BlendMode mode);
BlendMode stringToBlendMode(std::string mode);

float blend1(float inValue, float inNoise, BlendMode blendMode);

#endif