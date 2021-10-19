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

#include "../../../include/tool/toolSettings/effects/TSet_FX_Modulo.h"

////////////////////////////////////////////////////////////////
// FX MODULO Settings
////////////////////////////////////////////////////////////////

TSet_FX_Modulo::TSet_FX_Modulo()
{
	this->type = TSetType::fx_modulo;
	this->isEnabled = false;
	this->mixAmount = 1.0f;
	this->valueA = 50.0f; // 9110
	this->valueB = 1.0f; // 9111
	this->useX = false; // 9112
	this->useY = false; // 9113
	this->useDirection = true; // 9114

	this->XMixRed = 1.0f; // 9120
	this->XMixGreen = 1.0f; // 9121
	this->XMixBlue = 0.0f; // 9122
	this->YMixRed = 1.0f; // 9123
	this->YMixGreen = 0.0f; // 9124
	this->YMixBlue = 1.0f; // 9125
	this->DirMixRed = 0.0f; // 9126
	this->DirMixGreen = 1.0f; // 9127
	this->DirMixBlue = 1.0f; // 9128
}
std::shared_ptr<CObject> TSet_FX_Modulo::putProperty(
	std::shared_ptr<CInterpreter> interpreter, std::shared_ptr<Tool> owner,
	int settingsSig, int subSig, std::shared_ptr<CObject> object, bool isGet, bool asPercentage, bool asString)
{
	if (settingsSig >= 9100 && settingsSig < 9110)
	{
		if (settingsSig == 9105)
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
	else if (settingsSig >= 9110 && settingsSig < 9120)
	{
		if (settingsSig == 9110)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(valueA, settingsSig, subSig, "valueA")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange_cubed(valueA, TSET_FX_MODULO_VALUE_A_MIN, TSET_FX_MODULO_VALUE_A_MAX, true)));
				}
				else { return std::make_shared<CObject>(double(valueA)); }
			}
			else
			{
				if (asPercentage) {
					valueA = percentRange_cubed(
						(float)std::get<double>(object.get()->obj), TSET_FX_MODULO_VALUE_A_MIN, TSET_FX_MODULO_VALUE_A_MAX, false);
				}
				else { valueA = clampf((float)std::get<double>(object.get()->obj), TSET_FX_MODULO_VALUE_A_MIN, TSET_FX_MODULO_VALUE_A_MAX); }
			}
		}
		else if (settingsSig == 9111)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(valueB, settingsSig, subSig, "valueB")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange_cubed(valueB, TSET_FX_MODULO_VALUE_B_MIN, TSET_FX_MODULO_VALUE_B_MAX, true)));
				}
				else { return std::make_shared<CObject>(double(valueB)); }
			}
			else
			{
				if (asPercentage) {
					valueB = percentRange_cubed(
						(float)std::get<double>(object.get()->obj), TSET_FX_MODULO_VALUE_B_MIN, TSET_FX_MODULO_VALUE_B_MAX, false);
				}
				else { valueB = clampf((float)std::get<double>(object.get()->obj), TSET_FX_MODULO_VALUE_B_MIN, TSET_FX_MODULO_VALUE_B_MAX); }
			}
		}
		else if (settingsSig == 9112)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(useX, settingsSig, subSig, "useX")); }
				return std::make_shared<CObject>(useX);
			}
			else { useX = std::get<bool>(object.get()->obj); }
		}
		else if (settingsSig == 9113)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(useY, settingsSig, subSig, "useY")); }
				return std::make_shared<CObject>(useY);
			}
			else { useY = std::get<bool>(object.get()->obj); }
		}
		else if (settingsSig == 9114)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(useDirection, settingsSig, subSig, "useDirection")); }
				return std::make_shared<CObject>(useDirection);
			}
			else { useDirection = std::get<bool>(object.get()->obj); }
		}
	}
	else if (settingsSig >= 9120 && settingsSig < 9130)
	{
		if (settingsSig == 9120)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(XMixRed, settingsSig, subSig, "XMixRed")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange_cubedInvert(XMixRed, 0.0f, 1.0f, true)));
				}
				else { return std::make_shared<CObject>(double(XMixRed)); }
			}
			else
			{
				if (asPercentage) {
					XMixRed = percentRange_cubedInvert(
						(float)std::get<double>(object.get()->obj), 0.0f, 1.0f, false);
				}
				else { XMixRed = clampf((float)std::get<double>(object.get()->obj), 0.0f, 1.0f); }
			}
		}
		else if (settingsSig == 9121)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(XMixGreen, settingsSig, subSig, "XMixGreen")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange_cubedInvert(XMixGreen, 0.0f, 1.0f, true)));
				}
				else { return std::make_shared<CObject>(double(XMixGreen)); }
			}
			else
			{
				if (asPercentage) {
					XMixGreen = percentRange_cubedInvert(
						(float)std::get<double>(object.get()->obj), 0.0f, 1.0f, false);
				}
				else { XMixGreen = clampf((float)std::get<double>(object.get()->obj), 0.0f, 1.0f); }
			}
		}
		else if (settingsSig == 9122)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(XMixBlue, settingsSig, subSig, "XMixBlue")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange_cubedInvert(XMixBlue, 0.0f, 1.0f, true)));
				}
				else { return std::make_shared<CObject>(double(XMixBlue)); }
			}
			else
			{
				if (asPercentage) {
					XMixBlue = percentRange_cubedInvert(
						(float)std::get<double>(object.get()->obj), 0.0f, 1.0f, false);
				}
				else { XMixBlue = clampf((float)std::get<double>(object.get()->obj), 0.0f, 1.0f); }
			}
		}
		else if (settingsSig == 9123)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(XMixRed, settingsSig, subSig, "YMixRed")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange_cubedInvert(YMixRed, 0.0f, 1.0f, true)));
				}
				else { return std::make_shared<CObject>(double(YMixRed)); }
			}
			else
			{
				if (asPercentage) {
					YMixRed = percentRange_cubedInvert(
						(float)std::get<double>(object.get()->obj), 0.0f, 1.0f, false);
				}
				else { YMixRed = clampf((float)std::get<double>(object.get()->obj), 0.0f, 1.0f); }
			}
		}
		else if (settingsSig == 9124)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(YMixGreen, settingsSig, subSig, "YMixGreen")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange_cubedInvert(YMixGreen, 0.0f, 1.0f, true)));
				}
				else { return std::make_shared<CObject>(double(YMixGreen)); }
			}
			else
			{
				if (asPercentage) {
					YMixGreen = percentRange_cubedInvert(
						(float)std::get<double>(object.get()->obj), 0.0f, 1.0f, false);
				}
				else { YMixGreen = clampf((float)std::get<double>(object.get()->obj), 0.0f, 1.0f); }
			}
		}
		else if (settingsSig == 9125)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(YMixBlue, settingsSig, subSig, "YMixBlue")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange_cubedInvert(YMixBlue, 0.0f, 1.0f, true)));
				}
				else { return std::make_shared<CObject>(double(YMixBlue)); }
			}
			else
			{
				if (asPercentage) {
					YMixBlue = percentRange_cubedInvert(
						(float)std::get<double>(object.get()->obj), 0.0f, 1.0f, false);
				}
				else { YMixBlue = clampf((float)std::get<double>(object.get()->obj), 0.0f, 1.0f); }
			}
		}
		else if (settingsSig == 9126)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(XMixRed, settingsSig, subSig, "DirMixRed")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange_cubedInvert(DirMixRed, 0.0f, 1.0f, true)));
				}
				else { return std::make_shared<CObject>(double(DirMixRed)); }
			}
			else
			{
				if (asPercentage) {
					DirMixRed = percentRange_cubedInvert(
						(float)std::get<double>(object.get()->obj), 0.0f, 1.0f, false);
				}
				else { DirMixRed = clampf((float)std::get<double>(object.get()->obj), 0.0f, 1.0f); }
			}
		}
		else if (settingsSig == 9127)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(DirMixGreen, settingsSig, subSig, "DirMixGreen")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange_cubedInvert(DirMixGreen, 0.0f, 1.0f, true)));
				}
				else { return std::make_shared<CObject>(double(DirMixGreen)); }
			}
			else
			{
				if (asPercentage) {
					DirMixGreen = percentRange_cubedInvert(
						(float)std::get<double>(object.get()->obj), 0.0f, 1.0f, false);
				}
				else { DirMixGreen = clampf((float)std::get<double>(object.get()->obj), 0.0f, 1.0f); }
			}
		}
		else if (settingsSig == 9128)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(DirMixBlue, settingsSig, subSig, "DirMixBlue")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange_cubedInvert(DirMixBlue, 0.0f, 1.0f, true)));
				}
				else { return std::make_shared<CObject>(double(DirMixBlue)); }
			}
			else
			{
				if (asPercentage) {
					DirMixBlue = percentRange_cubedInvert(
						(float)std::get<double>(object.get()->obj), 0.0f, 1.0f, false);
				}
				else { DirMixBlue = clampf((float)std::get<double>(object.get()->obj), 0.0f, 1.0f); }
			}
		}
	}

	if (asString) { return std::make_shared<CObject>(makeSettingString(isEnabled, settingsSig, subSig, "NULL")); }
	return std::make_shared<CObject>(nullptr);
}

TSetControl_Node* TSet_FX_Modulo::getControlNode(int settingSig, int subSig)
{
	return nullptr;
}
TSetController* TSet_FX_Modulo::getController(int settingSig, int subSig)
{
	return nullptr;
}
TSetGraph* TSet_FX_Modulo::getGraph(int settingSig, int subSig)
{
	return nullptr;
}
TSetNoise* TSet_FX_Modulo::getNoise(int settingSig, int subSig)
{
	return nullptr;
}

void TSet_FX_Modulo::initializeData(CColor FGColor, CColor BGColor)
{
}