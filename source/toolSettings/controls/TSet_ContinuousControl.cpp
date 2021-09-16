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
#include "../../include/toolSettings/controls/TSet_ContinuousControl.h"

////////////////////////////////////////////////////////////////
// Continuous Control
////////////////////////////////////////////////////////////////

TSet_ContinuousControl::TSet_ContinuousControl()
{
	this->type = TSetType::continuous;
	this->isEnabled = true;
	// Input Mode
	this->defaultMode = TSetProp::draw; // draw / polyLine
	this->alternateModeKey = INPUT_MOD_ALT;
	//
	this->fillOnShiftClick = true;
	this->fillOnPolyDraw = false;
	this->connectPropertiesWeighting = 0.85f;
	//
	this->relativeSpacing = true;
	this->useCanvas = false;
	this->anchorSpacing = 5.0f;
	this->trueSpacing = anchorSpacing;
	this->useAngle = false; // 405
	this->angleMin = 0.0f; // 406
	this->angleMax = 0.0f; // 407
	this->angleBias = 0.0f; // 408
	this->useDirection = false; // 410
	this->relativeDistance = false; // 411
	this->splineRandomXMin = 0.0f; // 412
	this->splineRandomXMax = 0.0f; // 413
	this->splineRandomXBias = 0.0f; // 414
	this->splineRandomYMin = 0.0f; // 415
	this->splineRandomYMax = 0.0f; // 416
	this->splineRandomYBias = 0.0f; // 417
	// Constraint Global
	this->constraintLengthThreshold = 1.2f; // 419
	// Constrathis->Angle A
	this->enableConstrainA = true; // 420
	this->constrainA_angle = 0.0f; // 421
	this->constrainA_modKey = INPUT_MOD_SHIFT; // 422
	// Constrathis->Angle B
	this->enableConstrainB = true; // 423
	this->constrainB_angle = 90.0f; // 424
	this->constrainB_modKey = INPUT_MOD_SHIFT; // 425
	// Constrathis->Angle C
	this->enableConstrainC = true; // 426
	this->constrainC_angle = 45.0f; // 427
	this->constrainC_modKey = INPUT_MOD_SHIFT; // 428
	// Constrathis->Angle D
	this->enableConstrainD = true; // 429
	this->constrainD_angle = -45.0f; // 430
	this->constrainD_modKey = INPUT_MOD_SHIFT; // 431
	// Hotswap Angle A
	this->enableHotswapA = false; // 450
	this->hotswapA_toolID = 0.0f; // 451
	this->hotswapA_modKey = INPUT_KEY_UNKNOWN; // 452
	// Hotswap Angle B
	this->enableHotswapB = false; // 453
	this->hotswapB_toolID = 0.0f; // 454
	this->hotswapB_modKey = INPUT_KEY_UNKNOWN; // 455
	// Hotswap Angle C
	this->enableHotswapC = false; // 456
	this->hotswapC_toolID = 0.0f; // 457
	this->hotswapC_modKey = INPUT_KEY_UNKNOWN; // 458
	// Hotswap Angle D
	this->enableHotswapD = false; // 459
	this->hotswapD_toolID = 0.0f; // 460
	this->hotswapD_modKey = INPUT_KEY_UNKNOWN; // 461
}
std::shared_ptr<CObject> TSet_ContinuousControl::putProperty(
	std::shared_ptr<CInterpreter> interpreter, std::shared_ptr<Tool> owner,
	int settingsSig, int subSig, std::shared_ptr<CObject> object, bool isGet, bool asPercentage, bool asString)
{
	// Spacing Settings
	if (settingsSig >= 400 && settingsSig <= 404)
	{
		if (settingsSig == 400)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(defaultMode, settingsSig, subSig, "defaultMode")); }
				//return std::make_shared<CObject>(double(alternateModeKey));
			}
			else
			{
				//alternateModeKey = (int)std::get<double>(object.get()->obj);
			}
		}
		else if (settingsSig == 401)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(alternateModeKey, settingsSig, subSig, "alternateModeKey", true)); }
				return std::make_shared<CObject>(double(alternateModeKey));
			}
			else
			{
				alternateModeKey = (int)std::get<double>(object.get()->obj);
			}
		}
		else if (settingsSig == 402)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(anchorSpacing, settingsSig, subSig, "anchorSpacing")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange_cubed(anchorSpacing, TSET_CONTINUOUS_ANCHORSPACING_MIN, TSET_CONTINUOUS_ANCHORSPACING_MAX, true)));
				}
				else { return std::make_shared<CObject>(double(anchorSpacing)); }
			}
			else
			{
				if (asPercentage) {
					anchorSpacing = percentRange_cubed(
						(float)std::get<double>(object.get()->obj), TSET_CONTINUOUS_ANCHORSPACING_MIN, TSET_CONTINUOUS_ANCHORSPACING_MAX, false);
				}
				else { anchorSpacing = (float)std::get<double>(object.get()->obj); }
			}
		}
		else if (settingsSig == 403)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(relativeSpacing, settingsSig, subSig, "relativeSpacing")); }
				return std::make_shared<CObject>(relativeSpacing);
			}
			else
			{
				relativeSpacing = std::get<bool>(object.get()->obj);
			}
		}
		else if (settingsSig == 404)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(useCanvas, settingsSig, subSig, "useCanvas")); }
				return std::make_shared<CObject>(useCanvas);
			}
			else
			{
				useCanvas = std::get<bool>(object.get()->obj);
			}
		}
	}
	// Angle Settings
	else if (settingsSig >= 405 && settingsSig <= 411)
	{
		if (settingsSig == 405)
		{
			if (!isGet)
			{
				useAngle = std::get<bool>(object.get()->obj);
			}
			else
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(useAngle, settingsSig, subSig, "useAngle")); }
				return std::make_shared<CObject>(useAngle);
			}
		}
		else if (settingsSig == 406)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(angleMin, settingsSig, subSig, "angleMin")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange(angleMin, TSET_CONTINUOUS_ANGLERAND_MIN, TSET_CONTINUOUS_ANGLERAND_MAX, true)));
				}
				else { return std::make_shared<CObject>(double(angleMin)); }
			}
			else
			{
				if (asPercentage) {
					angleMin = percentRange(
						(float)std::get<double>(object.get()->obj), TSET_CONTINUOUS_ANGLERAND_MIN, TSET_CONTINUOUS_ANGLERAND_MAX, false);
				}
				else { angleMin = clampf((float)std::get<double>(object.get()->obj), TSET_CONTINUOUS_ANGLERAND_MIN, TSET_CONTINUOUS_ANGLERAND_MAX); }
			}
		}
		else if (settingsSig == 407)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(angleMax, settingsSig, subSig, "angleMax")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange(angleMax, TSET_SCATTER_ANGLE_MIN, TSET_SCATTER_ANGLE_MAX, true)));
				}
				else { return std::make_shared<CObject>(double(angleMax)); }
			}
			else
			{
				if (asPercentage) {
					angleMax = percentRange(
						(float)std::get<double>(object.get()->obj), TSET_SCATTER_ANGLE_MIN, TSET_SCATTER_ANGLE_MAX, false);
				}
				else { angleMax = clampf((float)std::get<double>(object.get()->obj), TSET_CONTINUOUS_ANGLERAND_MIN, TSET_CONTINUOUS_ANGLERAND_MAX); }
			}
		}
		else if (settingsSig == 408)
		{
			if (!isGet)
			{
				angleBias = clampf((float)std::get<double>(object.get()->obj), -1.0f, 1.0f);
			}
			else
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(angleBias, settingsSig, subSig, "angleBias")); }
				return std::make_shared<CObject>((double)angleBias);
			}
		}
		else if (settingsSig == 410)
		{
			if (!isGet)
			{
				useDirection = std::get<bool>(object.get()->obj);
			}
			else
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(useDirection, settingsSig, subSig, "useDirection")); }
				return std::make_shared<CObject>(useDirection);
			}
		}
		else if (settingsSig == 411)
		{
			if (!isGet)
			{
				relativeDistance = std::get<bool>(object.get()->obj);
			}
			else
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(relativeDistance, settingsSig, subSig, "relativeDistance")); }
				return std::make_shared<CObject>(relativeDistance);
			}

		}
	}
	// Distance Settings
	else if (settingsSig >= 412 && settingsSig <= 417)
	{
		if (settingsSig == 412)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(splineRandomXMin, settingsSig, subSig, "splineRandomXMin")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange(splineRandomXMin, TSET_CONTINUOUS_SPLINERAND_MIN, TSET_CONTINUOUS_SPLINERAND_MAX, true)));
				}
				else { return std::make_shared<CObject>(double(splineRandomXMin)); }
			}
			else
			{
				if (asPercentage) {
					splineRandomXMin = percentRange(
						(float)std::get<double>(object.get()->obj), TSET_CONTINUOUS_SPLINERAND_MIN, TSET_CONTINUOUS_SPLINERAND_MAX, false);
				}
				else { splineRandomXMin = (float)std::get<double>(object.get()->obj); }
			}
		}
		else if (settingsSig == 413)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(splineRandomXMax, settingsSig, subSig, "splineRandomXMax")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange(splineRandomXMax, TSET_CONTINUOUS_SPLINERAND_MIN, TSET_CONTINUOUS_SPLINERAND_MAX, true)));
				}
				else { return std::make_shared<CObject>(double(splineRandomXMax)); }
			}
			else
			{
				if (asPercentage) {
					splineRandomXMax = percentRange(
						(float)std::get<double>(object.get()->obj), TSET_CONTINUOUS_SPLINERAND_MIN, TSET_CONTINUOUS_SPLINERAND_MAX, false);
				}
				else { splineRandomXMax = (float)std::get<double>(object.get()->obj); }
			}
		}
		else if (settingsSig == 414)
		{
			if (!isGet)
			{
				splineRandomXBias = clampf((float)std::get<double>(object.get()->obj), -1.0f, 1.0f);
			}
			else
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(splineRandomXBias, settingsSig, subSig, "splineRandomXBias")); }
				return std::make_shared<CObject>((double)splineRandomXBias);
			}
		}
		else if (settingsSig == 415)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(splineRandomYMin, settingsSig, subSig, "splineRandomYMin")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange(splineRandomYMin, TSET_CONTINUOUS_SPLINERAND_MIN, TSET_CONTINUOUS_SPLINERAND_MAX, true)));
				}
				else { return std::make_shared<CObject>(double(splineRandomYMin)); }
			}
			else
			{
				if (asPercentage) {
					splineRandomYMin = percentRange(
						(float)std::get<double>(object.get()->obj), TSET_CONTINUOUS_SPLINERAND_MIN, TSET_CONTINUOUS_SPLINERAND_MAX, false);
				}
				else { splineRandomYMin = (float)std::get<double>(object.get()->obj); }
			}
		}
		else if (settingsSig == 416)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(splineRandomYMax, settingsSig, subSig, "splineRandomYMax")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange(splineRandomYMax, TSET_CONTINUOUS_SPLINERAND_MIN, TSET_CONTINUOUS_SPLINERAND_MAX, true)));
				}
				else { return std::make_shared<CObject>(double(splineRandomYMax)); }
			}
			else
			{
				if (asPercentage) {
					splineRandomYMax = percentRange(
						(float)std::get<double>(object.get()->obj), TSET_CONTINUOUS_SPLINERAND_MIN, TSET_CONTINUOUS_SPLINERAND_MAX, false);
				}
				else { splineRandomYMax = (float)std::get<double>(object.get()->obj); }
			}
		}
		else if (settingsSig == 417)
		{
			if (!isGet)
			{
				splineRandomYBias = clampf((float)std::get<double>(object.get()->obj), -1.0f, 1.0f);
			}
			else
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(splineRandomYBias, settingsSig, subSig, "splineRandomYBias")); }
				return std::make_shared<CObject>((double)splineRandomYBias);
			}

		}
	}
	// Constraint Length Threshold
	else if (settingsSig == 430)
	{
		if (isGet)
		{
			if (asString) { return std::make_shared<CObject>(makeSettingString(constraintLengthThreshold, settingsSig, subSig, "constraintLengthThreshold")); }
			if (asPercentage) {
				return std::make_shared<CObject>(double(
					percentRange(constraintLengthThreshold, TSET_CONTINUOUS_CONSTRAINT_LEN_THRESH_MIN, TSET_CONTINUOUS_CONSTRAINT_LEN_THRESH_MAX, true)));
			}
			else { return std::make_shared<CObject>(double(constraintLengthThreshold)); }
		}
		else
		{
			if (asPercentage) {
				constraintLengthThreshold = percentRange(
					(float)std::get<double>(object.get()->obj), TSET_CONTINUOUS_CONSTRAINT_LEN_THRESH_MIN, TSET_CONTINUOUS_CONSTRAINT_LEN_THRESH_MAX, false);
			}
			else {
				constraintLengthThreshold = clampf(
					(float)std::get<double>(object.get()->obj),
					TSET_CONTINUOUS_CONSTRAINT_LEN_THRESH_MIN,
					TSET_CONTINUOUS_CONSTRAINT_LEN_THRESH_MAX);
			}
		}
	}
	// Constraint Key and Angle Settings
	else if (settingsSig >= 431 && settingsSig <= 447)
	{
		if (settingsSig == 431)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(enableConstrainA, settingsSig, subSig, "enableConstrainA")); }
				return std::make_shared<CObject>(enableConstrainA);
			}
			else
			{
				enableConstrainA = std::get<bool>(object.get()->obj);
			}
		}
		else if (settingsSig == 432)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(constrainA_angle, settingsSig, subSig, "constrainA_angle")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange(constrainA_angle, TSET_CONTINUOUS_CONSTRAINT_ANGLE_MIN, TSET_CONTINUOUS_CONSTRAINT_ANGLE_MAX, true)));
				}
				else { return std::make_shared<CObject>(double(constrainA_angle)); }
			}
			else
			{
				if (asPercentage) {
					constrainA_angle = percentRange(
						(float)std::get<double>(object.get()->obj), TSET_CONTINUOUS_CONSTRAINT_ANGLE_MIN, TSET_CONTINUOUS_CONSTRAINT_ANGLE_MAX, false);
				}
				else {
					constrainA_angle = clampf((float)std::get<double>(object.get()->obj),
						TSET_CONTINUOUS_CONSTRAINT_ANGLE_MIN,
						TSET_CONTINUOUS_CONSTRAINT_ANGLE_MAX);
				}
			}
		}
		else if (settingsSig == 433)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(constrainA_modKey, settingsSig, subSig, "constrainA_modKey", true)); }
				return std::make_shared<CObject>(double(constrainA_modKey));
			}
			else
			{
				constrainA_modKey = (int)std::get<double>(object.get()->obj);
			}
		}
		else if (settingsSig == 435)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(enableConstrainB, settingsSig, subSig, "enableConstrainB")); }
				return std::make_shared<CObject>(enableConstrainB);
			}
			else
			{
				enableConstrainB = std::get<bool>(object.get()->obj);
			}
		}
		else if (settingsSig == 436)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(constrainB_angle, settingsSig, subSig, "constrainB_angle")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange(constrainB_angle, TSET_CONTINUOUS_CONSTRAINT_ANGLE_MIN, TSET_CONTINUOUS_CONSTRAINT_ANGLE_MAX, true)));
				}
				else { return std::make_shared<CObject>(double(constrainB_angle)); }
			}
			else
			{
				if (asPercentage) {
					constrainB_angle = percentRange(
						(float)std::get<double>(object.get()->obj), TSET_CONTINUOUS_CONSTRAINT_ANGLE_MIN, TSET_CONTINUOUS_CONSTRAINT_ANGLE_MAX, false);
				}
				else {
					constrainB_angle = clampf((float)std::get<double>(object.get()->obj),
						TSET_CONTINUOUS_CONSTRAINT_ANGLE_MIN,
						TSET_CONTINUOUS_CONSTRAINT_ANGLE_MAX);
				}
			}
		}
		else if (settingsSig == 437)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(constrainB_modKey, settingsSig, subSig, "constrainB_modKey", true)); }
				return std::make_shared<CObject>(double(constrainB_modKey));
			}
			else
			{
				constrainB_modKey = (int)std::get<double>(object.get()->obj);
			}
		}
		else if (settingsSig == 440)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(enableConstrainC, settingsSig, subSig, "enableConstrainC")); }
				return std::make_shared<CObject>(enableConstrainC);
			}
			else
			{
				enableConstrainC = std::get<bool>(object.get()->obj);
			}
		}
		else if (settingsSig == 441)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(constrainC_angle, settingsSig, subSig, "constrainC_angle")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange(constrainC_angle, TSET_CONTINUOUS_CONSTRAINT_ANGLE_MIN, TSET_CONTINUOUS_CONSTRAINT_ANGLE_MAX, true)));
				}
				else { return std::make_shared<CObject>(double(constrainC_angle)); }
			}
			else
			{
				if (asPercentage) {
					constrainC_angle = percentRange(
						(float)std::get<double>(object.get()->obj), TSET_CONTINUOUS_CONSTRAINT_ANGLE_MIN, TSET_CONTINUOUS_CONSTRAINT_ANGLE_MAX, false);
				}
				else {
					constrainC_angle = clampf((float)std::get<double>(object.get()->obj),
						TSET_CONTINUOUS_CONSTRAINT_ANGLE_MIN,
						TSET_CONTINUOUS_CONSTRAINT_ANGLE_MAX);
				}
			}
		}
		else if (settingsSig == 442)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(constrainC_modKey, settingsSig, subSig, "constrainC_modKey", true)); }
				return std::make_shared<CObject>(double(constrainC_modKey));
			}
			else
			{
				constrainC_modKey = (int)std::get<double>(object.get()->obj);
			}
		}
		else if (settingsSig == 445)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(enableConstrainD, settingsSig, subSig, "enableConstrainD")); }
				return std::make_shared<CObject>(enableConstrainD);
			}
			else
			{
				enableConstrainD = std::get<bool>(object.get()->obj);
			}
		}
		else if (settingsSig == 446)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(constrainD_angle, settingsSig, subSig, "constrainD_angle")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange(constrainD_angle, TSET_CONTINUOUS_CONSTRAINT_ANGLE_MIN, TSET_CONTINUOUS_CONSTRAINT_ANGLE_MAX, true)));
				}
				else { return std::make_shared<CObject>(double(constrainD_angle)); }
			}
			else
			{
				if (asPercentage) {
					constrainD_angle = percentRange(
						(float)std::get<double>(object.get()->obj), TSET_CONTINUOUS_CONSTRAINT_ANGLE_MIN, TSET_CONTINUOUS_CONSTRAINT_ANGLE_MAX, false);
				}
				else {
					constrainD_angle = clampf((float)std::get<double>(object.get()->obj),
						TSET_CONTINUOUS_CONSTRAINT_ANGLE_MIN,
						TSET_CONTINUOUS_CONSTRAINT_ANGLE_MAX);
				}
			}
		}
		else if (settingsSig == 447)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(constrainD_modKey, settingsSig, subSig, "constrainD_modKey", true)); }
				return std::make_shared<CObject>(double(constrainD_modKey));
			}
			else
			{
				constrainD_modKey = (int)std::get<double>(object.get()->obj);
			}
		}
	}
	// Hot Swap Keys
	else if (settingsSig >= 450 && settingsSig <= 461)
	{
		if (settingsSig == 450)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(enableHotswapA, settingsSig, subSig, "enableHotswapA")); }
				return std::make_shared<CObject>(enableHotswapA);
			}
			else
			{
				enableHotswapA = std::get<bool>(object.get()->obj);
			}
		}
		else if (settingsSig == 451)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(hotswapA_toolID, settingsSig, subSig, "hotswapA_toolID", false)); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange(hotswapA_toolID, TSET_CONTINUOUS_CONSTRAINT_ANGLE_MIN, TSET_CONTINUOUS_CONSTRAINT_ANGLE_MAX, true)));
				}
				else { return std::make_shared<CObject>(double(hotswapA_toolID)); }
			}
			else
			{
				if (asPercentage) {
					hotswapA_toolID = percentRange(
						(int)std::get<double>(object.get()->obj), TSET_CONTINUOUS_CONSTRAINT_ANGLE_MIN, TSET_CONTINUOUS_CONSTRAINT_ANGLE_MAX, false);
				}
				else { hotswapA_toolID = (int)std::get<double>(object.get()->obj); }
			}
		}
		else if (settingsSig == 452)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(hotswapA_modKey, settingsSig, subSig, "hotswapA_modKey", true)); }
				return std::make_shared<CObject>(double(hotswapA_modKey));
			}
			else
			{
				hotswapA_modKey = (int)std::get<double>(object.get()->obj);
			}
		}
		else if (settingsSig == 453)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(enableHotswapB, settingsSig, subSig, "enableHotswapB")); }
				return std::make_shared<CObject>(enableHotswapB);
			}
			else
			{
				enableHotswapB = std::get<bool>(object.get()->obj);
			}
		}
		else if (settingsSig == 454)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(hotswapB_toolID, settingsSig, subSig, "hotswapB_toolID", false)); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange(hotswapB_toolID, TSET_CONTINUOUS_CONSTRAINT_ANGLE_MIN, TSET_CONTINUOUS_CONSTRAINT_ANGLE_MAX, true)));
				}
				else { return std::make_shared<CObject>(double(hotswapB_toolID)); }
			}
			else
			{
				if (asPercentage) {
					hotswapB_toolID = percentRange(
						(int)std::get<double>(object.get()->obj), TSET_CONTINUOUS_CONSTRAINT_ANGLE_MIN, TSET_CONTINUOUS_CONSTRAINT_ANGLE_MAX, false);
				}
				else { hotswapB_toolID = (int)std::get<double>(object.get()->obj); }
			}
		}
		else if (settingsSig == 455)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(hotswapB_modKey, settingsSig, subSig, "hotswapB_modKey", true)); }
				return std::make_shared<CObject>(double(hotswapB_modKey));
			}
			else
			{
				hotswapB_modKey = (int)std::get<double>(object.get()->obj);
			}
		}
		else if (settingsSig == 456)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(enableHotswapC, settingsSig, subSig, "enableHotswapC")); }
				return std::make_shared<CObject>(enableHotswapC);
			}
			else
			{
				enableHotswapC = std::get<bool>(object.get()->obj);
			}
		}
		else if (settingsSig == 457)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(hotswapC_toolID, settingsSig, subSig, "hotswapC_toolID", false)); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange(hotswapC_toolID, TSET_CONTINUOUS_CONSTRAINT_ANGLE_MIN, TSET_CONTINUOUS_CONSTRAINT_ANGLE_MAX, true)));
				}
				else { return std::make_shared<CObject>(double(hotswapC_toolID)); }
			}
			else
			{
				if (asPercentage) {
					hotswapC_toolID = percentRange(
						(int)std::get<double>(object.get()->obj), TSET_CONTINUOUS_CONSTRAINT_ANGLE_MIN, TSET_CONTINUOUS_CONSTRAINT_ANGLE_MAX, false);
				}
				else { hotswapC_toolID = (int)std::get<double>(object.get()->obj); }
			}
		}
		else if (settingsSig == 458)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(hotswapC_modKey, settingsSig, subSig, "hotswapC_modKey", true)); }
				return std::make_shared<CObject>(double(hotswapC_modKey));
			}
			else
			{
				hotswapC_modKey = (int)std::get<double>(object.get()->obj);
			}
		}
		else if (settingsSig == 459)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(enableHotswapD, settingsSig, subSig, "enableHotswapD")); }
				return std::make_shared<CObject>(enableHotswapD);
			}
			else
			{
				enableHotswapD = std::get<bool>(object.get()->obj);
			}
		}
		else if (settingsSig == 460)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(hotswapD_toolID, settingsSig, subSig, "hotswapD_toolID", false)); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange(hotswapD_toolID, TSET_CONTINUOUS_CONSTRAINT_ANGLE_MIN, TSET_CONTINUOUS_CONSTRAINT_ANGLE_MAX, true)));
				}
				else { return std::make_shared<CObject>(double(hotswapD_toolID)); }
			}
			else
			{
				if (asPercentage) {
					hotswapD_toolID = percentRange(
						(int)std::get<double>(object.get()->obj), TSET_CONTINUOUS_CONSTRAINT_ANGLE_MIN, TSET_CONTINUOUS_CONSTRAINT_ANGLE_MAX, false);
				}
				else { hotswapD_toolID = (int)std::get<double>(object.get()->obj); }
			}
		}
		else if (settingsSig == 461)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(hotswapD_modKey, settingsSig, subSig, "hotswapD_modKey", true)); }
				return std::make_shared<CObject>(double(hotswapD_modKey));
			}
			else
			{
				hotswapD_modKey = (int)std::get<double>(object.get()->obj);
			}
		}
	}

	if (asString) { return std::make_shared<CObject>(makeSettingString(isEnabled, settingsSig, subSig, "NULL")); }
	return std::make_shared<CObject>(nullptr);
}
void TSet_ContinuousControl::clearConstraint()
{
	activated = false;
	line = glm::vec4(0);
	origin = dir = perpendicular = glm::vec3(0);
	activeAngle = 0.0f;
	activeKey = -1;
}
void TSet_ContinuousControl::updateTrueSpacing(std::shared_ptr<Tool> owner, int canvasWidth, int canvasHeight)
{
	if (relativeSpacing)
	{
		if (useCanvas)
		{
			trueSpacing = (anchorSpacing / 1000.0f) * (sqrtf(powf(canvasWidth, 2) + powf(canvasHeight, 2)));
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