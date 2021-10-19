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

#include "../../../include/tool/toolSettings/controls/TSet_Rotate.h"

////////////////////////////////////////////////////////////////
// Rotate
////////////////////////////////////////////////////////////////

TSet_Rotate::TSet_Rotate()
{
	this->type = TSetType::rotate;
	this->isEnabled = true;
	this->rotateAboutCursor = false; // 300
	this->rotationSpeed = 1.0f; // 301
	this->snapAngleA_modKey = Keybind(InputKey::unknown, InputModKey::ctrl); // 310
	this->snapAngleA_angle = 45.0f; // 311
	this->snapAngleB_modKey = Keybind(InputKey::unknown, InputModKey::shift); // 312
	this->snapAngleB_angle = 15.0f; // 313
	this->snapAngleC_modKey = Keybind(InputKey::unknown, InputModKey::alt); // 314
	this->snapAngleC_angle = 5.0f; // 315
	this->snapTolerance_factorA = 1.05f; // 320
}

std::shared_ptr<CObject> TSet_Rotate::putProperty(
	std::shared_ptr<CInterpreter> interpreter, std::shared_ptr<Tool> owner,
	int settingsSig, int subSig, std::shared_ptr<CObject> object, bool isGet, bool asPercentage, bool asString)
{
	if (settingsSig == 300)
	{
		if (isGet)
		{
			if (asString) { return std::make_shared<CObject>(makeSettingString(rotateAboutCursor, settingsSig, subSig, "rotateAboutCursor")); }
			return std::make_shared<CObject>(rotateAboutCursor);
		}
		else
		{
			rotateAboutCursor = std::get<bool>(object.get()->obj);
		}
	}
	else if (settingsSig == 301)
	{
		if (isGet)
		{
			if (asString) { return std::make_shared<CObject>(makeSettingString(rotationSpeed, settingsSig, subSig, "rotationSpeed")); }
			if (asPercentage) {
				return std::make_shared<CObject>(double(
					percentRange(rotationSpeed, TSET_ROTATE_SPEED_MIN, TSET_ROTATE_SPEED_MAX, true)));
			}
			else { return std::make_shared<CObject>(double(rotationSpeed)); }
		}
		else
		{
			if (asPercentage) {
				rotationSpeed = percentRange(
					(float)std::get<double>(object.get()->obj), TSET_ROTATE_SPEED_MIN, TSET_ROTATE_SPEED_MAX, false);
			}
			else { rotationSpeed = (float)std::get<double>(object.get()->obj); }
		}
	}
	else if (settingsSig == 310)
	{
		if (isGet)
		{
			if (asString) { return std::make_shared<CObject>(makeSettingString(snapAngleA_modKey, settingsSig, subSig, "snapAngleA_modKey")); }
			return std::make_shared<CObject>(double(keybindToInt(snapAngleA_modKey)));
		}
		else
		{
			if (object.get()->objType.type == CLiteralTypes::_CString) {
				snapAngleA_modKey = stringToKeybind(std::get<std::string>(object.get()->obj));
			}
			else {
				snapAngleA_modKey = intToKeybind((int)std::get<double>(object.get()->obj));
			}
		}
	}
	else if (settingsSig == 311)
	{
		if (isGet)
		{
			if (asString) { return std::make_shared<CObject>(makeSettingString(snapAngleA_angle, settingsSig, subSig, "snapAngleA_angle")); }
			if (asPercentage) {
				return std::make_shared<CObject>(double(
					percentRange(snapAngleA_angle, TSET_ROTATE_ANGLESNAP_MIN, TSET_ROTATE_ANGLESNAP_MAX, true)));
			}
			else { return std::make_shared<CObject>(double(snapAngleA_angle)); }
		}
		else
		{
			if (asPercentage) {
				snapAngleA_angle = percentRange(
					(float)std::get<double>(object.get()->obj), TSET_ROTATE_ANGLESNAP_MIN, TSET_ROTATE_ANGLESNAP_MAX, false);
			}
			else { snapAngleA_angle = (float)std::get<double>(object.get()->obj); }
		}
	}
	else if (settingsSig == 312)
	{
		if (isGet)
		{
			if (asString) { return std::make_shared<CObject>(makeSettingString(snapAngleB_modKey, settingsSig, subSig, "snapAngleB_modKey")); }
			return std::make_shared<CObject>(double(keybindToInt(snapAngleB_modKey)));
		}
		else
		{
			if (object.get()->objType.type == CLiteralTypes::_CString) {
				snapAngleB_modKey = stringToKeybind(std::get<std::string>(object.get()->obj));
			}
			else {
				snapAngleB_modKey = intToKeybind((int)std::get<double>(object.get()->obj));
			}
		}
	}
	else if (settingsSig == 313)
	{
		if (isGet)
		{
			if (asString) { return std::make_shared<CObject>(makeSettingString(snapAngleB_angle, settingsSig, subSig, "snapAngleB_angle")); }
			if (asPercentage) {
				return std::make_shared<CObject>(double(
					percentRange(snapAngleB_angle, TSET_ROTATE_ANGLESNAP_MIN, TSET_ROTATE_ANGLESNAP_MAX, true)));
			}
			else { return std::make_shared<CObject>(double(snapAngleB_angle)); }
		}
		else
		{
			if (asPercentage) {
				snapAngleB_angle = percentRange(
					(float)std::get<double>(object.get()->obj), TSET_ROTATE_ANGLESNAP_MIN, TSET_ROTATE_ANGLESNAP_MAX, false);
			}
			else { snapAngleB_angle = (float)std::get<double>(object.get()->obj); }
		}
	}
	else if (settingsSig == 314)
	{
		if (isGet)
		{
			if (asString) { return std::make_shared<CObject>(makeSettingString(snapAngleC_modKey, settingsSig, subSig, "snapAngleC_modKey")); }
			return std::make_shared<CObject>(double(keybindToInt(snapAngleC_modKey)));
		}
		else
		{
			if (object.get()->objType.type == CLiteralTypes::_CString) {
				snapAngleC_modKey = stringToKeybind(std::get<std::string>(object.get()->obj));
			}
			else {
				snapAngleC_modKey = intToKeybind((int)std::get<double>(object.get()->obj));
			}
		}
	}
	else if (settingsSig == 315)
	{
		if (isGet)
		{
			if (asString) { return std::make_shared<CObject>(makeSettingString(snapAngleC_angle, settingsSig, subSig, "snapAngleC_angle")); }
			if (asPercentage) {
				return std::make_shared<CObject>(double(
					percentRange(snapAngleC_angle, TSET_ROTATE_ANGLESNAP_MIN, TSET_ROTATE_ANGLESNAP_MAX, true)));
			}
			else { return std::make_shared<CObject>(double(snapAngleC_angle)); }
		}
		else
		{
			if (asPercentage) {
				snapAngleC_angle = percentRange(
					(float)std::get<double>(object.get()->obj), TSET_ROTATE_ANGLESNAP_MIN, TSET_ROTATE_ANGLESNAP_MAX, false);
			}
			else { snapAngleC_angle = (float)std::get<double>(object.get()->obj); }
		}
	}
	else if (settingsSig == 320)
	{
		if (isGet)
		{
			if (asString) { return std::make_shared<CObject>(makeSettingString(snapTolerance_factorA, settingsSig, subSig, "snapTolerance_factorA")); }
			if (asPercentage) {
				return std::make_shared<CObject>(double(
					percentRange(snapTolerance_factorA, TSET_ROTATE_FACTORA_MIN, TSET_ROTATE_FACTORA_MAX, true)));
			}
			else { return std::make_shared<CObject>(double(snapTolerance_factorA)); }
		}
		else
		{
			if (asPercentage) {
				snapTolerance_factorA = percentRange(
					(float)std::get<double>(object.get()->obj), TSET_ROTATE_FACTORA_MIN, TSET_ROTATE_FACTORA_MAX, false);
			}
			else { snapTolerance_factorA = (float)std::get<double>(object.get()->obj); }
		}
	}

	if (asString) { return std::make_shared<CObject>(makeSettingString(isEnabled, settingsSig, subSig, "NULL")); }
	return std::make_shared<CObject>(nullptr);
}

TSetControl_Node* TSet_Rotate::getControlNode(int settingSig, int subSig)
{
	return nullptr;
}
TSetController* TSet_Rotate::getController(int settingSig, int subSig)
{
	return nullptr;
}
TSetGraph* TSet_Rotate::getGraph(int settingSig, int subSig)
{
	return nullptr;
}
TSetNoise* TSet_Rotate::getNoise(int settingSig, int subSig)
{
	return nullptr;
}