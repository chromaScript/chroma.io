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

#include "../../../include/tool/toolSettings/effects/TSet_FX_Power.h"

////////////////////////////////////////////////////////////////
// FX POSTERIZE Settings
////////////////////////////////////////////////////////////////

TSet_FX_Power::TSet_FX_Power()
{
	this->type = TSetType::fx_power;
	this->isEnabled = false; 
	this->mixAmount = 1.0f; // 9005
	this->gammaRed = 1.0f; // 9010
	this->gammaBlue = 1.0f; // 9011
	this->gammaGreen = 1.0f; // 9012
}
std::shared_ptr<CObject> TSet_FX_Power::putProperty(
	std::shared_ptr<CInterpreter> interpreter, std::shared_ptr<Tool> owner,
	int settingsSig, int subSig, std::shared_ptr<CObject> object, bool isGet, bool asPercentage, bool asString)
{
	if (settingsSig >= 9000 && settingsSig < 9010)
	{
		if (settingsSig == 9005)
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
	else if (settingsSig >= 9010 && settingsSig < 9020)
	{
		if (settingsSig == 9010)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(gammaRed, settingsSig, subSig, "gammaRed")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange_cubed(gammaRed, TSET_FX_POWER_GAMMA_MIN, TSET_FX_POWER_GAMMA_MAX, true)));
				}
				else { return std::make_shared<CObject>(double(gammaRed)); }
			}
			else
			{
				if (asPercentage) {
					gammaRed = percentRange_cubed(
						(float)std::get<double>(object.get()->obj), TSET_FX_POWER_GAMMA_MIN, TSET_FX_POWER_GAMMA_MAX, false);
				}
				else { gammaRed = clampf((float)std::get<double>(object.get()->obj), TSET_FX_POWER_GAMMA_MIN, TSET_FX_POWER_GAMMA_MAX); }
			}
		}
		if (settingsSig == 9011)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(gammaGreen, settingsSig, subSig, "gammaGreen")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange_cubed(gammaGreen, TSET_FX_POWER_GAMMA_MIN, TSET_FX_POWER_GAMMA_MAX, true)));
				}
				else { return std::make_shared<CObject>(double(gammaGreen)); }
			}
			else
			{
				if (asPercentage) {
					gammaGreen = percentRange_cubed(
						(float)std::get<double>(object.get()->obj), TSET_FX_POWER_GAMMA_MIN, TSET_FX_POWER_GAMMA_MAX, false);
				}
				else { gammaGreen = clampf((float)std::get<double>(object.get()->obj), TSET_FX_POWER_GAMMA_MIN, TSET_FX_POWER_GAMMA_MAX); }
			}
		}
		if (settingsSig == 9012)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(gammaBlue, settingsSig, subSig, "gammaBlue")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange_cubed(gammaBlue, TSET_FX_POWER_GAMMA_MIN, TSET_FX_POWER_GAMMA_MAX, true)));
				}
				else { return std::make_shared<CObject>(double(gammaBlue)); }
			}
			else
			{
				if (asPercentage) {
					gammaBlue = percentRange_cubed(
						(float)std::get<double>(object.get()->obj), TSET_FX_POWER_GAMMA_MIN, TSET_FX_POWER_GAMMA_MAX, false);
				}
				else { gammaBlue = clampf((float)std::get<double>(object.get()->obj), TSET_FX_POWER_GAMMA_MIN, TSET_FX_POWER_GAMMA_MAX); }
			}
		}
	}

	if (asString) { return std::make_shared<CObject>(makeSettingString(isEnabled, settingsSig, subSig, "NULL")); }
	return std::make_shared<CObject>(nullptr);
}

TSetControl_Node* TSet_FX_Power::getControlNode(int settingSig, int subSig)
{
	return nullptr;
}
TSetController* TSet_FX_Power::getController(int settingSig, int subSig)
{
	return nullptr;
}
TSetGraph* TSet_FX_Power::getGraph(int settingSig, int subSig)
{
	return nullptr;
}
TSetNoise* TSet_FX_Power::getNoise(int settingSig, int subSig)
{
	return nullptr;
}

void TSet_FX_Power::initializeData(CColor FGColor, CColor BGColor)
{
}