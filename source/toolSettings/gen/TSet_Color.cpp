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
#include "../../include/toolSettings/gen/TSet_Color.h"

////////////////////////////////////////////////////////////////
// Color Dynamics
////////////////////////////////////////////////////////////////

TSet_Color::TSet_Color()
{
	this->isEnabled = false;
	enableMixBG = false; // 2700
	enableBGDynamics = false; // 2710
	BGHue_min = 0.0f; // 2713
	BGHue_max = 0.0f; // 2714
	BGHue_bias = 0.0f; // 2715
	BGSat_min = 0.0f; // 2716
	BGSat_max = 0.0f; // 2717
	BGSat_bias = 0.0f; // 2718
	BGValue_min = 0.0f; // 2719
	BGValue_max = 0.0f; // 2720
	BGValue_bias = 0.0f; // 2721
	enableFinalDynamics = false; // 2730
	finalHue_min = 0.0f; // 2733
	finalHue_max = 0.0f; // 2734
	finalHue_bias = 0.0f; // 2735
	finalSat_min = 0.0f; // 2736
	finalSat_max = 0.0f; // 2737
	finalSat_bias = 0.0f; // 2738
	finalValue_min = 0.0f; // 2739
	finalValue_max = 0.0f; // 2740
	finalValue_bias = 0.0f; // 2741
	useRGB_BG = false; // 2750
	bgRed_min = 0.0f; // 2751
	bgRed_max = 0.0f; // 2752
	bgGreen_min = 0.0f; // 2753
	bgGreen_max = 0.0f; // 2754
	bgBlue_min = 0.0f; // 2755
	bgBlue_max = 0.0f; //2756
	useRGB_FG = false; // 2760
	finalRed_min = 0.0f; // 2761
	finalRed_max = 0.0f; // 2762
	finalGreen_min = 0.0f; // 2763
	finalGreen_max = 0.0f; // 2764
	finalBlue_min = 0.0f; // 2765
	finalBlue_max = 0.0f; //2766
	this->type = TSetType::color;
}
std::shared_ptr<CObject> TSet_Color::putProperty(
	std::shared_ptr<CInterpreter> interpreter, std::shared_ptr<Tool> owner,
	int settingsSig, int subSig, std::shared_ptr<CObject> object, bool isGet, bool asPercentage, bool asString)
{
	// Mix FG/BG Settings
	if (settingsSig >= 2700 && settingsSig <= 2709)
	{
		if (settingsSig == 2700)
		{
			if (!isGet)
			{
				enableMixBG = std::get<bool>(object.get()->obj);
			}
			else
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(enableMixBG, settingsSig, subSig, "enableMixBG")); }
				return std::make_shared<CObject>(enableMixBG);
			}
		}
	}
	// BG Color Dynamics
	else if (settingsSig >= 2710 && settingsSig <= 2729)
	{
		switch (settingsSig)
		{
		case 2710:
			if (!isGet)
			{
				enableBGDynamics = std::get<bool>(object.get()->obj);
			}
			else
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(enableBGDynamics, settingsSig, subSig, "enableBGDynamics")); }
				return std::make_shared<CObject>(enableBGDynamics);
			}
			break;
		case 2713:
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(BGHue_min, settingsSig, subSig, "BGHue_min")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange_squaredCenter(BGHue_min, TSET_COLOR_HUE_MIN, TSET_COLOR_HUE_MAX, true)));
				}
				else { return std::make_shared<CObject>(double(BGHue_min)); }
			}
			else
			{
				if (asPercentage) {
					BGHue_min = percentRange_squaredCenter(
						(float)std::get<double>(object.get()->obj), TSET_COLOR_HUE_MIN, TSET_COLOR_HUE_MAX, false);
				}
				else { BGHue_min = (float)std::get<double>(object.get()->obj); }
			} break;
		case 2714:
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(BGHue_max, settingsSig, subSig, "BGHue_max")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange_squaredCenter(BGHue_max, TSET_COLOR_HUE_MIN, TSET_COLOR_HUE_MAX, true)));
				}
				else { return std::make_shared<CObject>(double(BGHue_max)); }
			}
			else
			{
				if (asPercentage) {
					BGHue_max = percentRange_squaredCenter(
						(float)std::get<double>(object.get()->obj), TSET_COLOR_HUE_MIN, TSET_COLOR_HUE_MAX, false);
				}
				else { BGHue_max = (float)std::get<double>(object.get()->obj); }
			} break;
		case 2715:
			if (!isGet)
			{
				BGHue_bias = (float)std::get<double>(object.get()->obj);
			}
			else
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(BGHue_bias, settingsSig, subSig, "BGHue_bias")); }
				return std::make_shared<CObject>((double)BGHue_bias);
			}
			break;
		case 2716:
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(BGSat_min, settingsSig, subSig, "BGSat_min")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange_squaredCenter(BGSat_min, TSET_COLOR_SAT_MIN, TSET_COLOR_SAT_MAX, true)));
				}
				else { return std::make_shared<CObject>(double(BGSat_min)); }
			}
			else
			{
				if (asPercentage) {
					BGSat_min = percentRange_squaredCenter(
						(float)std::get<double>(object.get()->obj), TSET_COLOR_SAT_MIN, TSET_COLOR_SAT_MAX, false);
				}
				else { BGSat_min = (float)std::get<double>(object.get()->obj); }
			} break;
		case 2717:
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(BGSat_max, settingsSig, subSig, "BGSat_max")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange_squaredCenter(BGSat_max, TSET_COLOR_SAT_MIN, TSET_COLOR_SAT_MAX, true)));
				}
				else { return std::make_shared<CObject>(double(BGSat_max)); }
			}
			else
			{
				if (asPercentage) {
					BGSat_max = percentRange_squaredCenter(
						(float)std::get<double>(object.get()->obj), TSET_COLOR_SAT_MIN, TSET_COLOR_SAT_MAX, false);
				}
				else { BGSat_max = (float)std::get<double>(object.get()->obj); }
			} break;
		case 2718:
			if (!isGet)
			{
				BGSat_bias = (float)std::get<double>(object.get()->obj);
			}
			else
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(BGSat_bias, settingsSig, subSig, "BGSat_bias")); }
				return std::make_shared<CObject>((double)BGSat_bias);
			}
			break;
		case 2719:
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(BGValue_min, settingsSig, subSig, "BGValue_min")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange_squaredCenter(BGValue_min, TSET_COLOR_LUM_MIN, TSET_COLOR_LUM_MAX, true)));
				}
				else { return std::make_shared<CObject>(double(BGValue_min)); }
			}
			else
			{
				if (asPercentage) {
					BGValue_min = percentRange_squaredCenter(
						(float)std::get<double>(object.get()->obj), TSET_COLOR_LUM_MIN, TSET_COLOR_LUM_MAX, false);
				}
				else { BGValue_min = (float)std::get<double>(object.get()->obj); }
			} break;
		case 2720:
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(BGValue_max, settingsSig, subSig, "BGValue_max")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange_squaredCenter(BGValue_max, TSET_COLOR_LUM_MIN, TSET_COLOR_LUM_MAX, true)));
				}
				else { return std::make_shared<CObject>(double(BGValue_max)); }
			}
			else
			{
				if (asPercentage) {
					BGValue_max = percentRange_squaredCenter(
						(float)std::get<double>(object.get()->obj), TSET_COLOR_LUM_MIN, TSET_COLOR_LUM_MAX, false);
				}
				else { BGValue_max = (float)std::get<double>(object.get()->obj); }
			} break;
		case 2721:
			if (!isGet)
			{
				BGValue_bias = (float)std::get<double>(object.get()->obj);
			}
			else
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(BGValue_bias, settingsSig, subSig, "BGValue_bias")); }
				return std::make_shared<CObject>((double)BGValue_bias);
			}
			break;
		}
	}
	// Final Color Dynamics
	else if (settingsSig >= 2730 && settingsSig <= 2749)
	{
		switch (settingsSig)
		{
		case 2730:
			if (!isGet)
			{
				enableFinalDynamics = std::get<bool>(object.get()->obj);
			}
			else
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(enableFinalDynamics, settingsSig, subSig, "enableFinalDynamics")); }
				return std::make_shared<CObject>(enableFinalDynamics);
			}
			break;
		case 2733:
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(finalHue_min, settingsSig, subSig, "finalHue_min")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange_squaredCenter(finalHue_min, TSET_COLOR_HUE_MIN, TSET_COLOR_HUE_MAX, true)));
				}
				else { return std::make_shared<CObject>(double(finalHue_min)); }
			}
			else
			{
				if (asPercentage) {
					finalHue_min = percentRange_squaredCenter(
						(float)std::get<double>(object.get()->obj), TSET_COLOR_HUE_MIN, TSET_COLOR_HUE_MAX, false);
				}
				else { finalHue_min = (float)std::get<double>(object.get()->obj); }
			} break;
		case 2734:
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(finalHue_max, settingsSig, subSig, "finalHue_max")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange_squaredCenter(finalHue_max, TSET_COLOR_HUE_MIN, TSET_COLOR_HUE_MAX, true)));
				}
				else { return std::make_shared<CObject>(double(finalHue_max)); }
			}
			else
			{
				if (asPercentage) {
					finalHue_max = percentRange_squaredCenter(
						(float)std::get<double>(object.get()->obj), TSET_COLOR_HUE_MIN, TSET_COLOR_HUE_MAX, false);
				}
				else { finalHue_max = (float)std::get<double>(object.get()->obj); }
			} break;
		case 2735:
			if (!isGet)
			{
				finalHue_bias = (float)std::get<double>(object.get()->obj);
			}
			else
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(finalHue_bias, settingsSig, subSig, "finalHue_bias")); }
				return std::make_shared<CObject>((double)finalHue_bias);
			}
			break;
		case 2736:
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(finalSat_min, settingsSig, subSig, "finalSat_min")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange_squaredCenter(finalSat_min, TSET_COLOR_SAT_MIN, TSET_COLOR_SAT_MAX, true)));
				}
				else { return std::make_shared<CObject>(double(finalSat_min)); }
			}
			else
			{
				if (asPercentage) {
					finalSat_min = percentRange_squaredCenter(
						(float)std::get<double>(object.get()->obj), TSET_COLOR_SAT_MIN, TSET_COLOR_SAT_MAX, false);
				}
				else { finalSat_min = (float)std::get<double>(object.get()->obj); }
			} break;
		case 2737:
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(finalSat_max, settingsSig, subSig, "finalSat_max")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange_squaredCenter(finalSat_max, TSET_COLOR_SAT_MIN, TSET_COLOR_SAT_MAX, true)));
				}
				else { return std::make_shared<CObject>(double(finalSat_max)); }
			}
			else
			{
				if (asPercentage) {
					finalSat_max = percentRange_squaredCenter(
						(float)std::get<double>(object.get()->obj), TSET_COLOR_SAT_MIN, TSET_COLOR_SAT_MAX, false);
				}
				else { finalSat_max = (float)std::get<double>(object.get()->obj); }
			} break;
		case 2738:
			if (!isGet)
			{
				finalSat_bias = (float)std::get<double>(object.get()->obj);
			}
			else
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(finalSat_bias, settingsSig, subSig, "finalSat_bias")); }
				return std::make_shared<CObject>((double)finalSat_bias);
			}
			break;
		case 2739:
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(finalValue_min, settingsSig, subSig, "finalValue_min")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange_squaredCenter(finalValue_min, TSET_COLOR_LUM_MIN, TSET_COLOR_LUM_MAX, true)));
				}
				else { return std::make_shared<CObject>(double(finalValue_min)); }
			}
			else
			{
				if (asPercentage) {
					finalValue_min = percentRange_squaredCenter(
						(float)std::get<double>(object.get()->obj), TSET_COLOR_LUM_MIN, TSET_COLOR_LUM_MAX, false);
				}
				else { finalValue_min = (float)std::get<double>(object.get()->obj); }
			} break;
		case 2740:
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(finalValue_max, settingsSig, subSig, "finalValue_max")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange_squaredCenter(finalValue_max, TSET_COLOR_LUM_MIN, TSET_COLOR_LUM_MAX, true)));
				}
				else { return std::make_shared<CObject>(double(finalValue_max)); }
			}
			else
			{
				if (asPercentage) {
					finalValue_max = percentRange_squaredCenter(
						(float)std::get<double>(object.get()->obj), TSET_COLOR_LUM_MIN, TSET_COLOR_LUM_MAX, false);
				}
				else { finalValue_max = (float)std::get<double>(object.get()->obj); }
			} break;
		case 2741:
			if (!isGet)
			{
				finalValue_bias = (float)std::get<double>(object.get()->obj);
			}
			else
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(finalValue_bias, settingsSig, subSig, "finalValue_bias")); }
				return std::make_shared<CObject>((double)finalValue_bias);
			}
			break;
		}
	}

	if (asString) { return std::make_shared<CObject>(makeSettingString(isEnabled, settingsSig, subSig, "NULL")); }
	return std::make_shared<CObject>(nullptr);
}
void TSet_Color::modulateColor(CColor& FGcolor, CColor& BGcolor, glm::vec3 dir,
	float pressure, float rotation, float tiltX, float tiltY, float velocity)
{
	std::random_device device;
	std::default_random_engine eng(device());
	std::uniform_real_distribution<float> distr(0, 1);

	// Modulate BG Color before assessing mixing
	CColor_hsl outBGHSL = RGB_toHSL(glm::dvec3((double)BGcolor.r, (double)BGcolor.g, (double)BGcolor.b));
	CColor_hsl outFGHSL = RGB_toHSL(glm::dvec3((double)FGcolor.r, (double)FGcolor.g, (double)FGcolor.b));

	// Apply color mixing
	if (enableMixBG)
	{
		// Only apply BG Dynamics when mixing is enabled
		if (enableBGDynamics)
		{
			outBGHSL.h = clampAngle_360(outBGHSL.h + lerpf(BGHue_min, BGHue_max, (double)distr(eng)));
			if (outBGHSL.s > 0) { outBGHSL.s = clampd(outBGHSL.s + lerpf(BGSat_min, BGSat_max, (double)distr(eng)), 0.0, 1.0); }
			outBGHSL.l = clampd(outBGHSL.l + lerpf(BGValue_min, BGValue_max, (double)distr(eng)), 0.0, 1.0);
		}

		CColor outBGRGB = HSL_toRGB(glm::dvec3(outBGHSL.h / 360.0, outBGHSL.s, outBGHSL.l));
		outBGRGB.mixColor(&FGcolor, pressure);
		outFGHSL = RGB_toHSL(glm::dvec3((double)outBGRGB.r, (double)outBGRGB.g, (double)outBGRGB.b));
	}

	// Modulate final color after mixing

	if (enableFinalDynamics)
	{
		outFGHSL.h = clampAngle_360(outFGHSL.h + lerpf(finalHue_min, finalHue_max, (double)distr(eng)));
		if (outFGHSL.s > 0) { outFGHSL.s = clampd(outFGHSL.s + lerpf(finalSat_min, finalSat_max, (double)distr(eng)), 0.0, 1.0); }
		outFGHSL.l = clampd(outFGHSL.l + lerpf(finalValue_min, finalValue_max, (double)distr(eng)), 0.0, 1.0);
	}

	FGcolor = HSL_toRGB(glm::dvec3((double)outFGHSL.h / 360.0, (double)outFGHSL.s, (double)outFGHSL.l));
}