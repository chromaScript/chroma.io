#include "../include/math/blend.h"
#include "../include/math/noise.h"
#include "../include/math/math.h"
#include "../include/math/string.h"

#include <glm.hpp>
#include <gtc/quaternion.hpp>
#include <vector>

float brightContrast(float input, float brightness, float contrast)
{
	return (input - 0.5) * (contrast + 1.0) + 0.5 + brightness;
}

std::string BlendModeToString(BlendMode mode)
{
	for (auto& it : BlendModeMap) {
		if (it.second == mode) {
			std::string found = it.first;
			return stringToUpper(found);
		}
	}
	return "NULL";
}

BlendMode stringToBlendMode(std::string mode)
{
	std::string search = mode;
	stringRemoveSpace(search);
	stringToLower(search);
	if (BlendModeMap.count(search) == 1)
	{
		return BlendModeMap.at(search);
	}
	return BlendMode::normal;
}

float blend1(float target, float blend, BlendMode blendMode)
{
	switch (blendMode)
	{
	case BlendMode::null: break;
	case BlendMode::normal: break;
	case BlendMode::erase: break;
	//
	case BlendMode::multiply:
		target *= blend; 
		break;
	case BlendMode::darken: 
		target = min(target, blend);
		break;
	case BlendMode::colorburn:
		target = 1.0f - ((1.0f - target) / blend);
		break;
	case BlendMode::linearburn: 
		target = target + blend - 1.0f;
		break;
	case BlendMode::darkercolor: break;
	//
	case BlendMode::lighten: 
		target = max(target, blend);
		break;
	case BlendMode::lightercolor: break;
	case BlendMode::screen:
		target = 1.0f - ((1.0f - target) * (1.0f - blend));
		break;
	case BlendMode::colordodge:
		target = blend / (1.0f - target);
		break;
	case BlendMode::lineardodge: 
		target += blend;
		break;
	case BlendMode::glowdodge: break;
	//
	case BlendMode::overlay:
		target = (target < 0.5f) ? 2.0f * target * blend : 1.0f - (2.0f * ((1.0f - target) * (1.0f - blend)));
		break;
	case BlendMode::hardlight:
		target = (blend < 0.5f) ? 2.0f * target * blend : 1.0f - (2.0f * ((1.0f - target) * (1.0f - blend)));
		break;
	case BlendMode::softlight:
		target = (blend < 0.5f) ? (2.0f * target * blend) + ((target * target) * (1.0f - (2.0f * blend))) :
			((2.0f * target) * (1.0f - blend)) + (sqrtf(target) * ((2.0f * blend) - 1.0f));
		break;
	case BlendMode::vividlight: 
		target = (blend > 0.5f) ? target / (1.0f - (2.0f * (blend - 0.5f))) : 1.0f - ((1.0f - target) / (2.0f * blend));
		break;
	case BlendMode::linearlight: 
		target = (blend > 0.5f) ? target + (2.0f * (blend - 0.5f)) : target + ((2.0f * blend) - 1.0f);
		break;
	case BlendMode::pinlight:
		target = (blend > 0.5f) ? max(target, 2.0f * (blend - 0.5f)) : min(target, 2.0f * blend);
		break;
	case BlendMode::hardmix: break;
	//
	case BlendMode::add:
		target += blend; 
		break;
	case BlendMode::modulo:
		target = float(int((target + 0.0001f) * 100.0f) % int((blend + 0.0001f) * 100.0f)) / 100.0f;
		break;
	case BlendMode::difference:
		target = abs(blend - target);
		break;
	case BlendMode::exclusion:
		target = 0.5f - (2.0f * (target - 0.5f) * (blend - 0.5f));
		break;
	case BlendMode::subtract:
		target -= blend;
		break;
	case BlendMode::divide:
		target /= clampf(blend, 0.0001f, 1.0f);
		break;
	case BlendMode::intersect: break;
	//
	case BlendMode::hue: break;
	case BlendMode::color: break;
	case BlendMode::saturation: break;
	case BlendMode::luminosity: break;
	//
	case BlendMode::dissolve: break;
	case BlendMode::shuffle: break;
	case BlendMode::scramble: break;
	case BlendMode::noise: break;
	case BlendMode::redshift: break;
	case BlendMode::greenshift: break;
	case BlendMode::blueshift: break;
	case BlendMode::randomize: break;
	//
	default:
		target *= blend;
	}
	return target;
}