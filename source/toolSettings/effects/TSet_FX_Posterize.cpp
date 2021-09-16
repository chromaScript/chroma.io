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
#include "../../include/toolSettings/effects/TSet_FX_Posterize.h"

////////////////////////////////////////////////////////////////
// FX POSTERIZE Settings
////////////////////////////////////////////////////////////////

TSet_FX_Posterize::TSet_FX_Posterize()
{
	this->type = TSetType::fx_posterize;
	this->isEnabled = false;
	this->mixAmount = 1.0f;
	this->levels = 4;
	this->gamma = 1.0f;
	this->useValueRange = false;
}
std::shared_ptr<CObject> TSet_FX_Posterize::putProperty(
	std::shared_ptr<CInterpreter> interpreter, std::shared_ptr<Tool> owner,
	int settingsSig, int subSig, std::shared_ptr<CObject> object, bool isGet, bool asPercentage, bool asString)
{
	if (settingsSig >= 8300 && settingsSig < 8310)
	{
		if (settingsSig == 8305)
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
	else if (settingsSig >= 8310 && settingsSig < 8320)
	{
		if (settingsSig == 8310)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(levels, settingsSig, subSig, "levels")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange_cubed(levels, TSET_FX_POSTERIZE_LEVELS_MIN, TSET_FX_POSTERIZE_LEVELS_MAX, true)));
				}
				else { return std::make_shared<CObject>(double(levels)); }
			}
			else
			{
				if (asPercentage) {
					levels = percentRange_cubed(
						(float)std::get<double>(object.get()->obj), TSET_FX_POSTERIZE_LEVELS_MIN, TSET_FX_POSTERIZE_LEVELS_MAX, false);
				}
				else { levels = clampf((float)std::get<double>(object.get()->obj), TSET_FX_POSTERIZE_LEVELS_MIN, TSET_FX_POSTERIZE_LEVELS_MAX); }
			}
		}
		else if (settingsSig == 8311)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(useValueRange, settingsSig, subSig, "useValueRange")); }
				return std::make_shared<CObject>(useValueRange);
			}
			else { useValueRange = std::get<bool>(object.get()->obj); }
		}
		else if (settingsSig == 8312)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(gamma, settingsSig, subSig, "gamma")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange_cubed(gamma, TSET_FX_POSTERIZE_GAMMA_MIN, TSET_FX_POSTERIZE_GAMMA_MAX, true)));
				}
				else { return std::make_shared<CObject>(double(gamma)); }
			}
			else
			{
				if (asPercentage) {
					gamma = percentRange_cubed(
						(float)std::get<double>(object.get()->obj), TSET_FX_POSTERIZE_GAMMA_MIN, TSET_FX_POSTERIZE_GAMMA_MAX, false);
				}
				else { gamma = clampf((float)std::get<double>(object.get()->obj), TSET_FX_POSTERIZE_GAMMA_MIN, TSET_FX_POSTERIZE_GAMMA_MAX); }
			}
		}
	}

	if (asString) { return std::make_shared<CObject>(makeSettingString(isEnabled, settingsSig, subSig, "NULL")); }
	return std::make_shared<CObject>(nullptr);
}
void TSet_FX_Posterize::initializeData(CColor FGColor, CColor BGColor)
{
}