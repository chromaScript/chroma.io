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
#include "../../include/toolSettings/effects/TSet_Effects.h"

////////////////////////////////////////////////////////////////
// Effects Settings
////////////////////////////////////////////////////////////////

// 81 - Fill , 82 - Gradient, 83 - Posterize, 84 - Invert, 85 - Threshold, 86 - Bright/Contrast
// 87 - HSV

TSet_Effects::TSet_Effects() // 8000
{
	this->type = TSetType::effects;
	this->isEnabled = false;

	this->effectsOrdering[0] = 0; // 8098 subSig 0 - 15
	this->effectsOrdering[1] = 0;
	this->effectsOrdering[2] = 0;
	this->effectsOrdering[3] = 0;
	this->effectsOrdering[4] = 0;
	this->effectsOrdering[5] = 0;
	this->effectsOrdering[6] = 0;
	this->effectsOrdering[7] = 0;
	this->effectsOrdering[8] = 0;
	this->effectsOrdering[9] = 0;
	this->effectsOrdering[10] = 0;
	this->effectsOrdering[11] = 0;
	this->effectsOrdering[12] = 0;
	this->effectsOrdering[13] = 0;
	this->effectsOrdering[14] = 0;
	this->effectsOrdering[15] = 0;
	updateEffectsCount(false);

	this->fill.isEnabled = false; // 8010
	this->gradient.isEnabled = false; // 8011
	this->posterize.isEnabled = false; // 8012
	this->invert.isEnabled = false; // 8013
	this->threshold.isEnabled = false; // 8014
	this->brightContrast.isEnabled = false; // 8015
	this->hsv.isEnabled = false; // 8016
	this->power.isEnabled = false; // 8017
	this->modulo.isEnabled = false; // 8018
	this->blur.isEnabled = false; // 8024

	this->totalBlend = 1.0f; // 8050
	this->totalMask_alphaCenter = 0.5f; // 8051
	this->totalMask_alphaRange = 1.0f; // 8052
}

std::shared_ptr<CObject> TSet_Effects::putProperty(
	std::shared_ptr<CInterpreter> interpreter, std::shared_ptr<Tool> owner,
	int settingsSig, int subSig, std::shared_ptr<CObject> object, bool isGet, bool asPercentage, bool asString)
{
	if (settingsSig == 8098)
	{
		if (subSig >= 0 && subSig <= 15)
		{
			if (!isGet)
			{
				effectsOrdering[(size_t)subSig] = objectToFX_int(object, false);
				updateEffectsCount(false);
			}
			else
			{
				if (asString) {
					return std::make_shared<CObject>(
						makeSettingString(effectsOrdering[(size_t)subSig], settingsSig, subSig, "effectsOrdering" + std::to_string(subSig), false));
				}
				return std::make_shared<CObject>(intToFX_string(effectsOrdering[(size_t)subSig]));
			}
		}
		else if (subSig == -2)
		{
			if (!isGet)
			{
				for (int i = 0; i < 15; i++)
				{
					if (effectsOrdering[i] == 0)
					{
						int fxSig = objectToFX_int(object, false);
						effectsOrdering[(size_t)i] = fxSig;
						switch (fxSig)
						{
						case 81: fill.isEnabled = true; break;
						case 82: gradient.isEnabled = true; break;
						case 83: posterize.isEnabled = true; break;
						case 84: invert.isEnabled = true; break;
						case 85: threshold.isEnabled = true; break;
						case 86: brightContrast.isEnabled = true; break;
						case 87: hsv.isEnabled = true; break;
						case 90: power.isEnabled = true; break;
						case 91: modulo.isEnabled = true; break;
						case 95: blur.isEnabled = true; break;
						}
						updateEffectsCount(false);
						break;
					}
				}
			}
		}
		else
		{
			if (!isGet)
			{
				if (object.get()->objType.type == CLiteralTypes::_CString)
				{
					std::string objStr = std::get<std::string>(object.get()->obj);
					std::vector<std::string> strVec = stringVec_fromCommaSeparated(objStr, true);
					if (strVec.size() != 0)
					{
						std::vector<std::shared_ptr<CObject>> strObjVec;
						for (std::string str : strVec)
						{
							strObjVec.push_back(std::make_shared<CObject>(str));
						}
						std::shared_ptr<CObject> vecObj = std::make_shared<CObject>(
							CLiteralTypes::_CString_Array, 
							std::make_shared<std::vector<std::shared_ptr<CObject>>>(strObjVec));
						objectToFX_int(vecObj, true);
					}
				}
				else
				{
					objectToFX_int(object, true);
				}
				updateEffectsCount(true);
			}
			else
			{
				if (asString) {
					std::vector<std::string> effectsStrVec;
					for (int i = 0; i < 15; i++)
					{
						if (effectsOrdering[i] == 0) { continue; }
						effectsStrVec.push_back(intToFX_string(effectsOrdering[i]));
					}
					return std::make_shared<CObject>(
						makeSettingString(effectsStrVec, settingsSig, subSig, "effectsOrdering"));
				}
				std::vector<std::shared_ptr<CObject>> effectsStrObj;
				for (int i = 0; i < 15; i++)
				{
					if (effectsOrdering[i] == 0) { continue; }
					effectsStrObj.push_back(std::make_shared<CObject>(intToFX_string(effectsOrdering[i])));
				}
				return std::make_shared<CObject>(CLiteralTypes::_CString_Array,
					std::make_shared<std::vector<std::shared_ptr<CObject>>>(effectsStrObj));
			}
		}
	}
	else if (settingsSig >= 8010 && settingsSig <= 8049)
	{
		if (settingsSig == 8010)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(fill.isEnabled, settingsSig, subSig, "fill_isEnabled")); }
				return std::make_shared<CObject>(fill.isEnabled);
			}
			else { fill.isEnabled = std::get<bool>(object.get()->obj); }
		}
		else if (settingsSig == 8011)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(gradient.isEnabled, settingsSig, subSig, "gradient_isEnabled")); }
				return std::make_shared<CObject>(gradient.isEnabled);
			}
			else { gradient.isEnabled = std::get<bool>(object.get()->obj); }
		}
		else if (settingsSig == 8012)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(posterize.isEnabled, settingsSig, subSig, "posterize_isEnabled")); }
				return std::make_shared<CObject>(posterize.isEnabled);
			}
			else { posterize.isEnabled = std::get<bool>(object.get()->obj); }
		}
		else if (settingsSig == 8013)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(invert.isEnabled, settingsSig, subSig, "invert_isEnabled")); }
				return std::make_shared<CObject>(invert.isEnabled);
			}
			else { invert.isEnabled = std::get<bool>(object.get()->obj); }
		}
		else if (settingsSig == 8014)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(threshold.isEnabled, settingsSig, subSig, "threshold_isEnabled")); }
				return std::make_shared<CObject>(threshold.isEnabled);
			}
			else { threshold.isEnabled = std::get<bool>(object.get()->obj); }
		}
		else if (settingsSig == 8015)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(brightContrast.isEnabled, settingsSig, subSig, "brightContrast_isEnabled")); }
				return std::make_shared<CObject>(brightContrast.isEnabled);
			}
			else { brightContrast.isEnabled = std::get<bool>(object.get()->obj); }
		}
		else if (settingsSig == 8016)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(hsv.isEnabled, settingsSig, subSig, "hsv_isEnabled")); }
				return std::make_shared<CObject>(hsv.isEnabled);
			}
			else { hsv.isEnabled = std::get<bool>(object.get()->obj); }
		}
		else if (settingsSig == 8017)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(power.isEnabled, settingsSig, subSig, "power_isEnabled")); }
				return std::make_shared<CObject>(power.isEnabled);
			}
			else { power.isEnabled = std::get<bool>(object.get()->obj); }
		}
		else if (settingsSig == 8018)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(modulo.isEnabled, settingsSig, subSig, "modulo_isEnabled")); }
				return std::make_shared<CObject>(modulo.isEnabled);
			}
			else { modulo.isEnabled = std::get<bool>(object.get()->obj); }
		}
		else if (settingsSig == 8024)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(blur.isEnabled, settingsSig, subSig, "blur_isEnabled")); }
				return std::make_shared<CObject>(blur.isEnabled);
			}
			else { blur.isEnabled = std::get<bool>(object.get()->obj); }
		}
	}
	else if (settingsSig >= 8050 && settingsSig <= 8069)
	{
		if (settingsSig == 8050)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(totalBlend, settingsSig, subSig, "totalBlend")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange_cubedInvert(totalBlend, 0.0f, 1.0f, true)));
				}
				else { return std::make_shared<CObject>(double(totalBlend)); }
			}
			else
			{
				if (asPercentage) {
					totalBlend = percentRange_cubedInvert(
						(float)std::get<double>(object.get()->obj), 0.0f, 1.0f, false);
				}
				else { totalBlend = clampf((float)std::get<double>(object.get()->obj), 0.0f, 1.0f); }
			}
		}
		else if (settingsSig == 8051)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(totalMask_alphaCenter, settingsSig, subSig, "totalMask_alphaCenter")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange(totalMask_alphaCenter, 0.0f, 1.0f, true)));
				}
				else { return std::make_shared<CObject>(double(totalMask_alphaCenter)); }
			}
			else
			{
				if (asPercentage) {
					totalMask_alphaCenter = percentRange(
						(float)std::get<double>(object.get()->obj), 0.0f, 1.0f, false);
				}
				else { totalMask_alphaCenter = clampf((float)std::get<double>(object.get()->obj), 0.0f, 1.0f); }
			}
		}
		else if (settingsSig == 8052)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(totalMask_alphaRange, settingsSig, subSig, "totalMask_alphaRange")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange(totalMask_alphaRange, 0.0f, 1.0f, true)));
				}
				else { return std::make_shared<CObject>(double(totalMask_alphaRange)); }
			}
			else
			{
				if (asPercentage) {
					totalMask_alphaRange = percentRange(
						(float)std::get<double>(object.get()->obj), 0.0f, 1.0f, false);
				}
				else { totalMask_alphaRange = clampf((float)std::get<double>(object.get()->obj), 0.0f, 1.0f); }
			}
		}
	}
	if (settingsSig >= 8300 && settingsSig <= 8399)
	{
		return posterize.putProperty(interpreter, owner, settingsSig, subSig, object, isGet, asPercentage, asString);
	}
	else if (settingsSig >= 8400 && settingsSig <= 8499)
	{
		return invert.putProperty(interpreter, owner, settingsSig, subSig, object, isGet, asPercentage, asString);
	}
	else if (settingsSig >= 8600 && settingsSig <= 8699)
	{
		return brightContrast.putProperty(interpreter, owner, settingsSig, subSig, object, isGet, asPercentage, asString);
	}
	else if (settingsSig >= 8700 && settingsSig <= 8799)
	{
		return hsv.putProperty(interpreter, owner, settingsSig, subSig, object, isGet, asPercentage, asString);
	}
	else if (settingsSig >= 9000 && settingsSig <= 9099)
	{
		return power.putProperty(interpreter, owner, settingsSig, subSig, object, isGet, asPercentage, asString);
	}
	else if (settingsSig >= 9100 && settingsSig <= 9199)
	{
		return modulo.putProperty(interpreter, owner, settingsSig, subSig, object, isGet, asPercentage, asString);
	}
	else if (settingsSig >= 9500 && settingsSig <= 9599)
	{
		return blur.putProperty(interpreter, owner, settingsSig, subSig, object, isGet, asPercentage, asString);
	}
	if (asString) { return std::make_shared<CObject>(makeSettingString(isEnabled, settingsSig, subSig, "NULL")); }
	return std::make_shared<CObject>(nullptr);
}

void TSet_Effects::initializeData(CColor FGColor, CColor BGColor, int tipSize)
{
	updateEffectsOrdering(true);
	updateEffectsCount(false);

	if (fill.isEnabled) { fill.initializeData(FGColor, BGColor, tipSize); }
	if (gradient.isEnabled) { gradient.initializeData(FGColor, BGColor, tipSize); }
	if (brightContrast.isEnabled) { brightContrast.initializeData(FGColor, BGColor); }
	if (hsv.isEnabled) { hsv.initializeData(FGColor, BGColor); }
}
std::vector<int> TSet_Effects::getOrdering_vec()
{
	std::vector<int> outVec;
	for (int i = 0; i < 16; i++)
	{
		outVec.push_back(effectsOrdering[i]);
	}
	return outVec;
}
void TSet_Effects::updateEffectsOrdering(bool removeZero)
{
	std::vector<int> newOrder;
	for (int i = 0; i < 16; i++)
	{
		switch (effectsOrdering[i])
		{
		case 81: newOrder.push_back(81 * ((removeZero) ? fill.isEnabled : 1)); break;
		case 82: newOrder.push_back(82 * ((removeZero) ? gradient.isEnabled : 1)); break;
		case 83: newOrder.push_back(83 * ((removeZero) ? posterize.isEnabled : 1)); break;
		case 84: newOrder.push_back(84 * ((removeZero) ? invert.isEnabled : 1)); break;
		case 85: newOrder.push_back(85 * ((removeZero) ? threshold.isEnabled : 1)); break;
		case 86: newOrder.push_back(86 * ((removeZero) ? brightContrast.isEnabled : 1)); break;
		case 87: newOrder.push_back(87 * ((removeZero) ? hsv.isEnabled : 1)); break;
		case 90: newOrder.push_back(90 * ((removeZero) ? power.isEnabled : 1)); break;
		case 91: newOrder.push_back(91 * ((removeZero) ? modulo.isEnabled : 1)); break;
		case 95: newOrder.push_back(95 * ((removeZero) ? blur.isEnabled : 1)); break;
		default: newOrder.push_back(0);
		}
		effectsOrdering[i] = 0;
	}
	int copyPos = 0;
	for (int newVal : newOrder)
	{
		if (newVal != 0) { effectsOrdering[copyPos] = newVal; copyPos++; }
	}
}
void TSet_Effects::updateEffectsCount(bool checkVisible)
{
	int count = 0;
	for (int i = 0; i < 16; i++)
	{
		if (effectsOrdering[i] != 0) 
		{ 
			if (checkVisible)
			{
				switch (effectsOrdering[i])
				{
				case 81: if (!fill.isEnabled) { effectsOrdering[i] = 0; }
					   else { count++; } break;
				case 82: if (!gradient.isEnabled) { effectsOrdering[i] = 0; } 
					   else { count++; } break;
				case 83: if (!posterize.isEnabled) { effectsOrdering[i] = 0; } 
					   else { count++; } break;
				case 84: if (!invert.isEnabled) { effectsOrdering[i] = 0; }
					   else { count++; } break;
				case 85: if (!threshold.isEnabled) { effectsOrdering[i] = 0; } 
					   else { count++; } break;
				case 86: if (!brightContrast.isEnabled) { effectsOrdering[i] = 0; } 
					   else { count++; } break;
				case 87: if (!hsv.isEnabled) { effectsOrdering[i] = 0; } 
					   else { count++; } break;
				case 90: if (!power.isEnabled) { effectsOrdering[i] = 0; }
					   else { count++; } break;
				case 91: if (!modulo.isEnabled) { effectsOrdering[i] = 0; }
					   else { count++; } break;
				case 95: if (!blur.isEnabled) { effectsOrdering[i] = 0; }
					   else { count++; } break;
				}
			}
			else { count++; }
		}
	}
	effectsCount = clampi(count, 0, 16);
	
}
int TSet_Effects::objectToFX_int(std::shared_ptr<CObject> object, bool isArraySet)
{
	if (object.get()->objType.type == CLiteralTypes::_CString_Array && isArraySet)
	{
		std::vector<std::shared_ptr<CObject>> vec;
		if (object.get()->obj.index() != 0)
		{ 
			vec = *std::get<std::shared_ptr<std::vector<std::shared_ptr<CObject>>>>(object.get()->obj);
		}
		for (int c = 0; c < 16; c++)
		{
			if (c < vec.size())
			{
				effectsOrdering[c] = objectToFX_int(vec[c], false);
			}
			else
			{
				effectsOrdering[c] = 0;
			}
		}
		return 0;
	}
	else if (object.get()->objType.type == CLiteralTypes::_CNumber_Array && isArraySet)
	{
		int i = 0;
		std::vector<std::shared_ptr<CObject>> vec = *std::get<std::shared_ptr<std::vector<std::shared_ptr<CObject>>>>(object.get()->obj);
		for (std::shared_ptr<CObject> obj : vec)
		{
			effectsOrdering[i] = (int)std::get<double>(obj.get()->obj);
			i++;
		}
		return 0;
	}
	else if (object.get()->objType.type == CLiteralTypes::_CString)
	{
		std::string objStr = std::get<std::string>(object.get()->obj);
		stringToLower(objStr);
		if (objStr.find("fill") != std::string::npos) { return 81; }
		if (objStr.find("gradient") != std::string::npos) { return 82; }
		if (objStr.find("posterize") != std::string::npos) { return 83; }
		if (objStr.find("invert") != std::string::npos) { return 84; }
		if (objStr.find("threshold") != std::string::npos) { return 85; }
		if (objStr.find("power") != std::string::npos) { return 90; }
		if (objStr.find("modulo") != std::string::npos) { return 91; }
		if (objStr.find("blur") != std::string::npos) { return 95; }

		if (objStr.find("hsv") != std::string::npos ||
			objStr.find("huesat") != std::string::npos) {
			return 87;
		}
		if (objStr.find("bright") != std::string::npos ||
			objStr.find("contrast") != std::string::npos ||
			objStr.find("brightness") != std::string::npos ||
			objStr.find("brightcontrast") != std::string::npos) {
			return 86;
		}
	}
	else if (object.get()->objType.type == CLiteralTypes::_CNumber)
	{
		return (int)std::get<double>(object.get()->obj);
	}
	return 0;
}
std::string TSet_Effects::intToFX_string(int effectsSig)
{
	switch (effectsSig)
	{
	case 81: return "fill"; break;
	case 82: return "gradient"; break;
	case 83: return "posterize"; break;
	case 84: return "invert"; break;
	case 85: return "threshold"; break;
	case 86: return "brightcontrast"; break;
	case 87: return "hsv"; break;
	case 90: return "power"; break;
	case 91: return "modulo"; break;
	case 95: return "blur"; break;
	}
	return "";
}