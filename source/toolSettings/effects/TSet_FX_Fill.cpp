#include "../../include/ToolSettings.h"
#include "../../include/keys.h"
#include "../../include/Color.h"

class CFunction;
#include "../../include/cscript/CInterpreter.h"
#include "../../include/cscript/CObject.h"
#include "../../include/Tool.h"

#include <glm.hpp>
#include <gtx/rotate_vector.hpp>
#include <string>
#include <vector>
#include <memory>
#include <random>

#include "../../include/structs.h"
#include "../../include/toolSettings/effects/TSet_FX_Fill.h"

////////////////////////////////////////////////////////////////
// FX FILL Settings
////////////////////////////////////////////////////////////////

TSet_FX_Fill::TSet_FX_Fill()
{
	this->type = TSetType::fx_fill;
	this->isEnabled = false;
	this->colorUsage = TSetProp::backgroundColor; // 8100
	this->color = blue; // 8101 // 0, 1, 2, 3
	this->hueMin = -85.0f; // 8104
	this->hueMax = -85.0f; // 8105
	this->satMin = -0.5f; // 8106
	this->satMax = 0.5f; // 8107
	this->valMin = -0.5f; // 8108
	this->valMax = 0.5f; // 8109
	this->mixAmountMin = 1.0f; // 8110
	this->mixAmountMax = 1.0f; // 8111
	this->mixAmountBias = 0.0f; // 8112

	this->useChannelMask = false; // 8120
	this->channelMask = TSetProp::value; // 8121
	this->channelMask_randomize = false;
	this->channelMask_randomize_mode = TSetProp::all;

	this->maskX = true; // 8130
	this->maskX_invert = false; // 8131
	this->maskX_invertRandom = false; // 8132
	this->maskX_variance = 0.0f; // 8133
	this->maskX_random = false; // 8134
	this->maskY = false; // 8135
	this->maskY_invert = false; // 8136
	this->maskY_invertRandom = false; // 8137
	this->maskY_variance = 0.0f; // 8138
	this->maskY_random = false; // 8139
	this->maskX_noiseChunkSize = 8.0f; // 8140
	this->maskY_noiseChunkSize = 8.0f; // 8141
	this->maskXY_noiseSizeRelative = true; // 8142

	this->maskUsingAngle = false; // 8150
	this->maskAngleMin = 45.0f; // 8151
	this->maskAngleMax = 45.0f; // 8152

	this->maskUsingCenter = false; // 8160
	this->combineCenterMask = true; // 8161
	this->maskCenter_invert = false; // 8162
	this->maskCenter_invertRandom = true; // 8163
	this->maskCenter_offsetXMin = -0.0f; // 8164
	this->maskCenter_offsetXMax = 0.0f; // 8165
	this->maskCenter_offsetXBias = 0.0f; // 8166
	this->maskCenter_offsetYMin = -0.0f; // 8167
	this->maskCenter_offsetYMax = 0.0f; // 8168
	this->maskCenter_offsetYBias = 0.0f; // 8169
}

std::shared_ptr<CObject> TSet_FX_Fill::putProperty(
	std::shared_ptr<CInterpreter> interpreter, std::shared_ptr<Tool> owner,
	int settingsSig, int subSig, std::shared_ptr<CObject> object, bool isGet, bool asPercentage, bool asString)
{

	if (asString) { return std::make_shared<CObject>(makeSettingString(isEnabled, settingsSig, subSig, "NULL")); }
	return std::make_shared<CObject>(nullptr);
}

void TSet_FX_Fill::initializeData(CColor FGColor, CColor BGColor, int tipSize)
{
	std::random_device device;
	std::default_random_engine eng(device());
	std::uniform_real_distribution<float> distr(0, 1);

	// Modulate Fill Color Randomization
	CColor_hsl outFinalColor = CColor_hsl();
	switch (colorUsage)
	{
	case TSetProp::foregroundColor: outFinalColor = RGB_toHSL(glm::dvec3((double)FGColor.r, (double)FGColor.g, (double)FGColor.b)); break;
	case TSetProp::backgroundColor: outFinalColor = RGB_toHSL(glm::dvec3((double)BGColor.r, (double)BGColor.g, (double)BGColor.b)); break;
	default: outFinalColor = RGB_toHSL(glm::dvec3((double)color.r, (double)color.g, (double)color.b)); break;
	}

	/*
	outFinalColor.h = clampAngle_360(outFinalColor.h + lerpf(hueMin, hueMax, (double)distr(eng)));
	if (outFinalColor.s > 0) { outFinalColor.s = clampd(outFinalColor.s + lerpf(satMin, satMax, (double)distr(eng)), 0.0, 1.0); }
	outFinalColor.l = clampd(outFinalColor.l + lerpf(valMin, valMax, (double)distr(eng)), 0.0, 1.0);
	*/
	// Convert back to RGB
	finalColor = HSL_toRGB(glm::dvec3(outFinalColor.h / 360.0, outFinalColor.s, outFinalColor.l));

	// Determine Mix Amount
	finalColor.a = lerpf(mixAmountMin, mixAmountMax, (double)distr(eng));

	// Determine Random X/Y Masking
	if (maskX && maskX_random) { finalMaskX = (distr(eng) > 0.5) ? true : false; }
	else { finalMaskX = maskX; }
	if (maskY && maskY_random) { finalMaskY = (distr(eng) > 0.5) ? true : false; }
	else { finalMaskY = maskY; }
	// Determine Random X/Y Inversion
	if (maskX_invert && maskX_invertRandom) { finalMaskXInvert = (distr(eng) > 0.5) ? true : false; }
	else { finalMaskXInvert = maskX_invert; }
	if (maskY_invert && maskY_invertRandom) { finalMaskYInvert = (distr(eng) > 0.5) ? true : false; }
	else { finalMaskYInvert = maskY_invert; }

	// Determine Chunk Size
	if (maskXY_noiseSizeRelative)
	{
		finalChunkSizeX = maskX_noiseChunkSize * (tipSize / 100.0f);
		finalChunkSizeY = maskY_noiseChunkSize * (tipSize / 100.0f);
	}
	else
	{
		finalChunkSizeX = maskX_noiseChunkSize;
		finalChunkSizeY = maskY_noiseChunkSize;
	}

	// Determine XY Mask Angle
	if (maskUsingAngle) { maskAngle = lerpf(maskAngleMin, maskAngleMax, (double)distr(eng)); }

	// Determine XY Mask Center Offset
	if (maskUsingCenter)
	{
		maskCenter_offsetX = lerpf(maskCenter_offsetXMin, maskCenter_offsetXMax, (double)distr(eng));
		maskCenter_offsetY = lerpf(maskCenter_offsetYMin, maskCenter_offsetYMax, (double)distr(eng));
		if (maskCenter_invert && maskCenter_invertRandom) { finalMaskCenterInvert = (distr(eng) > 0.5) ? true : false; }
		else { finalMaskCenterInvert = maskCenter_invert; }
	}

	// Determine Channel Mask if Random
	if (channelMask_randomize)
	{
		int choice = 0;
		switch (channelMask_randomize_mode)
		{
		case TSetProp::all: choice = clampi(int(roundf(distr(eng) * 5)), 0, 5); break;
		case TSetProp::allChannels: choice = clampi(int(roundf(distr(eng) * 4.0f) + 1.0f), 1, 5); break;
		case TSetProp::rgb: choice = clampi(int(roundf(distr(eng) * 2.0f) + 1.0f), 1, 3); break;
		case TSetProp::rgba: choice = clampi(int(roundf(distr(eng) * 3.0f) + 1.0f), 1, 3); break;
		default: choice = 0;
		}
		std::cout << "CHOICE : " << choice << std::endl;
		switch (choice)
		{
		case 1: finalChannelMask = TSetProp::red; break;
		case 2: finalChannelMask = TSetProp::green; break;
		case 3: finalChannelMask = TSetProp::blue; break;
		case 4: finalChannelMask = TSetProp::alpha; break;
		case 5: finalChannelMask = TSetProp::value; break;
		default: finalChannelMask = TSetProp::none;
		}
	}
}
int TSet_FX_Fill::getChannelMask()
{
	switch (finalChannelMask)
	{
	case TSetProp::red: return 1; break;
	case TSetProp::green: return 2; break;
	case TSetProp::blue: return 3; break;
	case TSetProp::alpha: return 4; break;
	case TSetProp::value: return 5; break;
	default: return 0;
	}
}