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

#include "../../../include/tool/toolSettings/controls/TSet_Pan.h"

////////////////////////////////////////////////////////////////
// Pan
////////////////////////////////////////////////////////////////

TSet_Pan::TSet_Pan()
{
	this->type = TSetType::pan;
	this->isEnabled = true;
	this->constrainToCanvas = false; // 100
	this->constrainBorderRelative = false; // 101
	this->constrainBorderAmount = 100.0f; // 102
	this->doFlickPanning = false; // 101
	this->friction = 0.0f; // 102
	this->force = 1000.0f; // 103
	this->speedAmount = 1.0f; // 107
	this->slowModKey = Keybind(InputKey::unknown, InputModKey::alt); // 110
	this->slowFactor = 0.85f; // 109
	this->clickTargetPanning = false; // 111
	this->clickTarModKey = Keybind(); // 113
	this->lockToX = false; // 114
	this->lockXModKey = Keybind(); // 115
	this->lockToY = false; // 116
	this->lockYModKey = Keybind(); // 117
	this->lockCustomAngle = false; // 118
	this->customAngle = 22.5f; // 119
	this->lockCustomModKey = Keybind(); // 120
	this->cameraPosRecording = false; // 121
	this->cameraPosRecordKey = Keybind(); // 122
}

std::shared_ptr<CObject> TSet_Pan::putProperty(
	std::shared_ptr<CInterpreter> interpreter, std::shared_ptr<Tool> owner,
	int settingsSig, int subSig, std::shared_ptr<CObject> object, bool isGet, bool asPercentage, bool asString)
{
	// Canvas Constraint
	if (settingsSig >= 100 && settingsSig <= 109)
	{
		if (settingsSig == 100)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(constrainToCanvas, settingsSig, subSig, "constrainToCanvas")); }
				return std::make_shared<CObject>(constrainToCanvas);
			}
			else
			{
				constrainToCanvas = std::get<bool>(object.get()->obj);
			}
		}
		else if (settingsSig == 101)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(constrainBorderRelative, settingsSig, subSig, "constrainBorderRelative")); }
				return std::make_shared<CObject>(constrainBorderRelative);
			}
			else
			{
				constrainBorderRelative = std::get<bool>(object.get()->obj);
			}
		}
		else if (settingsSig == 102)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(constrainBorderAmount, settingsSig, subSig, "constrainBorderAmount")); }
				if (asPercentage)
				{
					return std::make_shared<CObject>(double(
						percentRange_cubed(constrainBorderAmount, TSET_PAN_CONSTRAINBORDER_MIN, TSET_PAN_CONSTRAINBORDER_MAX, true)));
				}
				else { return std::make_shared<CObject>(double(constrainBorderAmount)); }

			}
			else
			{
				if (asPercentage) {
					constrainBorderAmount = percentRange_cubed((float)std::get<double>(object.get()->obj), TSET_PAN_CONSTRAINBORDER_MIN, TSET_PAN_CONSTRAINBORDER_MAX, false);
				}
				else { constrainBorderAmount = clampf((float)std::get<double>(object.get()->obj), TSET_PAN_CONSTRAINBORDER_MIN, TSET_PAN_CONSTRAINBORDER_MAX); }
			}
		}
	}
	// Friction and Speed
	else if (settingsSig >= 110 && settingsSig <= 119)
	{
		if (settingsSig == 110)
		{
			if (asString) { return std::make_shared<CObject>(makeSettingString(doFlickPanning, settingsSig, subSig, "doFlickPanning")); }
			if (isGet) { return std::make_shared<CObject>(doFlickPanning); }
			else { doFlickPanning = std::get<bool>(object.get()->obj); }
		}
		else if (settingsSig == 111)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(friction, settingsSig, subSig, "friction")); }
				if (asPercentage)
				{
					return std::make_shared<CObject>(double(
						percentRange_cubed(friction, TSET_PAN_FRICTION_MIN, TSET_PAN_FRICTION_MAX, true)));
				}
				else { return std::make_shared<CObject>(double(friction)); }

			}
			else
			{
				if (asPercentage) {
					friction = percentRange_cubed((float)std::get<double>(object.get()->obj), TSET_PAN_FRICTION_MIN, TSET_PAN_FRICTION_MAX, false);
				}
				else { friction = clampf((float)std::get<double>(object.get()->obj), TSET_PAN_FRICTION_MIN, TSET_PAN_FRICTION_MAX); }
			}
		}
		else if (settingsSig == 112)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(force, settingsSig, subSig, "force")); }
				if (asPercentage)
				{
					return std::make_shared<CObject>(double(
						percentRange_squared(force, TSET_PAN_FORCE_MIN, TSET_PAN_FORCE_MAX, true)));
				}
				else { return std::make_shared<CObject>(double(force)); }

			}
			else
			{
				if (asPercentage) {
					force = percentRange_squared((float)std::get<double>(object.get()->obj), TSET_PAN_FORCE_MIN, TSET_PAN_FORCE_MAX, false);
				}
				else { force = (float)std::get<double>(object.get()->obj); }
			}
		}
		else if (settingsSig == 113)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(speedAmount, settingsSig, subSig, "speedAmount")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange_squared(speedAmount, TSET_PAN_SPEEDAMOUNT_MIN, TSET_PAN_SPEEDAMOUNT_MAX, true)));
				}
				else { return std::make_shared<CObject>(double(speedAmount)); }

			}
			else
			{
				if (asPercentage) {
					speedAmount = percentRange_squared(
						(float)std::get<double>(object.get()->obj), TSET_PAN_SPEEDAMOUNT_MIN, TSET_PAN_SPEEDAMOUNT_MAX, false);
				}
				else { speedAmount = clampf((float)std::get<double>(object.get()->obj), TSET_PAN_SPEEDAMOUNT_MIN, TSET_PAN_SPEEDAMOUNT_MAX); }
			}
		}
		else if (settingsSig == 116)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(slowModKey, settingsSig, subSig, "slowModKey")); }
				return std::make_shared<CObject>(double(keybindToInt(slowModKey)));
			}
			else
			{
				if (object.get()->objType.type == CLiteralTypes::_CString) {
					slowModKey = stringToKeybind(std::get<std::string>(object.get()->obj));
				}
				else {
					slowModKey = intToKeybind((int)std::get<double>(object.get()->obj));
				}
			}
		}
		else if (settingsSig == 117)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(slowFactor, settingsSig, subSig, "slowFactor")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange(slowFactor, TSET_PAN_SLOWFACTOR_MIN, TSET_PAN_SLOWFACTOR_MAX, true)));
				}
				else { return std::make_shared<CObject>(double(slowFactor)); }

			}
			else
			{
				if (asPercentage) {
					slowFactor = percentRange(
						(float)std::get<double>(object.get()->obj), TSET_PAN_SLOWFACTOR_MIN, TSET_PAN_SLOWFACTOR_MAX, false);
				}
				else { slowFactor = clampf((float)std::get<double>(object.get()->obj), TSET_PAN_SLOWFACTOR_MIN, TSET_PAN_SLOWFACTOR_MAX); }
			}
		}
	}
	// Click Targeting
	else if (settingsSig >= 120 && settingsSig <= 129)
	{
		if (settingsSig == 120)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(clickTargetPanning, settingsSig, subSig, "clickTargetPanning")); }
				return std::make_shared<CObject>(clickTargetPanning);
			}
			else
			{
				clickTargetPanning = std::get<bool>(object.get()->obj);
			}
		}
		else if (settingsSig == 122)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(clickTarModKey, settingsSig, subSig, "clickTarModKey")); }
				return std::make_shared<CObject>(double(keybindToInt(clickTarModKey)));
			}
			else
			{
				if (object.get()->objType.type == CLiteralTypes::_CString) {
					clickTarModKey = stringToKeybind(std::get<std::string>(object.get()->obj));
				}
				else {
					clickTarModKey = intToKeybind((int)std::get<double>(object.get()->obj));
				}
			}
		}
	}
	// Lock Axis Settings
	else if (settingsSig >= 130 && settingsSig <= 159)
	{
		if (settingsSig == 130)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(lockToX, settingsSig, subSig, "lockToX")); }
				return std::make_shared<CObject>(lockToX);
			}
			else
			{
				lockToX = std::get<bool>(object.get()->obj);
			}
		}
		else if (settingsSig == 131)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(lockXModKey, settingsSig, subSig, "lockXModKey")); }
				return std::make_shared<CObject>(double(keybindToInt(lockXModKey)));
			}
			else
			{
				if (object.get()->objType.type == CLiteralTypes::_CString) {
					lockXModKey = stringToKeybind(std::get<std::string>(object.get()->obj));
				}
				else {
					lockXModKey = intToKeybind((int)std::get<double>(object.get()->obj));
				}
			}
		}
		else if (settingsSig == 132)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(lockToY, settingsSig, subSig, "lockToY")); }
				return std::make_shared<CObject>(lockToY);
			}
			else
			{
				lockToY = std::get<bool>(object.get()->obj);
			}
		}
		else if (settingsSig == 133)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(lockYModKey, settingsSig, subSig, "lockYModKey")); }
				return std::make_shared<CObject>(double(keybindToInt(lockYModKey)));
			}
			else
			{
				if (object.get()->objType.type == CLiteralTypes::_CString) {
					lockYModKey = stringToKeybind(std::get<std::string>(object.get()->obj));
				}
				else {
					lockYModKey = intToKeybind((int)std::get<double>(object.get()->obj));
				}
			}
		}
		else if (settingsSig == 134)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(lockCustomAngle, settingsSig, subSig, "lockCustomAngle")); }
				return std::make_shared<CObject>(lockCustomAngle);
			}
			else
			{
				lockCustomAngle = std::get<bool>(object.get()->obj);
			}
		}
		else if (settingsSig == 135)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(customAngle, settingsSig, subSig, "customAngle")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange(customAngle, TSET_PAN_CUSTOMANGLE_MIN, TSET_PAN_CUSTOMANGLE_MAX, true)));
				}
				else { return std::make_shared<CObject>(double(customAngle)); }

			}
			else
			{
				if (asPercentage) {
					customAngle = percentRange(
						(float)std::get<double>(object.get()->obj), TSET_PAN_CUSTOMANGLE_MIN, TSET_PAN_CUSTOMANGLE_MAX, false);
				}
				else { customAngle = clampf((float)std::get<double>(object.get()->obj), TSET_PAN_CUSTOMANGLE_MIN, TSET_PAN_CUSTOMANGLE_MAX); }
			}
		}
		else if (settingsSig == 136)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(lockCustomModKey, settingsSig, subSig, "lockCustomModKey")); }
				return std::make_shared<CObject>(double(keybindToInt(lockCustomModKey)));
			}
			else
			{
				if (object.get()->objType.type == CLiteralTypes::_CString) {
					lockCustomModKey = stringToKeybind(std::get<std::string>(object.get()->obj));
				}
				else {
					lockCustomModKey = intToKeybind((int)std::get<double>(object.get()->obj));
				}
			}
		}
		else if (settingsSig == 150)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(cameraPosRecording, settingsSig, subSig, "cameraPosRecording")); }
				return std::make_shared<CObject>(cameraPosRecording);
			}
			else
			{
				cameraPosRecording = std::get<bool>(object.get()->obj);
			}
		}
		else if (settingsSig == 151)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(cameraPosRecordKey, settingsSig, subSig, "cameraPosRecordKey")); }
				return std::make_shared<CObject>(double(keybindToInt(cameraPosRecordKey)));
			}
			else
			{
				if (object.get()->objType.type == CLiteralTypes::_CString) {
					cameraPosRecordKey = stringToKeybind(std::get<std::string>(object.get()->obj));
				}
				else {
					cameraPosRecordKey = intToKeybind((int)std::get<double>(object.get()->obj));
				}
			}
		}
	}

	if (asString) { return std::make_shared<CObject>(makeSettingString(isEnabled, settingsSig, subSig, "NULL")); }
	return std::make_shared<CObject>(nullptr);
}

TSetControl_Node* TSet_Pan::getControlNode(int settingSig, int subSig)
{
	return nullptr;
}
TSetController* TSet_Pan::getController(int settingSig, int subSig)
{
	return nullptr;
}
TSetGraph* TSet_Pan::getGraph(int settingSig, int subSig)
{
	return nullptr;
}
TSetNoise* TSet_Pan::getNoise(int settingSig, int subSig)
{
	return nullptr;
}