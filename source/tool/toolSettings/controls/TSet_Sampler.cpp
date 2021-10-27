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

#include "../../../include/tool/toolSettings/controls/TSet_Sampler.h"

////////////////////////////////////////////////////////////////
// Sampler Settings
////////////////////////////////////////////////////////////////

TSet_Sampler::TSet_Sampler()
{
	this->type = TSetType::sampler;
	this->isEnabled = true;
	// Sampler Shape Type & Settings
	this->sampleType = TSetProp::allLayers; // allLayers / currentLayer / currentBelow
	this->shapeType = TSetProp::point; // Pothis->/ Radius / Shape / Tip
	// Sample Background Color
	this->sampleBGModKey = Keybind(InputKey::unknown, InputModKey::alt);
	// Block Callback Notifications?
	this->shouldNotify = true;
}
std::shared_ptr<CObject> TSet_Sampler::putProperty(
	std::shared_ptr<CInterpreter> interpreter, std::shared_ptr<Tool> owner,
	int settingsSig, int subSig, std::shared_ptr<CObject> object, bool isGet, bool asPercentage, bool asString)
{
	if (settingsSig >= 900 && settingsSig < 920)
	{
		if (settingsSig == 900) {
			if (!isGet) { sampleType = stringToTSetProp(std::get<std::string>(object.get()->obj)); }
			else {
				if (asString) { return std::make_shared<CObject>(makeSettingString(sampleType, settingsSig, subSig, "sampleType")); }
				return std::make_shared<CObject>(TSetPropToString(sampleType));
			}
		}
		else if (settingsSig == 901) {
			if (!isGet) { shapeType = stringToTSetProp(std::get<std::string>(object.get()->obj)); }
			else {
				if (asString) { return std::make_shared<CObject>(makeSettingString(shapeType, settingsSig, subSig, "shapeType")); }
				return std::make_shared<CObject>(TSetPropToString(shapeType));
			}
		}
		else if (settingsSig == 902) {
			if (!isGet) {
				if (asPercentage) {
					radius = (int)std::round(
						percentRange_cubed((float)std::get<double>(object.get()->obj), 1.0f, 64.0f, false));
				}
				else {
					radius = clampi(
						(int)std::round(std::get<double>(object.get()->obj)), 1.0f, 64.0f);
				}
			}
			else {
				if (asString) { return std::make_shared<CObject>(makeSettingString(radius, settingsSig, subSig, "radius")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange_cubed((float)radius, 1.0f, 64.0f, true)));
				}
				else { return std::make_shared<CObject>(double(radius)); }
			}
		}
	}
	else if (settingsSig >= 920 && settingsSig < 940)
	{
		if (settingsSig == 928) {
			if (isGet) {
				if (asString) { return std::make_shared<CObject>(makeSettingString(sampleBGModKey, settingsSig, subSig, "sampleBGModKey")); }
				return std::make_shared<CObject>(double(keybindToInt(sampleBGModKey)));
			}
			else {
				if (object.get()->objType.type == CLiteralTypes::_CString) {
					sampleBGModKey = stringToKeybind(std::get<std::string>(object.get()->obj));
				}
				else {
					sampleBGModKey = intToKeybind((int)std::get<double>(object.get()->obj));
				}
			}
		}
		else if (settingsSig == 929) {
			if (!isGet) { useFGVariance = std::get<bool>(object.get()->obj); }
			else {
				if (asString) { return std::make_shared<CObject>(makeSettingString(useFGVariance, settingsSig, subSig, "useFGVariance")); }
				return std::make_shared<CObject>(useFGVariance);
			}
		}
		else if (settingsSig == 930)
		{
			return FGHueRedNode.putProperty(interpreter, owner, settingsSig, subSig, object, isGet, asPercentage, asString);
		}
		else if (settingsSig == 931)
		{
			return FGSatGreenNode.putProperty(interpreter, owner, settingsSig, subSig, object, isGet, asPercentage, asString);
		}
		else if (settingsSig == 932)
		{
			return FGValBlueNode.putProperty(interpreter, owner, settingsSig, subSig, object, isGet, asPercentage, asString);
		}
		else if (settingsSig == 933)
		{
			return FGController.putProperty(interpreter, owner, settingsSig, subSig, object, isGet, asPercentage, asString);
		}
		else if (settingsSig == 934)
		{
			return FGHueRedNoise.putProperty(interpreter, owner, settingsSig, subSig, object, isGet, asPercentage, asString);
		}
		else if (settingsSig == 935)
		{
			return FGSatGreenNoise.putProperty(interpreter, owner, settingsSig, subSig, object, isGet, asPercentage, asString);
		}
		else if (settingsSig == 936)
		{
			return FGValBlueNoise.putProperty(interpreter, owner, settingsSig, subSig, object, isGet, asPercentage, asString);
		}
		else if (settingsSig == 937) {
			if (!isGet) { FGTypeMode = stringToTSetProp(std::get<std::string>(object.get()->obj)); }
			else {
				if (asString) { return std::make_shared<CObject>(makeSettingString(FGTypeMode, settingsSig, subSig, "FGTypeMode")); }
				return std::make_shared<CObject>(TSetPropToString(FGTypeMode));
			}
		}
	}
	else if (settingsSig >= 940 && settingsSig < 960)
	{
		if (settingsSig == 940) {
			if (!isGet) { useAutoBGUpdater = std::get<bool>(object.get()->obj); }
			else {
				if (asString) { return std::make_shared<CObject>(makeSettingString(useAutoBGUpdater, settingsSig, subSig, "useAutoBGUpdater")); }
				return std::make_shared<CObject>(useAutoBGUpdater);
			}
		}
		else if (settingsSig == 941) {
			if (!isGet) {
				if (asPercentage) {
					bgHueRedOffset = percentRange_squaredCenter(
						(float)std::get<double>(object.get()->obj), -1.0f, 1.0f, false);
				}
				else { bgHueRedOffset = clampf((float)std::get<double>(object.get()->obj), -1.0f, 1.0f); }
			}
			else {
				if (asString) { return std::make_shared<CObject>(makeSettingString(bgHueRedOffset, settingsSig, subSig, "bgHueOffset")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange_squaredCenter(bgHueRedOffset, -1.0f, 1.0f, true)));
				}
				else { return std::make_shared<CObject>(double(bgHueRedOffset)); }
			}
		}
		else if (settingsSig == 942) {
			if (!isGet) {
				if (asPercentage) {
					bgSatGreenOffset = percentRange_squaredCenter(
						(float)std::get<double>(object.get()->obj), -1.0, 1.0f, false);
				}
				else { bgSatGreenOffset = clampf((float)std::get<double>(object.get()->obj), -1.0f, 1.0f); }
			}
			else {
				if (asString) { return std::make_shared<CObject>(makeSettingString(bgSatGreenOffset, settingsSig, subSig, "bgSatGreenOffset")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange_squaredCenter(bgSatGreenOffset, -1.0f, 1.0f, true)));
				}
				else { return std::make_shared<CObject>(double(bgSatGreenOffset)); }
			}
		}
		else if (settingsSig == 943) {
			if (!isGet) {
				if (asPercentage) {
					bgValBlueOffset = percentRange_squaredCenter(
						(float)std::get<double>(object.get()->obj), -1.0, 1.0f, false);
				}
				else { bgValBlueOffset = clampf((float)std::get<double>(object.get()->obj), -1.0f, 1.0f); }
			}
			else {
				if (asString) { return std::make_shared<CObject>(makeSettingString(bgValBlueOffset, settingsSig, subSig, "bgValBlueOffset")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange_squaredCenter(bgValBlueOffset, -1.0f, 1.0f, true)));
				}
				else { return std::make_shared<CObject>(double(bgValBlueOffset)); }
			}
		}
		else if (settingsSig == 944) {
			if (!isGet) { useModulatedFG = std::get<bool>(object.get()->obj); }
			else {
				if (asString) { return std::make_shared<CObject>(makeSettingString(useModulatedFG, settingsSig, subSig, "useModulatedFG")); }
				return std::make_shared<CObject>(useModulatedFG);
			}
		}
		else if (settingsSig == 950)
		{
			return BGHueRedNode.putProperty(interpreter, owner, settingsSig, subSig, object, isGet, asPercentage, asString);
		}
		else if (settingsSig == 951)
		{
			return BGSatGreenNode.putProperty(interpreter, owner, settingsSig, subSig, object, isGet, asPercentage, asString);
		}
		else if (settingsSig == 952)
		{
			return BGValBlueNode.putProperty(interpreter, owner, settingsSig, subSig, object, isGet, asPercentage, asString);
		}
		else if (settingsSig == 953)
		{
			return BGController.putProperty(interpreter, owner, settingsSig, subSig, object, isGet, asPercentage, asString);
		}
		else if (settingsSig == 954)
		{
			return BGHueRedNoise.putProperty(interpreter, owner, settingsSig, subSig, object, isGet, asPercentage, asString);
		}
		else if (settingsSig == 955)
		{
			return BGSatGreenNoise.putProperty(interpreter, owner, settingsSig, subSig, object, isGet, asPercentage, asString);
		}
		else if (settingsSig == 956)
		{
			return BGValBlueNoise.putProperty(interpreter, owner, settingsSig, subSig, object, isGet, asPercentage, asString);
		}
		else if (settingsSig == 957) {
			if (!isGet) { BGTypeMode = stringToTSetProp(std::get<std::string>(object.get()->obj)); }
			else {
				if (asString) { return std::make_shared<CObject>(makeSettingString(BGTypeMode, settingsSig, subSig, "BGTypeMode")); }
				return std::make_shared<CObject>(TSetPropToString(BGTypeMode));
			}
		}
	}
	else if (settingsSig >= 960 && settingsSig < 970)
	{
		if (settingsSig == 960) {
			if (!isGet) { usePaletteConstrain = std::get<bool>(object.get()->obj); }
			else {
				if (asString) { return std::make_shared<CObject>(makeSettingString(usePaletteConstrain, settingsSig, subSig, "usePaletteConstrain")); }
				return std::make_shared<CObject>(usePaletteConstrain);
			}
		}
		else if (settingsSig == 961) {
			if (!isGet) { paletteType = stringToTSetProp(std::get<std::string>(object.get()->obj)); }
			else {
				if (asString) { return std::make_shared<CObject>(makeSettingString(paletteType, settingsSig, subSig, "paletteType")); }
				return std::make_shared<CObject>(TSetPropToString(paletteType));
			}
		}
		else if (settingsSig == 962) {
			if (!isGet) {
				if (asPercentage) {
					hueMin = percentRange(
						(float)std::get<double>(object.get()->obj), 0.0f, 1.0f, false);
				}
				else { hueMin = clampf((float)std::get<double>(object.get()->obj), 0.0f, 1.0f); }
			}
			else {
				if (asString) { return std::make_shared<CObject>(makeSettingString(hueMin, settingsSig, subSig, "hueMin")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange(hueMin, 0.0f, 360.0f, true)));
				}
				else { return std::make_shared<CObject>(double(hueMin)); }
			}
		}
		else if (settingsSig == 963) {
			if (!isGet) {
				if (asPercentage) {
					hueMax = percentRange(
						(float)std::get<double>(object.get()->obj), 0.0f, 1.0f, false);
				}
				else { hueMax = clampf((float)std::get<double>(object.get()->obj), 0.0f, 1.0f); }
			}
			else {
				if (asString) { return std::make_shared<CObject>(makeSettingString(hueMax, settingsSig, subSig, "hueMax")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange(hueMax, 0.0f, 360.0f, true)));
				}
				else { return std::make_shared<CObject>(double(hueMax)); }
			}
		}
		else if (settingsSig == 964) {
			if (!isGet) {
				if (asPercentage) {
					saturationMin = percentRange(
						(float)std::get<double>(object.get()->obj), 0.0f, 1.0f, false);
				}
				else { saturationMin = clampf((float)std::get<double>(object.get()->obj), 0.0f, 1.0f); }
			}
			else {
				if (asString) { return std::make_shared<CObject>(makeSettingString(saturationMin, settingsSig, subSig, "saturationMin")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange(saturationMin, 0.0f, 1.0f, true)));
				}
				else { return std::make_shared<CObject>(double(saturationMin)); }
			}
		}
		else if (settingsSig == 965) {
			if (!isGet) {
				if (asPercentage) {
					saturationMax = percentRange(
						(float)std::get<double>(object.get()->obj), 0.0f, 1.0f, false);
				}
				else { saturationMax = clampf((float)std::get<double>(object.get()->obj), 0.0f, 1.0f); }
			}
			else {
				if (asString) { return std::make_shared<CObject>(makeSettingString(saturationMax, settingsSig, subSig, "saturationMax")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange(saturationMax, 0.0f, 1.0f, true)));
				}
				else { return std::make_shared<CObject>(double(saturationMax)); }
			}
		}
		else if (settingsSig == 966) {
			if (!isGet) {
				if (asPercentage) {
					valueMin = percentRange(
						(float)std::get<double>(object.get()->obj), 0.0f, 1.0f, false);
				}
				else { valueMin = clampf((float)std::get<double>(object.get()->obj), 0.0f, 1.0f); }
			}
			else {
				if (asString) { return std::make_shared<CObject>(makeSettingString(valueMin, settingsSig, subSig, "valueMin")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange(valueMin, 0.0f, 1.0f, true)));
				}
				else { return std::make_shared<CObject>(double(valueMin)); }
			}
		}
		else if (settingsSig == 967) {
			if (!isGet) {
				if (asPercentage) {
					valueMax = percentRange(
						(float)std::get<double>(object.get()->obj), 0.0f, 1.0f, false);
				}
				else { valueMax = clampf((float)std::get<double>(object.get()->obj), 0.0f, 1.0f); }
			}
			else {
				if (asString) { return std::make_shared<CObject>(makeSettingString(valueMax, settingsSig, subSig, "valueMax")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange(valueMax, 0.0f, 1.0f, true)));
				}
				else { return std::make_shared<CObject>(double(valueMax)); }
			}
		}
		else if (settingsSig == 968) {
			if (!isGet) { simplePaletteTypeMode = stringToTSetProp(std::get<std::string>(object.get()->obj)); }
			else {
				if (asString) { return std::make_shared<CObject>(makeSettingString(simplePaletteTypeMode, settingsSig, subSig, "simplePaletteTypeMode")); }
				return std::make_shared<CObject>(TSetPropToString(simplePaletteTypeMode));
			}
		}
	}
	if (asString) { return std::make_shared<CObject>(makeSettingString(0.0f, settingsSig, subSig, "NULL")); }
	return std::make_shared<CObject>(nullptr);
}

TSetControl_Node* TSet_Sampler::getControlNode(int settingSig, int subSig)
{
	if (settingSig == 930) { return &FGHueRedNode; }
	else if (settingSig == 931) { return &FGSatGreenNode; }
	else if (settingSig == 932) { return &FGValBlueNode; }
	else if (settingSig == 950) { return &BGHueRedNode; }
	else if (settingSig == 951) { return &BGSatGreenNode; }
	else if (settingSig == 952) { return &BGValBlueNode; }
	return nullptr;
}
TSetController* TSet_Sampler::getController(int settingSig, int subSig)
{
	if (settingSig == 933) { return &FGController; }
	else if (settingSig == 953) { return &BGController; }
	return nullptr;
}
TSetGraph* TSet_Sampler::getGraph(int settingSig, int subSig)
{
	if (settingSig == 933)
	{
		if (subSig == -1) { return &FGController.pressureGraph; }
		else if (subSig == -2) { return &FGController.directionGraph; }
		else if (subSig == -3) { return &FGController.tiltGraph; }
		else if (subSig == -4) { return &FGController.velocityGraph; }
		else if (subSig == -5) { return &FGController.rotationGraph; }
	}
	if (settingSig == 953)
	{
		if (subSig == -1) { return &BGController.pressureGraph; }
		else if (subSig == -2) { return &BGController.directionGraph; }
		else if (subSig == -3) { return &BGController.tiltGraph; }
		else if (subSig == -4) { return &BGController.velocityGraph; }
		else if (subSig == -5) { return &BGController.rotationGraph; }
	}
	return nullptr;
}
TSetNoise* TSet_Sampler::getNoise(int settingSig, int subSig)
{
	if (settingSig == 934) { return &FGHueRedNoise; }
	if (settingSig == 935) { return &FGSatGreenNoise; }
	if (settingSig == 936) { return &FGValBlueNoise; }
	if (settingSig == 954) { return &BGHueRedNoise; }
	if (settingSig == 955) { return &BGSatGreenNoise; }
	if (settingSig == 956) { return &BGValBlueNoise; }
	return nullptr;
}

void TSet_Sampler::initialize(float* trueSpacing, int* entityCount, glm::ivec2* canvasDimensions)
{
	//
	if (FGHueRedNoise.isEnabled) { FGHueRedNoise.initialize(trueSpacing, entityCount, canvasDimensions); }
	if (FGSatGreenNoise.isEnabled) { FGSatGreenNoise.initialize(trueSpacing, entityCount, canvasDimensions); }
	if (FGValBlueNoise.isEnabled) { FGValBlueNoise.initialize(trueSpacing, entityCount, canvasDimensions); }
	//
	if (BGHueRedNoise.isEnabled) { BGHueRedNoise.initialize(trueSpacing, entityCount, canvasDimensions); }
	if (BGSatGreenNoise.isEnabled) { BGSatGreenNoise.initialize(trueSpacing, entityCount, canvasDimensions); }
	if (BGValBlueNoise.isEnabled) { BGValBlueNoise.initialize(trueSpacing, entityCount, canvasDimensions); }
}

void TSet_Sampler::modulateColor(CColor& outColorFG, CColor& outColorBG, CColor* inColor,
	glm::vec3* pos, glm::vec3* origin, glm::vec3* dir, Input* input,
	int* shardCount, int* anchorCount, int* splineCount)
{
	if (useFGVariance)
	{
		// Calculate the color HSV/RGB control/noise value
		// Hue and Red Value
		float RedHueControl = 1.0f; float GreenSatControl = 1.0f; float BlueValControl = 1.0f;
		bool redEnabled = false; bool greenEnabled = false; bool blueEnabled = false;
		if (FGHueRedNode.reportEnabled()) {
			redEnabled = true;
			FGHueRedNode.calculateControl(RedHueControl, pos, dir, input, &FGController, anchorCount);
		}
		if (FGHueRedNoise.isEnabled) {
			FGHueRedNoise.calculateNoise(RedHueControl, pos, origin, dir, input, shardCount, anchorCount, splineCount, false);
		}
		if (!redEnabled && !FGHueRedNoise.isEnabled) { RedHueControl = 0.0f; }
		RedHueControl = lerpf(FGHueRedNode.controlMin, FGHueRedNode.controlMax, RedHueControl);
		// Saturation and Green Value
		if (FGSatGreenNode.reportEnabled()) {
			greenEnabled = true;
			FGSatGreenNode.calculateControl(GreenSatControl, pos, dir, input, &FGController, anchorCount);
		}
		if (FGSatGreenNoise.isEnabled) {
			FGSatGreenNoise.calculateNoise(GreenSatControl, pos, origin, dir, input, shardCount, anchorCount, splineCount, false);
		}
		if (!greenEnabled && !FGSatGreenNoise.isEnabled) { GreenSatControl = 0.0f; }
		GreenSatControl = lerpf(FGSatGreenNode.controlMin, FGSatGreenNode.controlMax, GreenSatControl);
		// Value and Blue Value
		if (FGValBlueNode.reportEnabled()) {
			blueEnabled = true;
			FGValBlueNode.calculateControl(BlueValControl, pos, dir, input, &FGController, anchorCount);
		}
		if (FGValBlueNoise.isEnabled) {
			FGValBlueNoise.calculateNoise(BlueValControl, pos, origin, dir, input, shardCount, anchorCount, splineCount, false);
		}
		if (!blueEnabled && !FGValBlueNoise.isEnabled) { BlueValControl = 0.0f; }
		BlueValControl = lerpf(FGValBlueNode.controlMin, FGValBlueNode.controlMax, BlueValControl);

		// Apply Each Control Control/Noise for Active Color (Mix Color)
		if (FGTypeMode == TSetProp::hsl) {
			CColor_hsl fgColorHSL = RGB_toHSL(inColor);
			fgColorHSL.h = clampAngle_360d((double)fgColorHSL.h + ((double)bgHueRedOffset * 180.0) + ((double)RedHueControl * 180.0));
			fgColorHSL.s = clampd((double)fgColorHSL.s + (double)bgSatGreenOffset + (double)GreenSatControl, 0.0, 1.0);
			fgColorHSL.l = clampd((double)fgColorHSL.l + (double)bgValBlueOffset + (double)BlueValControl, 0.0, 1.0);
			outColorFG = HSL_toRGB(&fgColorHSL);
		}
		else if (FGTypeMode == TSetProp::rgb) {
			CColor fgColorRGB = outColorFG;
			fgColorRGB.r = clampd((double)fgColorRGB.r + (double)bgHueRedOffset + (double)RedHueControl, 0.0, 1.0);
			fgColorRGB.g = clampd((double)fgColorRGB.g + (double)bgSatGreenOffset + (double)GreenSatControl, 0.0, 1.0);
			fgColorRGB.b = clampd((double)fgColorRGB.b + (double)bgValBlueOffset + (double)BlueValControl, 0.0, 1.0);
			outColorFG = fgColorRGB;
		}
	}

	if (useAutoBGUpdater)
	{
		// Calculate the color HSV/RGB control/noise value
		// Hue and Red Value
		float RedHueControl = 1.0f; float GreenSatControl = 1.0f; float BlueValControl = 1.0f;
		bool redEnabled = false; bool greenEnabled = false; bool blueEnabled = false;
		if (FGHueRedNode.reportEnabled()) {
			redEnabled = true;
			FGHueRedNode.calculateControl(RedHueControl, pos, dir, input, &BGController, anchorCount);
		}
		if (FGHueRedNoise.isEnabled) {
			FGHueRedNoise.calculateNoise(RedHueControl, pos, origin, dir, input, shardCount, anchorCount, splineCount, false);
		}
		if (!redEnabled && !FGHueRedNoise.isEnabled) { RedHueControl = 0.0f; }
		RedHueControl = lerpf(FGHueRedNode.controlMin, FGHueRedNode.controlMax, RedHueControl);
		// Saturation and Green Value
		if (FGSatGreenNode.reportEnabled()) {
			greenEnabled = true;
			FGSatGreenNode.calculateControl(GreenSatControl, pos, dir, input, &BGController, anchorCount);
		}
		if (FGSatGreenNoise.isEnabled) {
			FGSatGreenNoise.calculateNoise(GreenSatControl, pos, origin, dir, input, shardCount, anchorCount, splineCount, false);
		}
		if (!greenEnabled && !FGSatGreenNoise.isEnabled) { GreenSatControl = 0.0f; }
		GreenSatControl = lerpf(FGSatGreenNode.controlMin, FGSatGreenNode.controlMax, GreenSatControl);
		// Value and Blue Value
		if (FGValBlueNode.reportEnabled()) {
			blueEnabled = true;
			FGValBlueNode.calculateControl(BlueValControl, pos, dir, input, &BGController, anchorCount);
		}
		if (FGValBlueNoise.isEnabled) {
			FGValBlueNoise.calculateNoise(BlueValControl, pos, origin, dir, input, shardCount, anchorCount, splineCount, false);
		}
		if (!blueEnabled && !FGValBlueNoise.isEnabled) { BlueValControl = 0.0f; }
		BlueValControl = lerpf(FGValBlueNode.controlMin, FGValBlueNode.controlMax, BlueValControl);

		// Apply Each Control Control/Noise for Active Color (Mix Color)
		if (BGTypeMode == TSetProp::hsl) {
			CColor_hsl bgColorHSL = RGB_toHSL((useModulatedFG && useFGVariance) ? &outColorFG : inColor);
			bgColorHSL.h = clampAngle_360d((double)bgColorHSL.h + ((double)bgHueRedOffset * 180.0) + ((double)RedHueControl * 180.0));
			bgColorHSL.s = clampd((double)bgColorHSL.s + (double)bgSatGreenOffset + (double)GreenSatControl, 0.0, 1.0);
			bgColorHSL.l = clampd((double)bgColorHSL.l + (double)bgValBlueOffset + (double)BlueValControl, 0.0, 1.0);
			outColorBG = HSL_toRGB(&bgColorHSL);
		}
		else if (BGTypeMode == TSetProp::rgb) {
			CColor bgColorRGB = (useModulatedFG && useFGVariance) ? outColorFG : *inColor;
			bgColorRGB.r = clampd((double)bgColorRGB.r + (double)bgHueRedOffset + (double)RedHueControl, 0.0, 1.0);
			bgColorRGB.g = clampd((double)bgColorRGB.g + (double)bgSatGreenOffset + (double)GreenSatControl, 0.0, 1.0);
			bgColorRGB.b = clampd((double)bgColorRGB.b + (double)bgValBlueOffset + (double)BlueValControl, 0.0, 1.0);
			outColorBG = bgColorRGB;
		}
	}

	if (usePaletteConstrain) {
		if (simplePaletteTypeMode == TSetProp::hsl) {
			float hueMax360 = hueMax * 360.0f; float hueMin360 = hueMin * 360.0f;
			float centerPoint = hueMax360 - ((hueMin360 - hueMax360) / 2.0f);
			// FG Color
			CColor_hsl fgColorHSL = RGB_toHSL(&outColorFG);
			if (hueMax360 < hueMin360) {
				if (fgColorHSL.h > hueMin360 || fgColorHSL.h < hueMax360) { fgColorHSL.h = fgColorHSL.h; }
				else if (fgColorHSL.h >= centerPoint) { fgColorHSL.h = hueMin360; }
				else if (fgColorHSL.h <= centerPoint) { fgColorHSL.h = hueMax360; }
			}
			else {
				fgColorHSL.h = clampd(fgColorHSL.h, hueMin360, hueMax360);
			}
			fgColorHSL.s = clampd(fgColorHSL.s, saturationMin, saturationMax);
			fgColorHSL.l = clampd(fgColorHSL.l, valueMin, valueMax);
			outColorFG = HSL_toRGB(&fgColorHSL);

			// BG Color
			CColor_hsl bgColorHSL = RGB_toHSL(&outColorBG);
			if (hueMax360 < hueMin360) {
				if (bgColorHSL.h > hueMin360 || bgColorHSL.h < hueMax360) { bgColorHSL.h = bgColorHSL.h; }
				else if (bgColorHSL.h >= centerPoint) { bgColorHSL.h = hueMin360; }
				else if (bgColorHSL.h <= centerPoint) { bgColorHSL.h = hueMax360; }
			}
			else {
				bgColorHSL.h = clampd(bgColorHSL.h, hueMin360, hueMax360);
			}
			bgColorHSL.s = clampd(bgColorHSL.s, saturationMin, saturationMax);
			bgColorHSL.l = clampd(bgColorHSL.l, valueMin, valueMax);
			outColorBG = HSL_toRGB(&bgColorHSL);
		}
		else if (simplePaletteTypeMode == TSetProp::rgb) {
			// FG Color
			CColor fgColorRGB = outColorFG;
			fgColorRGB.r = clampf(fgColorRGB.r, hueMin, hueMax);
			fgColorRGB.g = clampf(fgColorRGB.g, saturationMin, saturationMax);
			fgColorRGB.b = clampf(fgColorRGB.b, valueMin, valueMax);
			outColorFG = fgColorRGB;

			// BG Color
			CColor bgColorRGB = outColorBG;
			bgColorRGB.r = clampf(bgColorRGB.r, hueMin, hueMax);
			bgColorRGB.g = clampf(bgColorRGB.g, saturationMin, saturationMax);
			bgColorRGB.b = clampf(bgColorRGB.b, valueMin, valueMax);
			outColorBG = bgColorRGB;
		}
	}
}