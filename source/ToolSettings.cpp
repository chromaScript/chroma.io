#include "include/ToolSettings.h"
#include "include/keys.h"
#include "include/Color.h"

class CFunction;
#include "include/cscript/CInterpreter.h"
#include "include/cscript/CObject.h"
#include "include/Tool.h"

#include <glm.hpp>
#include <gtx/rotate_vector.hpp>
#include <string>
#include <vector>
#include <memory>
#include <random>

#include "include/structs.h"

////////////////////////////////////////////////////////////////
// Settings Global
////////////////////////////////////////////////////////////////
std::string makeSettingString(int value, int settingsSig, int subSig, std::string settingName, bool isKey)
{
	std::string out = "";
	out += TSETTINGSWRITER_P1 + std::to_string(settingsSig) + TSETTINGSWRITER_P2 + std::to_string(subSig);
	out += TSETTINGSWRITER_P3 + settingName + TSETTINGSWRITER_P4;
	if (false)
	{
		out += keySigToString(value);
	}
	else
	{
		out += std::to_string(value);
	}
	out += TSETTINGSWRITER_P5;
	return out;
}
std::string makeSettingString(float value, int settingsSig, int subSig, std::string settingName)
{
	std::string out = "";
	out += TSETTINGSWRITER_P1 + std::to_string(settingsSig) + TSETTINGSWRITER_P2 + std::to_string(subSig);
	out += TSETTINGSWRITER_P3 + settingName + TSETTINGSWRITER_P4;
	out += std::to_string(value);
	out += TSETTINGSWRITER_P5;
	return out;
}
std::string makeSettingString(bool value, int settingsSig, int subSig, std::string settingName)
{
	std::string out = "";
	out += TSETTINGSWRITER_P1 + std::to_string(settingsSig) + TSETTINGSWRITER_P2 + std::to_string(subSig);
	out += TSETTINGSWRITER_P3 + settingName + TSETTINGSWRITER_P4;
	if (value) { out += "\"true\""; }
	else { out += "\"false\""; }
	out += TSETTINGSWRITER_P5;
	return out;
}
std::string makeSettingString(std::string value, int settingsSig, int subSig, std::string settingName)
{
	std::string out = "";
	out += TSETTINGSWRITER_P1 + std::to_string(settingsSig) + TSETTINGSWRITER_P2 + std::to_string(subSig);
	out += TSETTINGSWRITER_P3 + settingName + TSETTINGSWRITER_P4;
	out += "\"" + value + "\"";
	out += TSETTINGSWRITER_P5;
	return out;
}
std::string makeSettingString(std::vector<std::string> value, int settingsSig, int subSig, std::string settingName)
{
	std::string out = "";
	out += TSETTINGSWRITER_P1 + std::to_string(settingsSig) + TSETTINGSWRITER_P2 + std::to_string(subSig);
	out += TSETTINGSWRITER_P3 + settingName + TSETTINGSWRITER_P4;
	out += "\"" + stringVecToString(value, false) + "\"";
	out += TSETTINGSWRITER_P5;
	return out;
}
std::string makeSettingString(std::vector<int> value, int settingsSig, int subSig, std::string settingName)
{
	std::string out = "";
	out += TSETTINGSWRITER_P1 + std::to_string(settingsSig) + TSETTINGSWRITER_P2 + std::to_string(subSig);
	out += TSETTINGSWRITER_P3 + settingName + TSETTINGSWRITER_P4;
	out += "\"" + intVecToString(value) + "\"";
	out += TSETTINGSWRITER_P5;
	return out;
}
std::string makeSettingString(TSetProp value, int settingsSig, int subSig, std::string settingName)
{
	std::string out = "";
	out += TSETTINGSWRITER_P1 + std::to_string(settingsSig) + TSETTINGSWRITER_P2 + std::to_string(subSig);
	out += TSETTINGSWRITER_P3 + settingName + TSETTINGSWRITER_P4;
	out += "\"" + TSetPropToString(value) + "\"";
	out += TSETTINGSWRITER_P5;
	return out;
}
std::string makeSettingString(TSetController value, int settingsSig, int subSig, std::string settingName)
{
	std::string out = "";
	out += TSETTINGSWRITER_P1 + std::to_string(settingsSig) + TSETTINGSWRITER_P2 + std::to_string(subSig);
	out += TSETTINGSWRITER_P3 + settingName + TSETTINGSWRITER_P4;
	out += "\"" + std::string("NULL") + "\"";
	out += TSETTINGSWRITER_P5;
	return out;
}
std::string makeSettingString(TSetNoise value, int settingsSig, int subSig, std::string settingName)
{
	std::string out = "";
	out += TSETTINGSWRITER_P1 + std::to_string(settingsSig) + TSETTINGSWRITER_P2 + std::to_string(subSig);
	out += TSETTINGSWRITER_P3 + settingName + TSETTINGSWRITER_P4;
	out += "\"" + std::string("NULL") + "\"";
	out += TSETTINGSWRITER_P5;
	return out;
}
std::string makeSettingString(TSetGraph value, int settingsSig, int subSig, std::string settingName)
{
	std::string out = "";
	out += TSETTINGSWRITER_P1 + std::to_string(settingsSig) + TSETTINGSWRITER_P2 + std::to_string(subSig);
	out += TSETTINGSWRITER_P3 + settingName + TSETTINGSWRITER_P4;
	out += "\"" + std::string("NULL") + "\"";
	out += TSETTINGSWRITER_P5;
	return out;
}
std::string makeSettingString(std::shared_ptr<CFunction> value, int settingsSig, int subSig, std::string settingName)
{
	std::string out = "";
	out += TSETTINGSWRITER_P1 + std::to_string(settingsSig) + TSETTINGSWRITER_P2 + std::to_string(subSig);
	out += TSETTINGSWRITER_P3 + settingName + TSETTINGSWRITER_P4;
	out += "\"" + std::string("NULL") + "\"";
	out += TSETTINGSWRITER_P5;
	return out;
}
std::string makeSettingString(std::shared_ptr<CObject> value, int settingsSig, int subSig, std::string settingName)
{
	std::string out = "";
	out += TSETTINGSWRITER_P1 + std::to_string(settingsSig) + TSETTINGSWRITER_P2 + std::to_string(subSig);
	out += TSETTINGSWRITER_P3 + settingName + TSETTINGSWRITER_P4;
	std::string valueToString = "";
	CLiteralTypes valType = value.get()->objType.type;
	if (valType == CLiteralTypes::_CString_Array)
	{
		std::vector<std::string> stringVec;
		std::vector<std::shared_ptr<CObject>> objVec = *std::get<std::shared_ptr<std::vector<std::shared_ptr<CObject>>>>(value.get()->obj).get();
		for (std::shared_ptr<CObject> obj : objVec)
		{
			stringVec.push_back(std::get<std::string>(obj.get()->obj));
		}
		out = makeSettingString(stringVec, settingsSig, subSig, settingName);
	}
	else
	{
		out += TSETTINGSWRITER_P1 + std::to_string(settingsSig) + TSETTINGSWRITER_P2 + std::to_string(subSig);
		out += TSETTINGSWRITER_P3 + settingName + TSETTINGSWRITER_P4;
		out += "\"" + std::string("NULL") + "\"";
		out += TSETTINGSWRITER_P5;
	}
	return out;
}