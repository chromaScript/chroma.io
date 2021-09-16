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
#include "../../include/toolSettings/effects/TSet_FX_HSV.h"

////////////////////////////////////////////////////////////////
// FX HSV Settings
////////////////////////////////////////////////////////////////

TSet_FX_HSV::TSet_FX_HSV()
{
	this->type = TSetType::fx_threshold;
	this->isEnabled = false;
	this->mixAmount = 1.0f; // 8705
	this->hueMin = 0.0f; // 8710
	this->hueMax = 0.0f; // 8711
	this->satMin = 0.0f; // 8712
	this->satMax = 0.0f; // 8713
	this->valMin = 0.0f; // 8714
	this->valMax = 0.0f; // 8715
}
std::shared_ptr<CObject> TSet_FX_HSV::putProperty(
	std::shared_ptr<CInterpreter> interpreter, std::shared_ptr<Tool> owner,
	int settingsSig, int subSig, std::shared_ptr<CObject> object, bool isGet, bool asPercentage, bool asString)
{
	if (settingsSig >= 8700 && settingsSig < 8710)
	{
		if (settingsSig == 8705)
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
	else if (settingsSig >= 8710 && settingsSig < 8720)
	{
		if (settingsSig == 8710)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(hueMin, settingsSig, subSig, "hueMin")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange_squaredCenter(hueMin, TSET_FX_HSV_HUE_MIN, TSET_FX_HSV_HUE_MAX, true)));
				}
				else { return std::make_shared<CObject>(double(hueMin)); }
			}
			else
			{
				if (asPercentage) {
					hueMin = percentRange_squaredCenter(
						(float)std::get<double>(object.get()->obj), TSET_FX_HSV_HUE_MIN, TSET_FX_HSV_HUE_MAX, false);
				}
				else { hueMin = clampf((float)std::get<double>(object.get()->obj), TSET_FX_HSV_HUE_MIN, TSET_FX_HSV_HUE_MAX); }
			}
		}
		else if(settingsSig == 8711)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(hueMax, settingsSig, subSig, "hueMax")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange_squaredCenter(hueMax, TSET_FX_HSV_HUE_MIN, TSET_FX_HSV_HUE_MAX, true)));
				}
				else { return std::make_shared<CObject>(double(hueMax)); }
			}
			else
			{
				if (asPercentage) {
					hueMax = percentRange_squaredCenter(
						(float)std::get<double>(object.get()->obj), TSET_FX_HSV_HUE_MIN, TSET_FX_HSV_HUE_MAX, false);
				}
				else { hueMax = clampf((float)std::get<double>(object.get()->obj), TSET_FX_HSV_HUE_MIN, TSET_FX_HSV_HUE_MAX); }
			}
		}
		else if (settingsSig == 8712)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(satMin, settingsSig, subSig, "satMin")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange_squaredCenter(satMin, TSET_FX_HSV_SAT_MIN, TSET_FX_HSV_SAT_MAX, true)));
				}
				else { return std::make_shared<CObject>(double(satMin)); }
			}
			else
			{
				if (asPercentage) {
					satMin = percentRange_squaredCenter(
						(float)std::get<double>(object.get()->obj), TSET_FX_HSV_SAT_MIN, TSET_FX_HSV_SAT_MAX, false);
				}
				else { satMin = clampf((float)std::get<double>(object.get()->obj), TSET_FX_HSV_SAT_MIN, TSET_FX_HSV_SAT_MAX); }
			}
		}
		else if (settingsSig == 8713)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(satMax, settingsSig, subSig, "satMax")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange_squaredCenter(satMax, TSET_FX_HSV_SAT_MIN, TSET_FX_HSV_SAT_MAX, true)));
				}
				else { return std::make_shared<CObject>(double(satMax)); }
			}
			else
			{
				if (asPercentage) {
					satMax = percentRange_squaredCenter(
						(float)std::get<double>(object.get()->obj), TSET_FX_HSV_SAT_MIN, TSET_FX_HSV_SAT_MAX, false);
				}
				else { satMax = clampf((float)std::get<double>(object.get()->obj), TSET_FX_HSV_SAT_MIN, TSET_FX_HSV_SAT_MAX); }
			}
		}
		else if (settingsSig == 8714)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(valMin, settingsSig, subSig, "valMin")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange_squaredCenter(valMin, TSET_FX_HSV_LUM_MIN, TSET_FX_HSV_LUM_MAX, true)));
				}
				else { return std::make_shared<CObject>(double(valMin)); }
			}
			else
			{
				if (asPercentage) {
					valMin = percentRange_squaredCenter(
						(float)std::get<double>(object.get()->obj), TSET_FX_HSV_LUM_MIN, TSET_FX_HSV_LUM_MAX, false);
				}
				else { valMin = clampf((float)std::get<double>(object.get()->obj), TSET_FX_HSV_LUM_MIN, TSET_FX_HSV_LUM_MAX); }
			}
		}
		else if (settingsSig == 8715)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(valMax, settingsSig, subSig, "valMax")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange_squaredCenter(valMax, TSET_FX_HSV_LUM_MIN, TSET_FX_HSV_LUM_MAX, true)));
				}
				else { return std::make_shared<CObject>(double(valMax)); }
			}
			else
			{
				if (asPercentage) {
					valMax = percentRange_squaredCenter(
						(float)std::get<double>(object.get()->obj), TSET_FX_HSV_LUM_MIN, TSET_FX_HSV_LUM_MAX, false);
				}
				else { valMax = clampf((float)std::get<double>(object.get()->obj), TSET_FX_HSV_LUM_MIN, TSET_FX_HSV_LUM_MAX); }
			}
		}
	}

	if (asString) { return std::make_shared<CObject>(makeSettingString(isEnabled, settingsSig, subSig, "NULL")); }
	return std::make_shared<CObject>(nullptr);
}
void TSet_FX_HSV::initializeData(CColor FGColor, CColor BGColor)
{
	std::random_device device;
	std::default_random_engine eng(device());
	std::uniform_real_distribution<float> distr(0, 1);

	finalHue = lerpf(hueMin, hueMax, distr(eng)) / 360.0f;
	finalSat = lerpf(satMin, satMax, distr(eng));
	finalVal = lerpf(valMin, valMax, distr(eng));
}