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

#include "../../../include/tool/toolSettings/gen/TSet_Scatter.h"

////////////////////////////////////////////////////////////////
// Scatter Dynamics
////////////////////////////////////////////////////////////////

TSet_Scatter::TSet_Scatter()
{
	this->type = TSetType::scatter;
	this->isEnabled = false;
	this->distanceXNode.isEnabled = false;
	this->distanceYNode.isEnabled = false;
	this->countNode.isEnabled = false;
	this->countNode.controlMax = 0.0f;
}
std::shared_ptr<CObject> TSet_Scatter::putProperty(
	std::shared_ptr<CInterpreter> interpreter, std::shared_ptr<Tool> owner,
	int settingsSig, int subSig, std::shared_ptr<CObject> object, bool isGet, bool asPercentage, bool asString)
{
	// Angle Settings
	if (settingsSig >= 2600 && settingsSig <= 2619)
	{
		if (settingsSig == 2600)
		{
			return angleNode.putProperty(interpreter, owner, settingsSig, subSig, object, isGet, asPercentage, asString);
		}
		else if (settingsSig == 2601)
		{
			return angleController.putProperty(interpreter, owner, settingsSig, subSig, object, isGet, asPercentage, asString);
		}
		else if (settingsSig == 2602)
		{
			return angleNoise.putProperty(interpreter, owner, settingsSig, subSig, object, isGet, asPercentage, asString);
		}
	}
	// Distance Settings
	if (settingsSig >= 2610 && settingsSig < 2630)
	{
		if (settingsSig == 2610)
		{
			return distanceXNode.putProperty(interpreter, owner, settingsSig, subSig, object, isGet, asPercentage, asString);
		}
		else if (settingsSig == 2611)
		{
			return distanceXController.putProperty(interpreter, owner, settingsSig, subSig, object, isGet, asPercentage, asString);
		}
		else if (settingsSig == 2612)
		{
			return distanceXNoise.putProperty(interpreter, owner, settingsSig, subSig, object, isGet, asPercentage, asString);
		}
		else if (settingsSig == 2620)
		{
			return distanceYNode.putProperty(interpreter, owner, settingsSig, subSig, object, isGet, asPercentage, asString);
		}
		else if (settingsSig == 2621)
		{
			return distanceYController.putProperty(interpreter, owner, settingsSig, subSig, object, isGet, asPercentage, asString);
		}
		else if (settingsSig == 2622)
		{
			return distanceYNoise.putProperty(interpreter, owner, settingsSig, subSig, object, isGet, asPercentage, asString);
		}
	}
	// Count Dynamics
	else if (settingsSig >= 2630 && settingsSig < 2640)
	{
		if (settingsSig == 2630)
		{
			return countNode.putProperty(interpreter, owner, settingsSig, subSig, object, isGet, asPercentage, asString);
		}
		else if (settingsSig == 2631)
		{
			return countController.putProperty(interpreter, owner, settingsSig, subSig, object, isGet, asPercentage, asString);
		}
		else if (settingsSig == 2632)
		{
			return countNoise.putProperty(interpreter, owner, settingsSig, subSig, object, isGet, asPercentage, asString);
		}
	}
	// Misc Settings
	else if (settingsSig >= 2660 && settingsSig < 2670)
	{
		if (settingsSig == 2660) {
			if (!isGet) { useAngle = std::get<bool>(object.get()->obj); }
			else {
				if (asString) { return std::make_shared<CObject>(makeSettingString(useAngle, settingsSig, subSig, "useAngle")); }
				return std::make_shared<CObject>(useAngle);
			}
		}
		else if (settingsSig == 2661) {
			if (!isGet) { useDirection = std::get<bool>(object.get()->obj); }
			else {
				if (asString) { return std::make_shared<CObject>(makeSettingString(useDirection, settingsSig, subSig, "useDirection")); }
				return std::make_shared<CObject>(useDirection);
			}
		}
		else if (settingsSig == 2662) {
			if (!isGet) { relativeDistance = std::get<bool>(object.get()->obj); }
			else {
				if (asString) { return std::make_shared<CObject>(makeSettingString(relativeDistance, settingsSig, subSig, "relativeDistance")); }
				return std::make_shared<CObject>(relativeDistance);
			}
		}
		else if (settingsSig == 2663) {
			if (!isGet) {
				if (asPercentage) {
					offsetAngle = percentRange_squaredCenter(
						(float)std::get<double>(object.get()->obj), -180.0f, 180.0f, false);
				}
				else { offsetAngle = clampf((float)std::get<double>(object.get()->obj), -180.0f, 180.0f); }
			}
			else {
				if (asString) { return std::make_shared<CObject>(makeSettingString(offsetAngle, settingsSig, subSig, "offsetAngle")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange_squaredCenter(offsetAngle, -180.0f, 180.0f, true)));
				}
				else { return std::make_shared<CObject>(double(offsetAngle)); }
			}
		}
	}

	if (asString) { return std::make_shared<CObject>(makeSettingString(isEnabled, settingsSig, subSig, "NULL")); }
	return std::make_shared<CObject>(nullptr);
}

TSetControl_Node* TSet_Scatter::getControlNode(int settingSig, int subSig)
{
	if (settingSig == 2600)
	{
		return &angleNode;
	}
	else if (settingSig == 2610)
	{
		return &distanceXNode;
	}
	else if (settingSig == 2620)
	{
		return &distanceYNode;
	}
	else if (settingSig == 2630)
	{
		return &countNode;
	}
	return nullptr;
}
TSetController* TSet_Scatter::getController(int settingSig, int subSig)
{
	if (settingSig == 2601)
	{
		return &angleController;
	}
	else if (settingSig == 2611)
	{
		return &distanceXController;
	}
	else if (settingSig == 2621)
	{
		return &distanceYController;
	}
	else if (settingSig == 2631)
	{
		return &countController;
	}
	return nullptr;
}
TSetGraph* TSet_Scatter::getGraph(int settingSig, int subSig)
{
	if (settingSig == 2601)
	{
		if (subSig == -1) { return &angleController.pressureGraph; }
		else if (subSig == -2) { return &angleController.directionGraph; }
		else if (subSig == -3) { return &angleController.tiltGraph; }
		else if (subSig == -4) { return &angleController.velocityGraph; }
		else if (subSig == -5) { return &angleController.rotationGraph; }
	}
	else if (settingSig == 2611)
	{
		if (subSig == -1) { return &distanceXController.pressureGraph; }
		else if (subSig == -2) { return &distanceXController.directionGraph; }
		else if (subSig == -3) { return &distanceXController.tiltGraph; }
		else if (subSig == -4) { return &distanceXController.velocityGraph; }
		else if (subSig == -5) { return &distanceXController.rotationGraph; }
	}
	else if (settingSig == 2621)
	{
		if (subSig == -1) { return &distanceYController.pressureGraph; }
		else if (subSig == -2) { return &distanceYController.directionGraph; }
		else if (subSig == -3) { return &distanceYController.tiltGraph; }
		else if (subSig == -4) { return &distanceYController.velocityGraph; }
		else if (subSig == -5) { return &distanceYController.rotationGraph; }
	}
	else if (settingSig == 2631)
	{
		if (subSig == -1) { return &countController.pressureGraph; }
		else if (subSig == -2) { return &countController.directionGraph; }
		else if (subSig == -3) { return &countController.tiltGraph; }
		else if (subSig == -4) { return &countController.velocityGraph; }
		else if (subSig == -5) { return &countController.rotationGraph; }
	}
	else if (settingSig >= 2640 && settingSig < 2660)
	{
		if (settingSig == 2640) { return &opacityByDist; }
		else if (settingSig == 2641) { return &flowByDist; }
		else if (settingSig == 2642) { return &scaleByDist; }
		else if (settingSig == 2643) { return &hueByDist; }
		else if (settingSig == 2644) { return &satByDist; }
		else if (settingSig == 2645) { return &valByDist; }
		else if (settingSig == 2646) { return &rotByDist; }
	}
	return nullptr;
}
TSetNoise* TSet_Scatter::getNoise(int settingSig, int subSig)
{
	if (settingSig == 2602)
	{
		return &angleNoise;
	}
	else if (settingSig == 2612)
	{
		return &distanceXNoise;
	}
	else if (settingSig == 2622)
	{
		return &distanceYNoise;
	}
	else if (settingSig == 2632)
	{
		return &countNoise;
	}
	return nullptr;
}

void TSet_Scatter::initialize(float* trueSpacing, int* entityCount, int* tipSize, glm::ivec2* canvasDimensions)
{
	if (!isEnabled) { return; }
	imageSize = float(*tipSize);
	if (angleNoise.isEnabled) { angleNoise.initialize(trueSpacing, entityCount, canvasDimensions); }
	//
	if (distanceXNoise.isEnabled) { distanceXNoise.initialize(trueSpacing, entityCount, canvasDimensions); }
	if (distanceYNoise.isEnabled) { distanceYNoise.initialize(trueSpacing, entityCount, canvasDimensions); }
	//
	if (countNoise.isEnabled) { countNoise.initialize(trueSpacing, entityCount, canvasDimensions); }
}

glm::vec3 TSet_Scatter::modulatePosition(
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
		if (relativeDistance && (xControlEnabled || xNoiseEnabled))
		{
			offsetX *= (tip / 100.0f);
		}
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
		if (relativeDistance && (yControlEnabled || yNoiseEnabled))
		{
			offsetY *= (tip / 100.0f);
		}
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
int TSet_Scatter::modulateCount(int& outCount,
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
void TSet_Scatter::updatePadding()
{
	padding = 0.0f;
	if (distanceXNode.reportEnabled() || distanceXNoise.reportEnabled())
	{
		if (abs(distanceXNode.controlMin * distRange) > abs(distanceXNode.controlMax * distRange)) {
			padding = abs(distanceXNode.controlMin * distRange) + (imageSize / 2.0f);
		}
		else { padding = abs(distanceXNode.controlMax * distRange); }
	}
	if (distanceYNode.reportEnabled() || distanceYNoise.reportEnabled())
	{
		if (abs(distanceYNode.controlMin * distRange) > padding) {
			padding = (abs(distanceYNode.controlMin * distRange) + (imageSize / 2.0f)) * 1.3f;
		}
		if (abs(distanceXNode.controlMax * distRange) > padding) {
			padding = (abs(distanceYNode.controlMax * distRange) + (imageSize / 2.0f)) * 1.3f;
		}
	}
	if (relativeDistance && padding != 0.0f) {
		padding *= (imageSize / 100.0f);
		padding += imageSize / 1.5f;
	}
}
int TSet_Scatter::fetchPadding(int tipSize)
{
	return (int)padding;
}