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
#include "../../include/toolSettings/gen/TSet_Scatter.h"

////////////////////////////////////////////////////////////////
// Scatter Dynamics
////////////////////////////////////////////////////////////////

TSet_Scatter::TSet_Scatter()
{
	this->type = TSetType::scatter;
	this->isEnabled = false;
	this->maxDistance = 500.0f;
	this->maxCount = 30.0f;

	this->useAngle = false; // 2600
	//TSetController angleControl; // 2610
	//TSetNoise angleNoise; // 2611
	this->angleMin = 0.0f; // 2612
	this->angleMax = 0.0f; // 2613
	this->angleBias = 0.0f; // 2614
	this->useDirection = false; // 2616
	this->relativeDistance = true; // 2617
	//TSetController distanceControl; // 2620
	//TSetNoise distanceNoise; // 2621
	this->distanceXMin = 0.0f; // 2622
	this->distanceXMax = 0.0f; // 2623
	this->distanceXBias = 0.0f; // 2624
	this->distanceYMin = 0.0f; // 2625
	this->distanceYMax = 0.0f; // 2626
	this->distanceYBias = 0.0f; // 2627
	//TSetController countControl; // 2630
	//TSetNoise countNoise; // 2631
	this->countMin = TSET_SCATTER_COUNT_MIN; // 2632
	this->countMax = TSET_SCATTER_COUNT_MIN; // 2633
	this->countBias = 0.0f; // 2634
}
std::shared_ptr<CObject> TSet_Scatter::putProperty(
	std::shared_ptr<CInterpreter> interpreter, std::shared_ptr<Tool> owner,
	int settingsSig, int subSig, std::shared_ptr<CObject> object, bool isGet, bool asPercentage, bool asString)
{
	// Angle Settings
	if (settingsSig >= 2600 && settingsSig <= 2619)
	{
		switch (settingsSig)
		{
		case 2600:
			if (!isGet)
			{
				useAngle = std::get<bool>(object.get()->obj);
			}
			else
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(useAngle, settingsSig, subSig, "useAngle")); }
				return std::make_shared<CObject>(useAngle);
			}
			break;
		case 2612:
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(angleMin, settingsSig, subSig, "angleMin")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange(angleMin, TSET_SCATTER_ANGLE_MIN, TSET_SCATTER_ANGLE_MAX, true)));
				}
				else { return std::make_shared<CObject>(double(angleMin)); }
			}
			else
			{
				if (asPercentage) {
					angleMin = percentRange(
						(float)std::get<double>(object.get()->obj), TSET_SCATTER_ANGLE_MIN, TSET_SCATTER_ANGLE_MAX, false);
				}
				else { angleMin = (float)std::get<double>(object.get()->obj); }
			} break;
		case 2613:
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
				else { angleMax = (float)std::get<double>(object.get()->obj); }
			} break;
		case 2614:
			if (!isGet)
			{
				angleBias = clampf((float)std::get<double>(object.get()->obj), -1.0f, 1.0f);
			}
			else
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(angleBias, settingsSig, subSig, "angleBias")); }
				return std::make_shared<CObject>((double)angleBias);
			} break;
		case 2616:
			if (!isGet)
			{
				useDirection = std::get<bool>(object.get()->obj);
			}
			else
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(useDirection, settingsSig, subSig, "useDirection")); }
				return std::make_shared<CObject>(useDirection);
			} break;
		case 2617:
			if (!isGet)
			{
				relativeDistance = std::get<bool>(object.get()->obj);
			}
			else
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(relativeDistance, settingsSig, subSig, "relativeDistance")); }
				else { return std::make_shared<CObject>(relativeDistance); break; }
			}

		}
	}
	// Distance Settings
	else if (settingsSig >= 2620 && settingsSig <= 2629)
	{
		if (settingsSig == 2622)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(distanceXMin, settingsSig, subSig, "distanceXMin")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange_squaredCenter(distanceXMin, TSET_SCATTER_DIST_MIN, TSET_SCATTER_DIST_MAX, true)));
				}
				else { return std::make_shared<CObject>(double(distanceXMin)); }
			}
			else
			{
				if (asPercentage) {
					distanceXMin = percentRange_squaredCenter(
						(float)std::get<double>(object.get()->obj), TSET_SCATTER_DIST_MIN, TSET_SCATTER_DIST_MAX, false);
				}
				else { distanceXMin = (float)std::get<double>(object.get()->obj); }
			}
		}
		else if (settingsSig == 2623)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(distanceXMax, settingsSig, subSig, "distanceXMax")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange_squaredCenter(distanceXMax, TSET_SCATTER_DIST_MIN, TSET_SCATTER_DIST_MAX, true)));
				}
				else { return std::make_shared<CObject>(double(distanceXMax)); }
			}
			else
			{
				if (asPercentage) {
					distanceXMax = percentRange_squaredCenter(
						(float)std::get<double>(object.get()->obj), TSET_SCATTER_DIST_MIN, TSET_SCATTER_DIST_MAX, false);
				}
				else { distanceXMax = (float)std::get<double>(object.get()->obj); }
			}
		}
		else if (settingsSig == 2624)
		{
			if (!isGet)
			{
				distanceXBias = clampf((float)std::get<double>(object.get()->obj), -1.0f, 1.0f);
			}
			else
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(distanceXBias, settingsSig, subSig, "distanceXBias")); }
				return std::make_shared<CObject>((double)distanceXBias);
			}
		}
		else if (settingsSig == 2625)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(distanceYMin, settingsSig, subSig, "distanceYMin")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange_squaredCenter(distanceYMin, TSET_SCATTER_DIST_MIN, TSET_SCATTER_DIST_MAX, true)));
				}
				else { return std::make_shared<CObject>(double(distanceYMin)); }
			}
			else
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(distanceYMin, settingsSig, subSig, "distanceYMin")); }
				if (asPercentage) {
					distanceYMin = percentRange_squaredCenter(
						(float)std::get<double>(object.get()->obj), TSET_SCATTER_DIST_MIN, TSET_SCATTER_DIST_MAX, false);
				}
				else { distanceYMin = (float)std::get<double>(object.get()->obj); }
			}
		}
		else if (settingsSig == 2626)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(distanceYMax, settingsSig, subSig, "distanceYMax")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange_squaredCenter(distanceYMax, TSET_SCATTER_DIST_MIN, TSET_SCATTER_DIST_MAX, true)));
				}
				else { return std::make_shared<CObject>(double(distanceYMax)); }
			}
			else
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(distanceYMax, settingsSig, subSig, "distanceYMax")); }
				if (asPercentage) {
					distanceYMax = percentRange_squaredCenter(
						(float)std::get<double>(object.get()->obj), TSET_SCATTER_DIST_MIN, TSET_SCATTER_DIST_MAX, false);
				}
				else { distanceYMax = (float)std::get<double>(object.get()->obj); }
			}
		}
		else if (settingsSig == 2627)
		{
			if (!isGet)
			{
				distanceYBias = clampf((float)std::get<double>(object.get()->obj), -1.0f, 1.0f);
			}
			else
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(distanceYBias, settingsSig, subSig, "distanceYBias")); }
				return std::make_shared<CObject>((double)distanceYBias);
			}
		}
	}
	// Count Dynamics
	else if (settingsSig >= 2630 && settingsSig <= 2649)
	{
		if (settingsSig == 2632)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(countMin, settingsSig, subSig, "countMin")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange(countMin, TSET_SCATTER_COUNT_MIN, TSET_SCATTER_COUNT_MAX, true)));
				}
				else { return std::make_shared<CObject>(double(countMin)); }
			}
			else
			{
				if (asPercentage) {
					countMin = (int)std::round(
						percentRange(std::get<double>(object.get()->obj), TSET_SCATTER_COUNT_MIN, TSET_SCATTER_COUNT_MAX, false));
				}
				else {
					countMin = clampi(
						(int)std::round(std::get<double>(object.get()->obj)), TSET_SCATTER_COUNT_MIN, TSET_SCATTER_COUNT_MAX);
				}
			}
		}
		else if (settingsSig == 2633)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(countMax, settingsSig, subSig, "countMax")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange(countMax, TSET_SCATTER_COUNT_MIN, TSET_SCATTER_COUNT_MAX, true)));
				}
				else { return std::make_shared<CObject>(double(countMax)); }
			}
			else
			{
				if (asPercentage) {
					countMax = (int)std::round(
						percentRange(std::get<double>(object.get()->obj), TSET_SCATTER_COUNT_MIN, TSET_SCATTER_COUNT_MAX, false));
				}
				else {
					countMax = clampi(
						(int)std::round(std::get<double>(object.get()->obj)), TSET_SCATTER_COUNT_MIN, TSET_SCATTER_COUNT_MAX);
				}
			}
		}
		else if (settingsSig == 2633)
		{
			if (!isGet)
			{
				countBias = clampf((float)std::get<double>(object.get()->obj), -1.0f, 1.0f);
			}
			else
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(countBias, settingsSig, subSig, "countBias")); }
				return std::make_shared<CObject>((double)countBias);
			}
		}
	}

	if (asString) { return std::make_shared<CObject>(makeSettingString(isEnabled, settingsSig, subSig, "NULL")); }
	return std::make_shared<CObject>(nullptr);
}
glm::vec3 TSet_Scatter::modulatePosition(glm::vec3 pos, glm::vec3 dir, float tipSize)
{
	std::random_device device;
	std::default_random_engine eng(device());
	std::uniform_real_distribution<float> distr(0, 1);

	glm::vec3 outPos = pos;

	float offsetX = 0.0f; float offsetY = 0.0f;
	if (relativeDistance)
	{
		offsetX = lerpf((distanceXMin / 100) * (float)tipSize, (distanceXMax / 100) * (float)tipSize, (double)distr(eng));
		offsetY = lerpf((distanceYMin / 100) * (float)tipSize, (distanceYMax / 100) * (float)tipSize, (double)distr(eng));
	}
	else
	{
		offsetX = lerpf(distanceXMin, distanceXMax, (double)distr(eng));
		offsetY = lerpf(distanceYMin, distanceYMax, (double)distr(eng));
	}

	if (!useAngle)
	{
		outPos.x = outPos.x + offsetX;
		outPos.y = outPos.y + offsetY;
	}
	else
	{
		float outAngle = -clampAngle_180(lerpf(angleMin, angleMax, (double)distr(eng)));
		glm::vec3 inDir = (useDirection) ? dir : glm::vec3(cos(MATH_RAD_90), sin(MATH_RAD_90), 0.0f);
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

	return outPos;
}
int TSet_Scatter::modulateCount()
{
	std::random_device device;
	std::default_random_engine eng(device());
	std::uniform_real_distribution<float> distr(0, 1);

	int count = 1;

	count = 1 + (int)lerpf(countMin, countMax, (double)distr(eng));

	return count;
}
void TSet_Scatter::updatePadding()
{
	padding = 0;
	if (abs(distanceXMin) > abs(distanceXMax)) { padding = abs(distanceXMin); }
	else { padding = abs(distanceXMax); }
	if (abs(distanceYMin) > padding) { padding = abs(distanceYMin) * 1.3f; }
	if (abs(distanceYMax) > padding) { padding = abs(distanceYMax) * 1.3f; }
}
int TSet_Scatter::fetchPadding(int tipSize)
{
	if (relativeDistance)
	{
		return (int)((padding / 70.0f) * tipSize);
	}
	else
	{
		return (int)padding;
	}
}