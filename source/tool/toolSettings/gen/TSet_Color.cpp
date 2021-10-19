#include "../../../include/tool/ToolSettings.h"
#include "../../../include/input/keys.h"
#include "../../../include/math/Color.h"

class CFunction;
#include "../../../include/cscript/CInterpreter.h"
#include "../../../include/cscript/CObject.h"
#include "../../../include/tool/Tool.h"

#include <glm.hpp>
#include <gtx/rotate_vector.hpp>
#include <string>
#include <vector>
#include <memory>
#include <random>

#include "../../../include/tool/toolSettings/gen/TSet_Color.h"

////////////////////////////////////////////////////////////////
// Color Dynamics
////////////////////////////////////////////////////////////////

TSet_Color::TSet_Color()
{
	this->isEnabled = false;
	this->type = TSetType::color;

}
std::shared_ptr<CObject> TSet_Color::putProperty(
	std::shared_ptr<CInterpreter> interpreter, std::shared_ptr<Tool> owner,
	int settingsSig, int subSig, std::shared_ptr<CObject> object, bool isGet, bool asPercentage, bool asString)
{
	if (settingsSig >= 2700 && settingsSig < 2710)
	{
		if (settingsSig == 2700)
		{
			return mixColorNode.putProperty(interpreter, owner, settingsSig, subSig, object, isGet, asPercentage, asString);
		}
		else if (settingsSig == 2701)
		{
			return mixColorController.putProperty(interpreter, owner, settingsSig, subSig, object, isGet, asPercentage, asString);
		}
		else if (settingsSig == 2702)
		{
			return mixColorNoise.putProperty(interpreter, owner, settingsSig, subSig, object, isGet, asPercentage, asString);
		}
		else if (settingsSig == 2703) {
			if (!isGet) { colorMode = stringToTSetProp(std::get<std::string>(object.get()->obj)); }
			else {
				if (asString) { return std::make_shared<CObject>(makeSettingString(colorMode, settingsSig, subSig, "colorMode")); }
				return std::make_shared<CObject>(TSetPropToString(colorMode));
			}
		}
		else if (settingsSig == 2706) {
			if (!isGet) {
				if (asPercentage) {
					colorHueOffset = percentRange_squaredCenter(
						(float)std::get<double>(object.get()->obj), -180.0f, 180.0f, false);
				}
				else { colorHueOffset = clampf((float)std::get<double>(object.get()->obj), -180.0f, 180.0f); }
			}
			else {
				if (asString) { return std::make_shared<CObject>(makeSettingString(colorHueOffset, settingsSig, subSig, "colorHueOffset")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange_squaredCenter(colorHueOffset, -180.0f, 180.0f, true)));
				}
				else { return std::make_shared<CObject>(double(colorHueOffset)); }
			}
		}
		else if (settingsSig == 2707) {
			if (!isGet) {
				if (asPercentage) {
					colorSatOffset = percentRange_squaredCenter(
						(float)std::get<double>(object.get()->obj), -1.0, 1.0f, false);
				}
				else { colorSatOffset = clampf((float)std::get<double>(object.get()->obj), -1.0f, 1.0f); }
			}
			else {
				if (asString) { return std::make_shared<CObject>(makeSettingString(colorSatOffset, settingsSig, subSig, "colorSatOffset")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange_squaredCenter(colorSatOffset, -1.0f, 1.0f, true)));
				}
				else { return std::make_shared<CObject>(double(colorSatOffset)); }
			}
		}
		else if (settingsSig == 2708) {
			if (!isGet) {
				if (asPercentage) {
					colorValOffset = percentRange_squaredCenter(
						(float)std::get<double>(object.get()->obj), -1.0, 1.0f, false);
				}
				else { colorValOffset = clampf((float)std::get<double>(object.get()->obj), -1.0f, 1.0f); }
			}
			else {
				if (asString) { return std::make_shared<CObject>(makeSettingString(colorValOffset, settingsSig, subSig, "colorValOffset")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange_squaredCenter(colorValOffset, -1.0f, 1.0f, true)));
				}
				else { return std::make_shared<CObject>(double(colorValOffset)); }
			}
		}
		else if (settingsSig == 2709) {
			if (!isGet) { enableMixColor = std::get<bool>(object.get()->obj); }
			else {
				if (asString) { return std::make_shared<CObject>(makeSettingString(enableMixColor, settingsSig, subSig, "enableMixColor")); }
				return std::make_shared<CObject>(enableMixColor);
			}
		}
	}
	else if (settingsSig >= 2710 && settingsSig < 2720)
	{
		if (settingsSig == 2710) {
			if (!isGet) { mixColorMode = stringToTSetProp(std::get<std::string>(object.get()->obj)); }
			else {
				if (asString) { return std::make_shared<CObject>(makeSettingString(mixColorMode, settingsSig, subSig, "mixColorMode")); }
				return std::make_shared<CObject>(TSetPropToString(mixColorMode));
			}
		}
		else if (settingsSig == 2711) {
			if (!isGet) { mixRampMode = stringToTSetProp(std::get<std::string>(object.get()->obj)); }
			else {
				if (asString) { return std::make_shared<CObject>(makeSettingString(mixRampMode, settingsSig, subSig, "mixRampMode")); }
				return std::make_shared<CObject>(TSetPropToString(mixRampMode));
			}
		}
	}
	else if (settingsSig >= 2720 && settingsSig < 2730)
	{
		if (settingsSig == 2720)
		{
			return colorHueRedNode.putProperty(interpreter, owner, settingsSig, subSig, object, isGet, asPercentage, asString);
		}
		if (settingsSig == 2721)
		{
			return colorSatGreenNode.putProperty(interpreter, owner, settingsSig, subSig, object, isGet, asPercentage, asString);
		}
		if (settingsSig == 2722)
		{
			return colorValBlueNode.putProperty(interpreter, owner, settingsSig, subSig, object, isGet, asPercentage, asString);
		}
		else if (settingsSig == 2723)
		{
			return colorController.putProperty(interpreter, owner, settingsSig, subSig, object, isGet, asPercentage, asString);
		}
		else if (settingsSig == 2724)
		{
			return colorHueRedNoise.putProperty(interpreter, owner, settingsSig, subSig, object, isGet, asPercentage, asString);
		}
		else if (settingsSig == 2725)
		{
			return colorSatGreenNoise.putProperty(interpreter, owner, settingsSig, subSig, object, isGet, asPercentage, asString);
		}
		else if (settingsSig == 2726)
		{
			return colorValBlueNoise.putProperty(interpreter, owner, settingsSig, subSig, object, isGet, asPercentage, asString);
		}
		else if (settingsSig == 2727) {
			if (!isGet) { colorTypeMode = stringToTSetProp(std::get<std::string>(object.get()->obj)); }
			else {
				if (asString) { return std::make_shared<CObject>(makeSettingString(colorTypeMode, settingsSig, subSig, "colorTypeMode")); }
				return std::make_shared<CObject>(TSetPropToString(colorTypeMode));
			}
		}
		else if (settingsSig == 2728) {
			if (!isGet) { enableColor = std::get<bool>(object.get()->obj); }
			else {
				if (asString) { return std::make_shared<CObject>(makeSettingString(enableColor, settingsSig, subSig, "enableColor")); }
				return std::make_shared<CObject>(enableColor);
			}
		}
	}
	else if (settingsSig >= 2770 && settingsSig < 2780)
	{
		if (settingsSig == 2770)
		{
			return finalHueRedNode.putProperty(interpreter, owner, settingsSig, subSig, object, isGet, asPercentage, asString);
		}
		else if (settingsSig == 2771)
		{
			return finalSatGreenNode.putProperty(interpreter, owner, settingsSig, subSig, object, isGet, asPercentage, asString);
		}
		else if (settingsSig == 2772)
		{
			return finalValBlueNode.putProperty(interpreter, owner, settingsSig, subSig, object, isGet, asPercentage, asString);
		}
		else if (settingsSig == 2773)
		{
			return finalController.putProperty(interpreter, owner, settingsSig, subSig, object, isGet, asPercentage, asString);
		}
		else if (settingsSig == 2774)
		{
			return finalHueRedNoise.putProperty(interpreter, owner, settingsSig, subSig, object, isGet, asPercentage, asString);
		}
		else if (settingsSig == 2775)
		{
			return finalSatGreenNoise.putProperty(interpreter, owner, settingsSig, subSig, object, isGet, asPercentage, asString);
		}
		else if (settingsSig == 2776)
		{
			return finalValBlueNoise.putProperty(interpreter, owner, settingsSig, subSig, object, isGet, asPercentage, asString);
		}
		else if (settingsSig == 2777) {
			if (!isGet) { finalTypeMode = stringToTSetProp(std::get<std::string>(object.get()->obj)); }
			else {
				if (asString) { return std::make_shared<CObject>(makeSettingString(finalTypeMode, settingsSig, subSig, "finalTypeMode")); }
				return std::make_shared<CObject>(TSetPropToString(finalTypeMode));
			}
		}
		else if (settingsSig == 2778) {
			if (!isGet) { enableFinalColor = std::get<bool>(object.get()->obj); }
			else {
				if (asString) { return std::make_shared<CObject>(makeSettingString(enableFinalColor, settingsSig, subSig, "enableFinalColor")); }
				return std::make_shared<CObject>(enableFinalColor);
			}
		}
	}
	

	if (asString) { return std::make_shared<CObject>(makeSettingString(isEnabled, settingsSig, subSig, "NULL")); }
	return std::make_shared<CObject>(nullptr);
}

TSetControl_Node* TSet_Color::getControlNode(int settingSig, int subSig)
{
	if (settingSig == 2700) { return &mixColorNode; }
	else if (settingSig == 2720) { return &colorHueRedNode; }
	else if (settingSig == 2721) { return &colorSatGreenNode; }
	else if (settingSig == 2722) { return &colorValBlueNode; }
	else if (settingSig == 2770) { return &finalHueRedNode; }
	else if (settingSig == 2771) { return &finalSatGreenNode; }
	else if (settingSig == 2772) { return &finalValBlueNode; }
	return nullptr;
}
TSetController* TSet_Color::getController(int settingSig, int subSig)
{
	if (settingSig == 2701) { return &mixColorController; }
	else if (settingSig == 2723) { return &colorController; }
	else if (settingSig == 2773) { return &finalController; }
	return nullptr;
}
TSetGraph* TSet_Color::getGraph(int settingSig, int subSig)
{
	if (settingSig == 2701)
	{
		if (subSig == -1) { return &mixColorController.pressureGraph; }
		else if (subSig == -2) { return &mixColorController.directionGraph; }
		else if (subSig == -3) { return &mixColorController.tiltGraph; }
		else if (subSig == -4) { return &mixColorController.velocityGraph; }
		else if (subSig == -5) { return &mixColorController.rotationGraph; }
	}
	if (settingSig == 2723)
	{
		if (subSig == -1) { return &colorController.pressureGraph; }
		else if (subSig == -2) { return &colorController.directionGraph; }
		else if (subSig == -3) { return &colorController.tiltGraph; }
		else if (subSig == -4) { return &colorController.velocityGraph; }
		else if (subSig == -5) { return &colorController.rotationGraph; }
	}
	if (settingSig == 2773)
	{
		if (subSig == -1) { return &finalController.pressureGraph; }
		else if (subSig == -2) { return &finalController.directionGraph; }
		else if (subSig == -3) { return &finalController.tiltGraph; }
		else if (subSig == -4) { return &finalController.velocityGraph; }
		else if (subSig == -5) { return &finalController.rotationGraph; }
	}
	return nullptr;
}
TSetNoise* TSet_Color::getNoise(int settingSig, int subSig)
{
	if (settingSig == 2702) { return &mixColorNoise; }
	if (settingSig == 2724) { return &colorHueRedNoise; }
	if (settingSig == 2725) { return &colorSatGreenNoise; }
	if (settingSig == 2726) { return &colorValBlueNoise; }
	if (settingSig == 2774) { return &finalHueRedNoise; }
	if (settingSig == 2775) { return &finalSatGreenNoise; }
	if (settingSig == 2776) { return &finalValBlueNoise; }
	return nullptr;
}

void TSet_Color::initialize(float* trueSpacing, int* entityCount, glm::ivec2* canvasDimensions)
{
	if (!isEnabled) { return; }
	if (mixColorNoise.isEnabled) { mixColorNoise.initialize(trueSpacing, entityCount, canvasDimensions); }
	//
	if (colorHueRedNoise.isEnabled) { colorHueRedNoise.initialize(trueSpacing, entityCount, canvasDimensions); }
	if (colorSatGreenNoise.isEnabled) { colorSatGreenNoise.initialize(trueSpacing, entityCount, canvasDimensions); }
	if (colorValBlueNoise.isEnabled) { colorValBlueNoise.initialize(trueSpacing, entityCount, canvasDimensions); }
	//
	if (finalHueRedNoise.isEnabled) { colorHueRedNoise.initialize(trueSpacing, entityCount, canvasDimensions); }
	if (finalSatGreenNoise.isEnabled) { colorSatGreenNoise.initialize(trueSpacing, entityCount, canvasDimensions); }
	if (finalValBlueNoise.isEnabled) { colorValBlueNoise.initialize(trueSpacing, entityCount, canvasDimensions); }
}

void TSet_Color::modulateColor(CColor& outColor, CColor* FGcolor, CColor* BGcolor,
	glm::vec3* pos, glm::vec3* origin, glm::vec3* dir, Input* input,
	int* shardCount, int* anchorCount, int* splineCount)
{
	if (FGcolor == nullptr || BGcolor == nullptr) {
		printStr("TOOL::SETTINGS::MODULATE_COLOR::NULLPTR");
	}
	bool calcNextHSL = false;
	colorActive = *FGcolor;
	colorActive_hsl = RGB_toHSL(&colorActive);

	// Calculate the active color with offsets and active color mix value
	float colorMixValue = 1.0f;
	if (enableMixColor)
	{
		switch (colorMode)
		{
		case TSetProp::background: colorActive = *BGcolor; break;
		case TSetProp::foreground: colorActive = *FGcolor;  break;
		case TSetProp::custom:
		default: colorActive = colorStored;
		}

		if (colorHueOffset != 0.0f || colorSatOffset != 0.0f || colorValOffset != 0.0f)
		{
			calcNextHSL = true;
			colorActive_hsl = RGB_toHSL(glm::dvec3((double)colorActive.r, (double)colorActive.g, (double)colorActive.b));
			colorActive_hsl.h = clampAngle_360d(colorActive_hsl.h + (double)colorHueOffset);
			colorActive_hsl.s = clampd(colorActive_hsl.s + (double)colorSatOffset, 0.0, 1.0);
			colorActive_hsl.l = clampd(colorActive_hsl.l + (double)colorValOffset, 0.0, 1.0);
		}
		// Modulate active color mix value before assessing control/noise adjustment
		if (mixColorNode.reportEnabled()) {
			mixColorNode.calculateControl(colorMixValue, pos, dir, input, &mixColorController, anchorCount);
		}
		colorMixValue = lerpf(mixColorNode.controlMin, mixColorNode.controlMax, colorMixValue);
		if (mixColorNoise.isEnabled) {
			mixColorNoise.calculateNoise(colorMixValue, pos, origin, dir, input, shardCount, anchorCount, splineCount, false);
		}
		colorMixValue = clampf(colorMixValue, mixColorNode.rangeMin, mixColorNode.rangeMax);

		if (enableColor)
		{
			// Calculate the color HSV/RGB control/noise value
			// Hue and Red Value
			float RedHueControl = 1.0f; float GreenSatControl = 1.0f; float BlueValControl = 1.0f;
			bool redEnabled = false; bool greenEnabled = false; bool blueEnabled = false;
			if (colorHueRedNode.reportEnabled()) {
				redEnabled = true;
				colorHueRedNode.calculateControl(RedHueControl, pos, dir, input, &colorController, anchorCount);
			}
			if (colorHueRedNoise.isEnabled) {
				colorHueRedNoise.calculateNoise(RedHueControl, pos, origin, dir, input, shardCount, anchorCount, splineCount, false);
			}
			if (!redEnabled && !colorHueRedNoise.isEnabled) { RedHueControl = 0.0f; }
			RedHueControl = lerpf(colorHueRedNode.controlMin, colorHueRedNode.controlMax, RedHueControl);
			// Saturation and Green Value
			if (colorSatGreenNode.reportEnabled()) {
				greenEnabled = true;
				colorSatGreenNode.calculateControl(GreenSatControl, pos, dir, input, &colorController, anchorCount);
			}
			if (colorSatGreenNoise.isEnabled) {
				colorSatGreenNoise.calculateNoise(GreenSatControl, pos, origin, dir, input, shardCount, anchorCount, splineCount, false);
			}
			if (!greenEnabled && !colorSatGreenNoise.isEnabled) { GreenSatControl = 0.0f; }
			GreenSatControl = lerpf(colorSatGreenNode.controlMin, colorSatGreenNode.controlMax, GreenSatControl);
			// Value and Blue Value
			if (colorValBlueNode.reportEnabled()) {
				blueEnabled = true;
				colorValBlueNode.calculateControl(BlueValControl, pos, dir, input, &colorController, anchorCount);
			}
			if (colorValBlueNoise.isEnabled) {
				colorValBlueNoise.calculateNoise(BlueValControl, pos, origin, dir, input, shardCount, anchorCount, splineCount, false);
			}
			if (!blueEnabled && !colorValBlueNoise.isEnabled) { BlueValControl = 0.0f; }
			BlueValControl = lerpf(colorValBlueNode.controlMin, colorValBlueNode.controlMax, BlueValControl);

			// Apply Each Control Control/Noise for Active Color (Mix Color)
			if (colorTypeMode == TSetProp::hsl) {
				if (calcNextHSL) {
					colorActive_hsl.h = clampAngle_360((float)colorActive_hsl.h + (RedHueControl * 180.0f));
					colorActive_hsl.s = clampd(colorActive_hsl.s + (double)GreenSatControl, 0.0, 1.0);
					colorActive_hsl.l = clampd(colorActive_hsl.l + (double)BlueValControl, 0.0, 1.0);
				}
				else {
					colorActive_hsl = RGB_toHSL(&colorActive);
					calcNextHSL = true;
					colorActive_hsl.h = clampAngle_360((float)colorActive_hsl.h + (RedHueControl * 180.0f));
					colorActive_hsl.s = clampd(colorActive_hsl.s + (double)GreenSatControl, 0.0, 1.0);
					colorActive_hsl.l = clampd(colorActive_hsl.l + (double)BlueValControl, 0.0, 1.0);
				}
			}
			else if (colorTypeMode == TSetProp::rgb) {
				if (calcNextHSL) {
					colorActive = HSL_toRGB(&colorActive_hsl);
					calcNextHSL = false;
					colorActive.r = clampd(colorActive.r + (double)RedHueControl, 0.0, 1.0);
					colorActive.g = clampd(colorActive.g + (double)GreenSatControl, 0.0, 1.0);
					colorActive.b = clampd(colorActive.b + (double)BlueValControl, 0.0, 1.0);
				}
				else {
					colorActive.r = clampd(colorActive.r + (double)RedHueControl, 0.0, 1.0);
					colorActive.g = clampd(colorActive.g + (double)GreenSatControl, 0.0, 1.0);
					colorActive.b = clampd(colorActive.b + (double)BlueValControl, 0.0, 1.0);
				}
			}
		}
	}
	// Calculate the ramp color and ramp color mix value

	// Mix colors to foreground color for final color result
	if (enableMixColor)
	{
		if (calcNextHSL)
		{
			if (mixColorMode == TSetProp::hsl) {
				CColor_hsl targetColor_hsl = RGB_toHSL(FGcolor);
				colorActive_hsl.mixColor(&targetColor_hsl, colorMixValue);
			}
			else {
				outColor = *FGcolor;
				colorActive = HSL_toRGB(&colorActive_hsl);
				calcNextHSL = false;
				colorActive.mixColor(FGcolor, colorMixValue);
			}
		}
		else
		{
			if (mixColorMode == TSetProp::hsl) {
				colorActive_hsl = RGB_toHSL(&colorActive);
				calcNextHSL = true;
				CColor_hsl targetColor_hsl = RGB_toHSL(FGcolor);
				colorActive_hsl.mixColor(&targetColor_hsl, colorMixValue);
			}
			else {
				colorActive.mixColor(FGcolor, colorMixValue);
			}
		}
	}

	if (enableFinalColor)
	{
		// Calculate the color HSV/RGB control/noise value
		// Hue and Red Value
		float RedHueControl = 1.0f; float GreenSatControl = 1.0f; float BlueValControl = 1.0f;
		bool redEnabled = false; bool greenEnabled = false; bool blueEnabled = false;
		if (finalHueRedNode.reportEnabled()) {
			redEnabled = true;
			finalHueRedNode.calculateControl(RedHueControl, pos, dir, input, &finalController, anchorCount);
		}
		if (finalHueRedNoise.isEnabled) {
			finalHueRedNoise.calculateNoise(RedHueControl, pos, origin, dir, input, shardCount, anchorCount, splineCount, false);
		}
		if (!redEnabled && !finalHueRedNoise.isEnabled) { RedHueControl = 0.0f; }
		RedHueControl = lerpf(finalHueRedNode.controlMin, finalHueRedNode.controlMax, RedHueControl);
		// Saturation and Green Value
		if (finalSatGreenNode.reportEnabled()) {
			greenEnabled = true;
			finalSatGreenNode.calculateControl(GreenSatControl, pos, dir, input, &finalController, anchorCount);
		}
		if (finalSatGreenNoise.isEnabled) {
			finalSatGreenNoise.calculateNoise(GreenSatControl, pos, origin, dir, input, shardCount, anchorCount, splineCount, false);
		}
		if (!greenEnabled && !finalSatGreenNoise.isEnabled) { GreenSatControl = 0.0f; }
		GreenSatControl = lerpf(finalSatGreenNode.controlMin, finalSatGreenNode.controlMax, GreenSatControl);
		// Value and Blue Value
		if (finalValBlueNode.reportEnabled()) {
			blueEnabled = true;
			finalValBlueNode.calculateControl(BlueValControl, pos, dir, input, &finalController, anchorCount);
		}
		if (finalValBlueNoise.isEnabled) {
			finalValBlueNoise.calculateNoise(BlueValControl, pos, origin, dir, input, shardCount, anchorCount, splineCount, false);
		}
		if (!blueEnabled && !finalValBlueNoise.isEnabled) { BlueValControl = 0.0f; }
		BlueValControl = lerpf(finalValBlueNode.controlMin, finalValBlueNode.controlMax, BlueValControl);

		// Apply Each Control Control/Noise for Active Color (Mix Color)
		if (finalTypeMode == TSetProp::hsl) {
			if (calcNextHSL) {
				colorActive_hsl.h = clampAngle_360((float)colorActive_hsl.h + (RedHueControl * 180.0f));
				colorActive_hsl.s = clampd(colorActive_hsl.s + (double)GreenSatControl, 0.0, 1.0);
				colorActive_hsl.l = clampd(colorActive_hsl.l + (double)BlueValControl, 0.0, 1.0);
			}
			else {
				colorActive_hsl = RGB_toHSL(&colorActive);
				calcNextHSL = true;
				colorActive_hsl.h = clampAngle_360((float)colorActive_hsl.h + (RedHueControl * 180.0f));
				colorActive_hsl.s = clampd(colorActive_hsl.s + (double)GreenSatControl, 0.0, 1.0);
				colorActive_hsl.l = clampd(colorActive_hsl.l + (double)BlueValControl, 0.0, 1.0);
			}
		}
		else if (finalTypeMode == TSetProp::rgb) {
			if (calcNextHSL) {
				colorActive = HSL_toRGB(&colorActive_hsl);
				calcNextHSL = false;
				colorActive.r = clampd(colorActive.r + (double)RedHueControl, 0.0, 1.0);
				colorActive.g = clampd(colorActive.g + (double)GreenSatControl, 0.0, 1.0);
				colorActive.b = clampd(colorActive.b + (double)BlueValControl, 0.0, 1.0);
			}
			else {
				colorActive.r = clampd(colorActive.r + (double)RedHueControl, 0.0, 1.0);
				colorActive.g = clampd(colorActive.g + (double)GreenSatControl, 0.0, 1.0);
				colorActive.b = clampd(colorActive.b + (double)BlueValControl, 0.0, 1.0);
			}
		}
	}
		
	if (calcNextHSL) { outColor = HSL_toRGB(&colorActive_hsl); }
	else { outColor = colorActive; }
}