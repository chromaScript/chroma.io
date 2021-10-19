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

#include "../../../include/tool/toolSettings/effects/TSet_FX_BrightContrast.h"

////////////////////////////////////////////////////////////////
// FX BRIGHTCONTRAST Settings
////////////////////////////////////////////////////////////////

TSet_FX_BrightContrast::TSet_FX_BrightContrast()
{
	this->type = TSetType::fx_threshold;
	this->isEnabled = false;
	this->mixAmount = 1.0f; // 8605
	this->brightnessMin = 0.0f; // 8610
	this->brightnessMax = 0.0f; // 8611
	this->brightnessMix = 1.0f; // 8612
	this->contrastMin = 0.0f; // 8613
	this->contrastMax = 0.0f; // 8614
	this->contrastMix = 1.0f; // 8615
}
std::shared_ptr<CObject> TSet_FX_BrightContrast::putProperty(
	std::shared_ptr<CInterpreter> interpreter, std::shared_ptr<Tool> owner,
	int settingsSig, int subSig, std::shared_ptr<CObject> object, bool isGet, bool asPercentage, bool asString)
{
	if (settingsSig >= 8600 && settingsSig < 8610)
	{
		if (settingsSig == 8605)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(mixAmount, settingsSig, subSig, "mixAmount")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange_cubedInvert(mixAmount, 0.0f, 1.0f, true)));
				}
				else { return std::make_shared<CObject>(double(mixAmount)); }
			}
			else
			{
				if (asPercentage) {
					mixAmount = percentRange_cubedInvert(
						(float)std::get<double>(object.get()->obj), 0.0f, 1.0f, false);
				}
				else { mixAmount = clampf((float)std::get<double>(object.get()->obj), 0.0f, 1.0f); }
			}
		}
	}
	else if (settingsSig >= 8610 && settingsSig < 8620)
	{
		if (settingsSig == 8610)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(brightnessMin, settingsSig, subSig, "brightnessMin")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange_squaredCenter(brightnessMin, TSET_FX_BRIGHT_BRIGHT_MIN, TSET_FX_BRIGHT_BRIGHT_MAX, true)));
				}
				else { return std::make_shared<CObject>(double(brightnessMin)); }
			}
			else
			{
				if (asPercentage) {
					brightnessMin = percentRange_squaredCenter(
						(float)std::get<double>(object.get()->obj), TSET_FX_BRIGHT_BRIGHT_MIN, TSET_FX_BRIGHT_BRIGHT_MAX, false);
				}
				else { brightnessMin = clampf((float)std::get<double>(object.get()->obj), TSET_FX_BRIGHT_BRIGHT_MIN, TSET_FX_BRIGHT_BRIGHT_MAX); }
			}
		}
		else if (settingsSig == 8611)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(brightnessMax, settingsSig, subSig, "brightnessMax")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange_squaredCenter(brightnessMax, TSET_FX_BRIGHT_BRIGHT_MIN, TSET_FX_BRIGHT_BRIGHT_MAX, true)));
				}
				else { return std::make_shared<CObject>(double(brightnessMax)); }
			}
			else
			{
				if (asPercentage) {
					brightnessMax = percentRange_squaredCenter(
						(float)std::get<double>(object.get()->obj), TSET_FX_BRIGHT_BRIGHT_MIN, TSET_FX_BRIGHT_BRIGHT_MAX, false);
				}
				else { brightnessMax = clampf((float)std::get<double>(object.get()->obj), TSET_FX_BRIGHT_BRIGHT_MIN, TSET_FX_BRIGHT_BRIGHT_MAX); }
			}
		}
		else if (settingsSig == 8612)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(brightnessMix, settingsSig, subSig, "brightnessMix")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange_cubedInvert(brightnessMix, 0.0f, 1.0f, true)));
				}
				else { return std::make_shared<CObject>(double(brightnessMix)); }
			}
			else
			{
				if (asPercentage) {
					brightnessMix = percentRange_cubedInvert(
						(float)std::get<double>(object.get()->obj), 0.0f, 1.0f, false);
				}
				else { brightnessMix = clampf((float)std::get<double>(object.get()->obj), 0.0f, 1.0f); }
			}
		}
		else if (settingsSig == 8613)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(contrastMin, settingsSig, subSig, "contrastMin")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange_squaredCenter(contrastMin, TSET_FX_BRIGHT_CONTRAST_MIN, TSET_FX_BRIGHT_CONTRAST_MAX, true)));
				}
				else { return std::make_shared<CObject>(double(contrastMin)); }
			}
			else
			{
				if (asPercentage) {
					contrastMin = percentRange_squaredCenter(
						(float)std::get<double>(object.get()->obj), TSET_FX_BRIGHT_CONTRAST_MIN, TSET_FX_BRIGHT_CONTRAST_MAX, false);
				}
				else { contrastMin = clampf((float)std::get<double>(object.get()->obj), TSET_FX_BRIGHT_CONTRAST_MIN, TSET_FX_BRIGHT_CONTRAST_MAX); }
			}
		}
		else if (settingsSig == 8614)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(contrastMax, settingsSig, subSig, "contrastMax")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange_squaredCenter(contrastMax, TSET_FX_BRIGHT_CONTRAST_MIN, TSET_FX_BRIGHT_CONTRAST_MAX, true)));
				}
				else { return std::make_shared<CObject>(double(contrastMax)); }
			}
			else
			{
				if (asPercentage) {
					contrastMax = percentRange_squaredCenter(
						(float)std::get<double>(object.get()->obj), TSET_FX_BRIGHT_CONTRAST_MIN, TSET_FX_BRIGHT_CONTRAST_MAX, false);
				}
				else { contrastMax = clampf((float)std::get<double>(object.get()->obj), TSET_FX_BRIGHT_CONTRAST_MIN, TSET_FX_BRIGHT_CONTRAST_MAX); }
			}
		}
		else if (settingsSig == 8615)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(contrastMix, settingsSig, subSig, "contrastMix")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange_cubedInvert(contrastMix, 0.0f, 1.0f, true)));
				}
				else { return std::make_shared<CObject>(double(contrastMix)); }
			}
			else
			{
				if (asPercentage) {
					contrastMix = percentRange_cubedInvert(
						(float)std::get<double>(object.get()->obj), 0.0f, 1.0f, false);
				}
				else { contrastMix = clampf((float)std::get<double>(object.get()->obj), 0.0f, 1.0f); }
			}
		}
	}
	if (asString) { return std::make_shared<CObject>(makeSettingString(isEnabled, settingsSig, subSig, "NULL")); }
	return std::make_shared<CObject>(nullptr);
}

TSetControl_Node* TSet_FX_BrightContrast::getControlNode(int settingSig, int subSig)
{
	return nullptr;
}
TSetController* TSet_FX_BrightContrast::getController(int settingSig, int subSig)
{
	return nullptr;
}
TSetGraph* TSet_FX_BrightContrast::getGraph(int settingSig, int subSig)
{
	return nullptr;
}
TSetNoise* TSet_FX_BrightContrast::getNoise(int settingSig, int subSig)
{
	return nullptr;
}

void TSet_FX_BrightContrast::initializeData(CColor FGColor, CColor BGColor)
{
	std::random_device device;
	std::default_random_engine eng(device());
	std::uniform_real_distribution<float> distr(0, 1);

	finalBrightness = clampf(lerpf(brightnessMin, brightnessMax, distr(eng)), -1.0f, 1.0f);
	finalContrast = clampf(lerpf(contrastMin, contrastMax, distr(eng)), -1.0f, 1.0f);
}