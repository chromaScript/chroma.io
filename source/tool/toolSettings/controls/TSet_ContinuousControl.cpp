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

#include "../../../include/tool/toolSettings/controls/TSet_ContinuousControl.h"

#define TSET_CONTINUOUS_ANCHORSPACING_MIN 0.5f
#define TSET_CONTINUOUS_ANCHORSPACING_MAX 500.0f
#define TSET_CONTINUOUS_ANGLERAND_MIN -180.0f
#define TSET_CONTINUOUS_ANGLERAND_MAX 180.0f
#define TSET_CONTINUOUS_SPLINERAND_MIN -500.0f
#define TSET_CONTINUOUS_SPLINERAND_MAX 500.0f
#define TSET_CONTINUOUS_CONSTRAINT_LEN_THRESH_MIN 0.5f
#define TSET_CONTINUOUS_CONSTRAINT_LEN_THRESH_MAX 20.0f
#define TSET_CONTINUOUS_CONSTRAINT_ANGLE_MIN -90.0f
#define TSET_CONTINUOUS_CONSTRAINT_ANGLE_MAX 90.0f

////////////////////////////////////////////////////////////////
// Continuous Control
////////////////////////////////////////////////////////////////

TSet_ContinuousControl::TSet_ContinuousControl()
{
	this->type = TSetType::continuous;
	this->isEnabled = true;
	//
	this->defaultMode = TSetProp::draw;
	this->trueSpacing = anchorSpacing;
	this->altAngleSnapKeyB = Keybind(InputKey::unknown, InputModKey::none);
	this->subdivide = false;
	this->subdivideAmount = 0.2f;
	this->countNode.controlMax = 0.0f;
}
std::shared_ptr<CObject> TSet_ContinuousControl::putProperty(
	std::shared_ptr<CInterpreter> interpreter, std::shared_ptr<Tool> owner,
	int settingsSig, int subSig, std::shared_ptr<CObject> object, bool isGet, bool asPercentage, bool asString)
{
	// Spacing Settings
	if (settingsSig >= 400 && settingsSig < 420)
	{
		if (settingsSig == 400) {
			if (!isGet) { defaultMode = stringToTSetProp(std::get<std::string>(object.get()->obj)); }
			else {
				if (asString) { return std::make_shared<CObject>(makeSettingString(defaultMode, settingsSig, subSig, "defaultMode")); }
				return std::make_shared<CObject>(TSetPropToString(defaultMode));
			}
		}
		else if (settingsSig == 401) {
			if (isGet) {
				if (asString) { return std::make_shared<CObject>(makeSettingString(alternateModeKey, settingsSig, subSig, "alternateModeKey")); }
				return std::make_shared<CObject>(double(keybindToInt(alternateModeKey)));
			}
			else {
				if (object.get()->objType.type == CLiteralTypes::_CString) {
					alternateModeKey = stringToKeybind(std::get<std::string>(object.get()->obj));
				}
				else {
					alternateModeKey = intToKeybind((int)std::get<double>(object.get()->obj));
				}
			}
		}
		else if (settingsSig == 402) {
			if (isGet) {
				if (asString) { return std::make_shared<CObject>(makeSettingString(altAngleSnapKeyA, settingsSig, subSig, "altAngleSnapKeyA")); }
				return std::make_shared<CObject>(double(keybindToInt(altAngleSnapKeyA)));
			}
			else {
				if (object.get()->objType.type == CLiteralTypes::_CString) {
					altAngleSnapKeyA = stringToKeybind(std::get<std::string>(object.get()->obj));
				}
				else {
					altAngleSnapKeyA = intToKeybind((int)std::get<double>(object.get()->obj));
				}
			}
		}
		else if (settingsSig == 403) {
			if (isGet) {
				if (asString) { return std::make_shared<CObject>(makeSettingString(altAngleSnapA_angle, settingsSig, subSig, "altAngleSnapA_angle")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange(altAngleSnapA_angle, TSET_CONTINUOUS_CONSTRAINT_ANGLE_MIN, TSET_CONTINUOUS_CONSTRAINT_ANGLE_MAX, true)));
				}
				else { return std::make_shared<CObject>(double(altAngleSnapA_angle)); }
			}
			else {
				if (asPercentage) {
					altAngleSnapA_angle = percentRange(
						(float)std::get<double>(object.get()->obj), TSET_CONTINUOUS_CONSTRAINT_ANGLE_MIN, TSET_CONTINUOUS_CONSTRAINT_ANGLE_MAX, false);
				}
				else {
					altAngleSnapA_angle = clampf((float)std::get<double>(object.get()->obj),
						TSET_CONTINUOUS_CONSTRAINT_ANGLE_MIN,
						TSET_CONTINUOUS_CONSTRAINT_ANGLE_MAX);
				}
			}
		}
		else if (settingsSig == 404) {
			if (isGet) {
				if (asString) { return std::make_shared<CObject>(makeSettingString(altAngleSnapKeyB, settingsSig, subSig, "altAngleSnapKeyB")); }
				return std::make_shared<CObject>(double(keybindToInt(altAngleSnapKeyB)));
			}
			else {
				if (object.get()->objType.type == CLiteralTypes::_CString) {
					altAngleSnapKeyB = stringToKeybind(std::get<std::string>(object.get()->obj));
				}
				else {
					altAngleSnapKeyB = intToKeybind((int)std::get<double>(object.get()->obj));
				}
			}
		}
		else if (settingsSig == 405) {
			if (isGet) {
				if (asString) { return std::make_shared<CObject>(makeSettingString(altAngleSnapB_angle, settingsSig, subSig, "altAngleSnapB_angle")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange(altAngleSnapB_angle, TSET_CONTINUOUS_CONSTRAINT_ANGLE_MIN, TSET_CONTINUOUS_CONSTRAINT_ANGLE_MAX, true)));
				}
				else { return std::make_shared<CObject>(double(altAngleSnapB_angle)); }
			}
			else {
				if (asPercentage) {
					altAngleSnapB_angle = percentRange(
						(float)std::get<double>(object.get()->obj), TSET_CONTINUOUS_CONSTRAINT_ANGLE_MIN, TSET_CONTINUOUS_CONSTRAINT_ANGLE_MAX, false);
				}
				else {
					altAngleSnapB_angle = clampf((float)std::get<double>(object.get()->obj),
						TSET_CONTINUOUS_CONSTRAINT_ANGLE_MIN,
						TSET_CONTINUOUS_CONSTRAINT_ANGLE_MAX);
				}
			}
		}
		else if (settingsSig == 406) {
			if (isGet) {
				if (asString) { return std::make_shared<CObject>(makeSettingString(subdivide, settingsSig, subSig, "subdivide")); }
				return std::make_shared<CObject>(subdivide);
			}
			else {
				subdivide = std::get<bool>(object.get()->obj);
			}
		}
		else if (settingsSig == 407) {
			if (isGet) {
				if (asString) { return std::make_shared<CObject>(makeSettingString(pressureBoost, settingsSig, subSig, "pressureBoost")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange(pressureBoost, 0.01f, 6.0f, true)));
				}
				else { return std::make_shared<CObject>(double(pressureBoost)); }
			}
			else {
				if (asPercentage) {
					pressureBoost = percentRange(
						(float)std::get<double>(object.get()->obj), 0.01f, 6.0f, false);
				}
				else {
					pressureBoost = clampf((float)std::get<double>(object.get()->obj), 0.01f, 6.0f);
				}
			}
		}
		else if (settingsSig == 408) {
			if (isGet) {
				if (asString) { return std::make_shared<CObject>(makeSettingString(closeShape, settingsSig, subSig, "closeShape")); }
				return std::make_shared<CObject>(closeShape);
			}
			else {
				closeShape = std::get<bool>(object.get()->obj);
			}
		}
		else if (settingsSig == 409) {
			if (isGet) {
				if (asString) { return std::make_shared<CObject>(makeSettingString(subdivideAmount, settingsSig, subSig, "subdivideAmount")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange_cubed(subdivideAmount, 0.001f, 1.0f, true)));
				}
				else { return std::make_shared<CObject>(double(subdivideAmount)); }
			}
			else {
				if (asPercentage) {
					subdivideAmount = percentRange_cubed(
						(float)std::get<double>(object.get()->obj), 0.001f, 1.0f, false);
				}
				else { subdivideAmount = clampf((float)std::get<double>(object.get()->obj), 0.001f, 1.0f); }
			}
		}
		else if (settingsSig == 410) {
			if (isGet) {
				if (asString) { return std::make_shared<CObject>(makeSettingString(connectLastStrokeKey, settingsSig, subSig, "connectLastStrokeKey")); }
				return std::make_shared<CObject>(double(keybindToInt(connectLastStrokeKey)));
			}
			else {
				if (object.get()->objType.type == CLiteralTypes::_CString) {
					connectLastStrokeKey = stringToKeybind(std::get<std::string>(object.get()->obj));
				}
				else {
					connectLastStrokeKey = intToKeybind((int)std::get<double>(object.get()->obj));
				}
			}
		}
		else if (settingsSig == 411) {
			if (isGet) {
				if (asString) { return std::make_shared<CObject>(makeSettingString(scatterConnections, settingsSig, subSig, "scatterConnections")); }
				return std::make_shared<CObject>(scatterConnections);
			}
			else {
				scatterConnections = std::get<bool>(object.get()->obj);
			}
		}
		else if (settingsSig == 415) {
			if (isGet) {
				if (asString) { return std::make_shared<CObject>(makeSettingString(anchorSpacing, settingsSig, subSig, "anchorSpacing")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange_cubed(anchorSpacing, TSET_CONTINUOUS_ANCHORSPACING_MIN, TSET_CONTINUOUS_ANCHORSPACING_MAX, true)));
				}
				else { return std::make_shared<CObject>(double(anchorSpacing)); }
			}
			else {
				if (asPercentage) {
					anchorSpacing = percentRange_cubed(
						(float)std::get<double>(object.get()->obj), TSET_CONTINUOUS_ANCHORSPACING_MIN, TSET_CONTINUOUS_ANCHORSPACING_MAX, false);
				}
				else { anchorSpacing = (float)std::get<double>(object.get()->obj); }
			}
		}
		else if (settingsSig == 416) {
			if (isGet) {
				if (asString) { return std::make_shared<CObject>(makeSettingString(relativeSpacing, settingsSig, subSig, "relativeSpacing")); }
				return std::make_shared<CObject>(relativeSpacing);
			}
			else {
				relativeSpacing = std::get<bool>(object.get()->obj);
			}
		}
		else if (settingsSig == 417) {
			if (isGet) {
				if (asString) { return std::make_shared<CObject>(makeSettingString(useCanvas, settingsSig, subSig, "useCanvas")); }
				return std::make_shared<CObject>(useCanvas);
			}
			else {
				useCanvas = std::get<bool>(object.get()->obj);
			}
		}
		else if (settingsSig == 418) {
			if (isGet) {
				if (asString) { return std::make_shared<CObject>(makeSettingString(enableAltSnapA, settingsSig, subSig, "enableAltSnapA")); }
				return std::make_shared<CObject>(enableAltSnapA);
			}
			else {
				enableAltSnapA = std::get<bool>(object.get()->obj);
			}
		}
		else if (settingsSig == 419) {
			if (isGet) {
				if (asString) { return std::make_shared<CObject>(makeSettingString(enableAltSnapB, settingsSig, subSig, "enableAltSnapB")); }
				return std::make_shared<CObject>(enableAltSnapB);
			}
			else {
				enableAltSnapB = std::get<bool>(object.get()->obj);
			}
		}
	}
	// Scatter Settings
	else if (settingsSig >= 420 && settingsSig < 440)
	{
		if (settingsSig == 420) {
			return angleNode.putProperty(interpreter, owner, settingsSig, subSig, object, isGet, asPercentage, asString);
		}
		else if (settingsSig == 421) {
			return angleController.putProperty(interpreter, owner, settingsSig, subSig, object, isGet, asPercentage, asString);
		}
		else if (settingsSig == 422) {
			return angleNoise.putProperty(interpreter, owner, settingsSig, subSig, object, isGet, asPercentage, asString);
		}
		else if (settingsSig == 423) {
			return distanceXNode.putProperty(interpreter, owner, settingsSig, subSig, object, isGet, asPercentage, asString);
		}
		else if (settingsSig == 424) {
			return distanceXController.putProperty(interpreter, owner, settingsSig, subSig, object, isGet, asPercentage, asString);
		}
		else if (settingsSig == 425) {
			return distanceXNoise.putProperty(interpreter, owner, settingsSig, subSig, object, isGet, asPercentage, asString);
		}
		else if (settingsSig == 426) {
			return distanceYNode.putProperty(interpreter, owner, settingsSig, subSig, object, isGet, asPercentage, asString);
		}
		else if (settingsSig == 427) {
			return distanceYController.putProperty(interpreter, owner, settingsSig, subSig, object, isGet, asPercentage, asString);
		}
		else if (settingsSig == 428) {
			return distanceYNoise.putProperty(interpreter, owner, settingsSig, subSig, object, isGet, asPercentage, asString);
		}
		else if (settingsSig == 429) {
			return countNode.putProperty(interpreter, owner, settingsSig, subSig, object, isGet, asPercentage, asString);
		}
		else if (settingsSig == 430) {
			return countController.putProperty(interpreter, owner, settingsSig, subSig, object, isGet, asPercentage, asString);
		}
		else if (settingsSig == 431) {
			return countNoise.putProperty(interpreter, owner, settingsSig, subSig, object, isGet, asPercentage, asString);
		}
		else if (settingsSig == 432) {
			if (!isGet) { connectionType = stringToTSetProp(std::get<std::string>(object.get()->obj)); }
			else {
				if (asString) { return std::make_shared<CObject>(makeSettingString(connectionType, settingsSig, subSig, "connectionType")); }
				return std::make_shared<CObject>(TSetPropToString(connectionType));
			}
		}
		else if (settingsSig == 433) {
			if (!isGet) {
				if (asPercentage) {
					connectInterval = (int)percentRange_cubed(
						(float)std::get<double>(object.get()->obj), 1.0f, 250.0f, false);
				}
				else { connectInterval = clampi((int)std::get<double>(object.get()->obj), 1, 250.0f); }
			}
			else {
				if (asString) { return std::make_shared<CObject>(makeSettingString(connectInterval, settingsSig, subSig, "connectInterval")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange_cubed((float)connectInterval, 1.0f, 250.0f, true)));
				}
				else { return std::make_shared<CObject>(double(connectInterval)); }
			}
		}
		else if (settingsSig == 434) {
			if (isGet) {
				if (asString) { return std::make_shared<CObject>(makeSettingString(useAngle, settingsSig, subSig, "useAngle")); }
				return std::make_shared<CObject>(useAngle);
			}
			else {
				useAngle = std::get<bool>(object.get()->obj);
			}
		}
		else if (settingsSig == 435) {
			if (isGet) {
				if (asString) { return std::make_shared<CObject>(makeSettingString(useDirection, settingsSig, subSig, "useDirection")); }
				return std::make_shared<CObject>(useDirection);
			}
			else {
				useDirection = std::get<bool>(object.get()->obj);
			}
		}
		else if (settingsSig == 436) {
			if (!isGet) {
				if (asPercentage) {
					offsetAngle = (int)percentRange(
						(float)std::get<double>(object.get()->obj), -180.0f, 180.0f, false);
				}
				else { offsetAngle = clampi((int)std::get<double>(object.get()->obj), -180.0f, 180.0f); }
			}
			else {
				if (asString) { return std::make_shared<CObject>(makeSettingString(offsetAngle, settingsSig, subSig, "offsetAngle")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange((float)offsetAngle, -180.0f, 180.0f, true)));
				}
				else { return std::make_shared<CObject>(double(offsetAngle)); }
			}
		}
		else if (settingsSig == 437) {
			if (isGet) {
				if (asString) { return std::make_shared<CObject>(makeSettingString(relativeDistance, settingsSig, subSig, "relativeDistance")); }
				return std::make_shared<CObject>(relativeDistance);
			}
			else {
				relativeDistance = std::get<bool>(object.get()->obj);
			}
		}
	}
	// Constraint Length Threshold
	else if (settingsSig == 440)
	{
		if (isGet) {
			if (asString) { return std::make_shared<CObject>(makeSettingString(constraintLengthThreshold, settingsSig, subSig, "constraintLengthThreshold")); }
			if (asPercentage) {
				return std::make_shared<CObject>(double(
					percentRange(constraintLengthThreshold, TSET_CONTINUOUS_CONSTRAINT_LEN_THRESH_MIN, TSET_CONTINUOUS_CONSTRAINT_LEN_THRESH_MAX, true)));
			}
			else { return std::make_shared<CObject>(double(constraintLengthThreshold)); }
		}
		else {
			if (asPercentage) {
				constraintLengthThreshold = percentRange(
					(float)std::get<double>(object.get()->obj), TSET_CONTINUOUS_CONSTRAINT_LEN_THRESH_MIN, TSET_CONTINUOUS_CONSTRAINT_LEN_THRESH_MAX, false);
			}
			else {
				constraintLengthThreshold = clampf(
					(float)std::get<double>(object.get()->obj), TSET_CONTINUOUS_CONSTRAINT_LEN_THRESH_MIN, TSET_CONTINUOUS_CONSTRAINT_LEN_THRESH_MAX);
			}
		}
	}
	// Constraint Key and Angle Settings
	else if (settingsSig >= 441 && settingsSig <= 457)
	{
		if (settingsSig == 441) {
			if (isGet) {
				if (asString) { return std::make_shared<CObject>(makeSettingString(enableConstrainA, settingsSig, subSig, "enableConstrainA")); }
				return std::make_shared<CObject>(enableConstrainA);
			}
			else {
				enableConstrainA = std::get<bool>(object.get()->obj);
			}
		}
		else if (settingsSig == 442) {
			if (isGet) {
				if (asString) { return std::make_shared<CObject>(makeSettingString(constrainA_angle, settingsSig, subSig, "constrainA_angle")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange(constrainA_angle, TSET_CONTINUOUS_CONSTRAINT_ANGLE_MIN, TSET_CONTINUOUS_CONSTRAINT_ANGLE_MAX, true)));
				}
				else { return std::make_shared<CObject>(double(constrainA_angle)); }
			}
			else {
				if (asPercentage) {
					constrainA_angle = percentRange(
						(float)std::get<double>(object.get()->obj), TSET_CONTINUOUS_CONSTRAINT_ANGLE_MIN, TSET_CONTINUOUS_CONSTRAINT_ANGLE_MAX, false);
				}
				else {
					constrainA_angle = clampf((float)std::get<double>(object.get()->obj), TSET_CONTINUOUS_CONSTRAINT_ANGLE_MIN, TSET_CONTINUOUS_CONSTRAINT_ANGLE_MAX);
				}
			}
		}
		else if (settingsSig == 443) {
			if (isGet) {
				if (asString) { return std::make_shared<CObject>(makeSettingString(constrainA_modKey, settingsSig, subSig, "constrainA_modKey")); }
				return std::make_shared<CObject>(double(keybindToInt(constrainA_modKey)));
			}
			else {
				if (object.get()->objType.type == CLiteralTypes::_CString) {
					constrainA_modKey = stringToKeybind(std::get<std::string>(object.get()->obj));
				}
				else {
					constrainA_modKey = intToKeybind((int)std::get<double>(object.get()->obj));
				}
			}
		}
		else if (settingsSig == 445) {
			if (isGet) {
				if (asString) { return std::make_shared<CObject>(makeSettingString(enableConstrainB, settingsSig, subSig, "enableConstrainB")); }
				return std::make_shared<CObject>(enableConstrainB);
			}
			else {
				enableConstrainB = std::get<bool>(object.get()->obj);
			}
		}
		else if (settingsSig == 446) {
			if (isGet) {
				if (asString) { return std::make_shared<CObject>(makeSettingString(constrainB_angle, settingsSig, subSig, "constrainB_angle")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange(constrainB_angle, TSET_CONTINUOUS_CONSTRAINT_ANGLE_MIN, TSET_CONTINUOUS_CONSTRAINT_ANGLE_MAX, true)));
				}
				else { return std::make_shared<CObject>(double(constrainB_angle)); }
			}
			else {
				if (asPercentage) {
					constrainB_angle = percentRange(
						(float)std::get<double>(object.get()->obj), TSET_CONTINUOUS_CONSTRAINT_ANGLE_MIN, TSET_CONTINUOUS_CONSTRAINT_ANGLE_MAX, false);
				}
				else {
					constrainB_angle = clampf((float)std::get<double>(object.get()->obj), TSET_CONTINUOUS_CONSTRAINT_ANGLE_MIN, TSET_CONTINUOUS_CONSTRAINT_ANGLE_MAX);
				}
			}
		}
		else if (settingsSig == 447) {
			if (isGet) {
				if (asString) { return std::make_shared<CObject>(makeSettingString(constrainB_modKey, settingsSig, subSig, "constrainB_modKey")); }
				return std::make_shared<CObject>(double(keybindToInt(constrainB_modKey)));
			}
			else {
				if (object.get()->objType.type == CLiteralTypes::_CString) {
					constrainB_modKey = stringToKeybind(std::get<std::string>(object.get()->obj));
				}
				else {
					constrainB_modKey = intToKeybind((int)std::get<double>(object.get()->obj));
				}
			}
		}
		else if (settingsSig == 450) {
			if (isGet) {
				if (asString) { return std::make_shared<CObject>(makeSettingString(enableConstrainC, settingsSig, subSig, "enableConstrainC")); }
				return std::make_shared<CObject>(enableConstrainC);
			}
			else {
				enableConstrainC = std::get<bool>(object.get()->obj);
			}
		}
		else if (settingsSig == 451) {
			if (isGet) {
				if (asString) { return std::make_shared<CObject>(makeSettingString(constrainC_angle, settingsSig, subSig, "constrainC_angle")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange(constrainC_angle, TSET_CONTINUOUS_CONSTRAINT_ANGLE_MIN, TSET_CONTINUOUS_CONSTRAINT_ANGLE_MAX, true)));
				}
				else { return std::make_shared<CObject>(double(constrainC_angle)); }
			}
			else {
				if (asPercentage) {
					constrainC_angle = percentRange(
						(float)std::get<double>(object.get()->obj), TSET_CONTINUOUS_CONSTRAINT_ANGLE_MIN, TSET_CONTINUOUS_CONSTRAINT_ANGLE_MAX, false);
				}
				else {
					constrainC_angle = clampf((float)std::get<double>(object.get()->obj), TSET_CONTINUOUS_CONSTRAINT_ANGLE_MIN, TSET_CONTINUOUS_CONSTRAINT_ANGLE_MAX);
				}
			}
		}
		else if (settingsSig == 452) {
			if (isGet) {
				if (asString) { return std::make_shared<CObject>(makeSettingString(constrainC_modKey, settingsSig, subSig, "constrainC_modKey")); }
				return std::make_shared<CObject>(double(keybindToInt(constrainC_modKey)));
			}
			else {
				if (object.get()->objType.type == CLiteralTypes::_CString) {
					constrainC_modKey = stringToKeybind(std::get<std::string>(object.get()->obj));
				}
				else {
					constrainC_modKey = intToKeybind((int)std::get<double>(object.get()->obj));
				}
			}
		}
		else if (settingsSig == 455) {
			if (isGet) {
				if (asString) { return std::make_shared<CObject>(makeSettingString(enableConstrainD, settingsSig, subSig, "enableConstrainD")); }
				return std::make_shared<CObject>(enableConstrainD);
			}
			else {
				enableConstrainD = std::get<bool>(object.get()->obj);
			}
		}
		else if (settingsSig == 456) {
			if (isGet) {
				if (asString) { return std::make_shared<CObject>(makeSettingString(constrainD_angle, settingsSig, subSig, "constrainD_angle")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange(constrainD_angle, TSET_CONTINUOUS_CONSTRAINT_ANGLE_MIN, TSET_CONTINUOUS_CONSTRAINT_ANGLE_MAX, true)));
				}
				else { return std::make_shared<CObject>(double(constrainD_angle)); }
			}
			else {
				if (asPercentage) {
					constrainD_angle = percentRange(
						(float)std::get<double>(object.get()->obj), TSET_CONTINUOUS_CONSTRAINT_ANGLE_MIN, TSET_CONTINUOUS_CONSTRAINT_ANGLE_MAX, false);
				}
				else {
					constrainD_angle = clampf((float)std::get<double>(object.get()->obj), TSET_CONTINUOUS_CONSTRAINT_ANGLE_MIN, TSET_CONTINUOUS_CONSTRAINT_ANGLE_MAX);
				}
			}
		}
		else if (settingsSig == 457) {
			if (isGet) {
				if (asString) { return std::make_shared<CObject>(makeSettingString(constrainD_modKey, settingsSig, subSig, "constrainD_modKey")); }
				return std::make_shared<CObject>(double(keybindToInt(constrainD_modKey)));
			}
			else {
				if (object.get()->objType.type == CLiteralTypes::_CString) {
					constrainD_modKey = stringToKeybind(std::get<std::string>(object.get()->obj));
				}
				else {
					constrainD_modKey = intToKeybind((int)std::get<double>(object.get()->obj));
				}
			}
		}
	}

	if (asString) { return std::make_shared<CObject>(makeSettingString(isEnabled, settingsSig, subSig, "NULL")); }
	return std::make_shared<CObject>(nullptr);
}

TSetControl_Node* TSet_ContinuousControl::getControlNode(int settingSig, int subSig)
{
	if (settingSig == 420) { return &angleNode; }
	else if (settingSig == 423) { return &distanceXNode; }
	else if (settingSig == 426) { return &distanceYNode; }
	else if (settingSig == 429) { return &countNode; }
	return nullptr;
}
TSetController* TSet_ContinuousControl::getController(int settingSig, int subSig)
{
	if (settingSig == 421) { return &angleController; }
	else if (settingSig == 424) { return &distanceXController; }
	else if (settingSig == 427) { return &distanceYController; }
	else if (settingSig == 430) { return &countController; }
	return nullptr;
}
TSetGraph* TSet_ContinuousControl::getGraph(int settingSig, int subSig)
{
	if (settingSig == 421)
	{
		if (subSig == -1) { return &angleController.pressureGraph; }
		else if (subSig == -2) { return &angleController.directionGraph; }
		else if (subSig == -3) { return &angleController.tiltGraph; }
		else if (subSig == -4) { return &angleController.velocityGraph; }
		else if (subSig == -5) { return &angleController.rotationGraph; }
	}
	else if (settingSig == 424)
	{
		if (subSig == -1) { return &distanceXController.pressureGraph; }
		else if (subSig == -2) { return &distanceXController.directionGraph; }
		else if (subSig == -3) { return &distanceXController.tiltGraph; }
		else if (subSig == -4) { return &distanceXController.velocityGraph; }
		else if (subSig == -5) { return &distanceXController.rotationGraph; }
	}
	else if (settingSig == 427)
	{
		if (subSig == -1) { return &distanceYController.pressureGraph; }
		else if (subSig == -2) { return &distanceYController.directionGraph; }
		else if (subSig == -3) { return &distanceYController.tiltGraph; }
		else if (subSig == -4) { return &distanceYController.velocityGraph; }
		else if (subSig == -5) { return &distanceYController.rotationGraph; }
	}
	else if (settingSig == 430)
	{
		if (subSig == -1) { return &countController.pressureGraph; }
		else if (subSig == -2) { return &countController.directionGraph; }
		else if (subSig == -3) { return &countController.tiltGraph; }
		else if (subSig == -4) { return &countController.velocityGraph; }
		else if (subSig == -5) { return &countController.rotationGraph; }
	}
	return nullptr;
}
TSetNoise* TSet_ContinuousControl::getNoise(int settingSig, int subSig)
{
	if (settingSig == 422) { return &angleNoise; }
	else if (settingSig == 425) { return &distanceXNoise; }
	else if (settingSig == 428) { return &distanceYNoise; }
	else if (settingSig == 431) { return &countNoise; }
	return nullptr;
}

void TSet_ContinuousControl::clearConstraint()
{
	activated = false;
	line = glm::vec4(0);
	origin = dir = perpendicular = glm::vec3(0);
	activeAngle = 0.0f;
	activeKey = Keybind();
	//std::cout << "CONTINUOUS_CONTROL::CLEAR_CONSTRAINT" << std::endl;
}
void TSet_ContinuousControl::updateTrueSpacing(std::shared_ptr<Tool> owner, int canvasWidth, int canvasHeight)
{
	if (relativeSpacing)
	{
		if (useCanvas)
		{
			trueSpacing = (anchorSpacing / 1000.0f) * (sqrtf(powf((float)canvasWidth, 2) + powf((float)canvasHeight, 2)));
		}
		else if (owner.get()->checkInterestMask(TSetType::image) && owner.get()->getImage()->isEnabled)
		{
			trueSpacing = (anchorSpacing / 10.0f) * owner.get()->getImage()->tipSize;
		}
		else
		{
			trueSpacing = anchorSpacing;
		}
	}
	else
	{
		trueSpacing = anchorSpacing;
	}
	trueSpacing = clampf(trueSpacing, TSET_CONTINUOUS_ANCHORSPACING_MIN, TSET_CONTINUOUS_ANCHORSPACING_MAX);
}

void TSet_ContinuousControl::initialize(float* trueSpacing, int* entityCount, int* tipSize, glm::ivec2* canvasDimensions)
{
	if (!isEnabled) { return; }
	if (angleNoise.isEnabled) { angleNoise.initialize(trueSpacing, entityCount, canvasDimensions); }
	//
	if (distanceXNoise.isEnabled) { distanceXNoise.initialize(trueSpacing, entityCount, canvasDimensions); }
	if (distanceYNoise.isEnabled) { distanceYNoise.initialize(trueSpacing, entityCount, canvasDimensions); }
	//
	if (countNoise.isEnabled) { countNoise.initialize(trueSpacing, entityCount, canvasDimensions); }
}

glm::vec3 TSet_ContinuousControl::modulatePosition(
	glm::vec3& finalPos, int* tipSize,
	glm::vec3* pos, glm::vec3* origin, glm::vec3* dir, Input* input,
	int* shardCount, int* anchorCount, int* splineCount)
{
	glm::vec3 outPos = *pos;
	float tip = float(*tipSize);

	float controlX = 1.0f; float controlY = 1.0f;
	float offsetX = 1.0f; float offsetY = 1.0f;
	bool xNoiseEnabled = false; bool yNoiseEnabled = false;
	bool xControlEnabled = false; bool yControlEnabled = false;

	// Calculate distance amounts
	// X DIST - Use shuffleSeed option on calculateNoise to prevent shard-stacking
	if (distanceXNode.reportEnabled()) {
		xControlEnabled = true;
		distanceXNode.calculateControl(controlX, pos, dir, input, &distanceXController, anchorCount);
	}
	if (distanceXNoise.reportEnabled()) {
		xNoiseEnabled = true;
		distanceXNoise.calculateNoise(offsetX, pos, origin, dir, input, shardCount, anchorCount, splineCount, true);
	}
	else if (xControlEnabled) {
		offsetX = controlX;
	}
	//
	if (!xControlEnabled && !xNoiseEnabled) { offsetX = 0.0f; }
	else {
		offsetX = lerpf(
			distanceXNode.controlMin * distRange * ((xControlEnabled && !xNoiseEnabled) ? 1.0f : controlX),
			distanceXNode.controlMax * distRange * ((xControlEnabled && !xNoiseEnabled) ? 1.0f : controlX),
			offsetX);
	}
	// Y DIST - Use shuffleSeed option on calculateNoise to prevent shard-stacking
	if (distanceYNode.reportEnabled()) {
		yControlEnabled = true;
		distanceYNode.calculateControl(controlY, pos, dir, input, &distanceYController, anchorCount);
	}
	if (distanceYNoise.reportEnabled()) {
		yNoiseEnabled = true;
		distanceYNoise.calculateNoise(offsetY, pos, origin, dir, input, shardCount, anchorCount, splineCount, true);
	}
	else if (yControlEnabled) {
		offsetY = controlY;
	}
	//
	if (!yControlEnabled && !yNoiseEnabled) { offsetY = 0.0f; }
	else {
		offsetY = lerpf(
			distanceYNode.controlMin * distRange * ((yControlEnabled && !yNoiseEnabled) ? 1.0f : controlY),
			distanceYNode.controlMax * distRange * ((yControlEnabled && !yNoiseEnabled) ? 1.0f : controlY),
			offsetY);
	}

	if (!useAngle)
	{
		outPos.x = outPos.x + offsetX;
		outPos.y = outPos.y + offsetY;
	}
	else
	{
		float angleControl = 1.0f;
		if (angleNode.reportEnabled()) {
			angleNode.calculateControl(angleControl, pos, dir, input, &distanceXController, anchorCount);
		}
		if (angleNoise.isEnabled) {
			angleNoise.calculateNoise(angleControl, pos, origin, dir, input, shardCount, anchorCount, splineCount, true);
		}
		float outAngle = -clampAngle_180(lerpf(angleNode.controlMin * 180.0f, angleNode.controlMax * 180.0f, angleControl) + offsetAngle);
		glm::vec3 inDir = (useDirection) ? *dir : glm::vec3(cos(MATH_RAD_90), sin(MATH_RAD_90), 0.0f);
		glm::vec3 rotDir = glm::vec3(
			inDir.x * cos(outAngle * (MATH_PI / 180.0f)) - -inDir.y * sin(outAngle * (MATH_PI / 180.0f)),
			inDir.x * sin(outAngle * (MATH_PI / 180.0f)) + -inDir.y * cos(outAngle * (MATH_PI / 180.0f)),
			inDir.z);
		outPos += (rotDir * offsetX);
		glm::vec3 rotPerpDir = glm::vec3(
			rotDir.x * cos(MATH_RAD_90) - -rotDir.y * sin(MATH_RAD_90),
			rotDir.x * sin(MATH_RAD_90) + -rotDir.y * cos(MATH_RAD_90),
			rotDir.z);
		outPos += (rotPerpDir * offsetY);
	}

	finalPos = outPos;
	return finalPos;
}
int TSet_ContinuousControl::modulateCount(int& outCount,
	glm::vec3* pos, glm::vec3* origin, glm::vec3* dir, Input* input,
	int* shardCount, int* anchorCount, int* splineCount)
{
	int count = 1;

	float countValue = 1.0f;
	bool countEnabled = false;
	if (countNode.reportEnabled()) {
		countEnabled = true;
		countNode.calculateControl(countValue, pos, dir, input, &countController, anchorCount);
	}
	if (countNoise.isEnabled) {
		countEnabled = true;
		countNoise.calculateNoise(countValue, pos, origin, dir, input, shardCount, anchorCount, splineCount, false);
	}
	if (countEnabled) {
		count = clampi(int(countRange * lerpf(countNode.controlMin, countNode.controlMax, countValue)), 1, int(countRange));
	}
	else {
		count = clampi(int(countRange * countNode.controlMax), 1, int(countRange));
	}
	return count;
}