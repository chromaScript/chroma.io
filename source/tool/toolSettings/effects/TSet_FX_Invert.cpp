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

#include "../../../include/tool/toolSettings/effects/TSet_FX_Invert.h"

////////////////////////////////////////////////////////////////
// FX INVERT Settings
////////////////////////////////////////////////////////////////

TSet_FX_Invert::TSet_FX_Invert()
{
	this->type = TSetType::fx_invert;
	this->isEnabled = false;
	this->mixAmount = 1.0f;
	this->invertRed = true;
	this->invertGreen = true;
	this->invertBlue = true;
}
std::shared_ptr<CObject> TSet_FX_Invert::putProperty(
	std::shared_ptr<CInterpreter> interpreter, std::shared_ptr<Tool> owner,
	int settingsSig, int subSig, std::shared_ptr<CObject> object, bool isGet, bool asPercentage, bool asString)
{
	if (settingsSig >= 8400 && settingsSig < 8420)
	{
		if (settingsSig == 8405)
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
		else if (settingsSig == 8410)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(invertRed, settingsSig, subSig, "invertRed")); }
				return std::make_shared<CObject>(invertRed);
			}
			else { invertRed = std::get<bool>(object.get()->obj); }
		}
		else if (settingsSig == 8411)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(invertGreen, settingsSig, subSig, "invertGreen")); }
				return std::make_shared<CObject>(invertGreen);
			}
			else { invertGreen = std::get<bool>(object.get()->obj); }
		}
		else if (settingsSig == 8412)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(invertBlue, settingsSig, subSig, "invertBlue")); }
				return std::make_shared<CObject>(invertBlue);
			}
			else { invertBlue = std::get<bool>(object.get()->obj); }
		}
	}
	if (asString) { return std::make_shared<CObject>(makeSettingString(isEnabled, settingsSig, subSig, "NULL")); }
	return std::make_shared<CObject>(nullptr);
}

TSetControl_Node* TSet_FX_Invert::getControlNode(int settingSig, int subSig)
{
	return nullptr;
}
TSetController* TSet_FX_Invert::getController(int settingSig, int subSig)
{
	return nullptr;
}
TSetGraph* TSet_FX_Invert::getGraph(int settingSig, int subSig)
{
	return nullptr;
}
TSetNoise* TSet_FX_Invert::getNoise(int settingSig, int subSig)
{
	return nullptr;
}

void TSet_FX_Invert::initializeData(CColor FGColor, CColor BGColor)
{
}