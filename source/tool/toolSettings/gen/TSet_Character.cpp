#include "../../../include/tool/ToolSettings.h"
#include "../../../include/input/keys.h"
#include "../../../include/math/Color.h"
#include "../../../include/math/noise.h"

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

#include "../../../include/tool/toolSettings/gen/TSet_Character.h"

////////////////////////////////////////////////////////////////
// Character Settings
////////////////////////////////////////////////////////////////

TSet_Character::TSet_Character()
{
	this->type = TSetType::character;
	this->isEnabled = false;

	this->flipX_random = false; // 2320
	this->flipY_random = false; // 2321

	this->sizeX_node.isEnabled = false;
	this->sizeX_node.controlMin = 0.0f;
	this->sizeX_node.controlMax = 0.0f;
	this->sizeY_node.isEnabled = false;
	this->sizeY_node.controlMin = 0.0f;
	this->sizeY_node.controlMax = 0.0f;
	//
	this->rotation_node.isEnabled = false;
	this->rotation_node.controlMin = -1.0f;
	this->rotation_node.controlMax = 1.0f;
	this->rotation_node.usePressure = false;
	this->rotation_node.useDirection = true;
	this->rotation_node.loopDirValue = false;
	//
	this->skewX_node.isEnabled = false;
	this->skewX_node.controlMin = 0.0f;
	this->skewX_node.controlMax = 0.0f;
	this->skewY_node.isEnabled = false;
	this->skewY_node.controlMin = 0.0f;
	this->skewY_node.controlMax = 0.0f;
	//
	this->shuffleA_node.isEnabled = false;
	this->shuffleA_node.controlMin = 0.0f;
	this->shuffleA_node.controlMax = 0.0f;
	this->shuffleB_node.isEnabled = false;
	this->shuffleB_node.controlMin = 0.0f;
	this->shuffleB_node.controlMax = 0.0f;
	//
	this->spread_node.isEnabled = false;
	this->spread_node.controlMin = 0.0f;
	this->spread_node.controlMax = 0.0f;
	//
	this->applyRotationLast = true;
}

std::shared_ptr<CObject> TSet_Character::putProperty(
	std::shared_ptr<CInterpreter> interpreter, std::shared_ptr<Tool> owner,
	int settingsSig, int subSig, std::shared_ptr<CObject> object, bool isGet, bool asPercentage, bool asString)
{
	if (settingsSig >= 2300 && settingsSig < 2340)
	{
		if (settingsSig == 2300)
		{
			return scaleNode.putProperty(interpreter, owner, settingsSig, subSig, object, isGet, asPercentage, asString);
		}
		else if (settingsSig == 2301)
		{
			return scaleController.putProperty(interpreter, owner, settingsSig, subSig, object, isGet, asPercentage, asString);
		}
		else if (settingsSig == 2302)
		{
			return scaleNoise.putProperty(interpreter, owner, settingsSig, subSig, object, isGet, asPercentage, asString);
		}
		else if (settingsSig == 2303)
		{
			return sizeX_node.putProperty(interpreter, owner, settingsSig, subSig, object, isGet, asPercentage, asString);
		}
		else if (settingsSig == 2304)
		{
			return sizeX_controller.putProperty(interpreter, owner, settingsSig, subSig, object, isGet, asPercentage, asString);
		}
		else if (settingsSig == 2305)
		{
			return sizeX_noise.putProperty(interpreter, owner, settingsSig, subSig, object, isGet, asPercentage, asString);
		}
		else if (settingsSig == 2306)
		{
			return sizeY_node.putProperty(interpreter, owner, settingsSig, subSig, object, isGet, asPercentage, asString);
		}
		else if (settingsSig == 2307)
		{
			return sizeY_controller.putProperty(interpreter, owner, settingsSig, subSig, object, isGet, asPercentage, asString);
		}
		else if (settingsSig == 2308)
		{
			return sizeY_noise.putProperty(interpreter, owner, settingsSig, subSig, object, isGet, asPercentage, asString);
		}
		else if (settingsSig == 2320)
		{
			return rotation_node.putProperty(interpreter, owner, settingsSig, subSig, object, isGet, asPercentage, asString);
		}
		else if (settingsSig == 2321)
		{
			return rotation_controller.putProperty(interpreter, owner, settingsSig, subSig, object, isGet, asPercentage, asString);
		}
		else if (settingsSig == 2322)
		{
			return rotation_noise.putProperty(interpreter, owner, settingsSig, subSig, object, isGet, asPercentage, asString);
		}
		else if (settingsSig == 2323) {
			if (!isGet) { applyRotationLast = std::get<bool>(object.get()->obj); }
			else {
				if (asString) { return std::make_shared<CObject>(makeSettingString(applyRotationLast, settingsSig, subSig, "applyRotationLast")); }
				return std::make_shared<CObject>(applyRotationLast);
			}
		}
		else if (settingsSig == 2330)
		{
			return skewX_node.putProperty(interpreter, owner, settingsSig, subSig, object, isGet, asPercentage, asString);
		}
		else if (settingsSig == 2331)
		{
			return skewX_controller.putProperty(interpreter, owner, settingsSig, subSig, object, isGet, asPercentage, asString);
		}
		else if (settingsSig == 2332)
		{
			return skewX_noise.putProperty(interpreter, owner, settingsSig, subSig, object, isGet, asPercentage, asString);
		}
		else if (settingsSig == 2333)
		{
			return skewY_node.putProperty(interpreter, owner, settingsSig, subSig, object, isGet, asPercentage, asString);
		}
		else if (settingsSig == 2334)
		{
			return skewY_controller.putProperty(interpreter, owner, settingsSig, subSig, object, isGet, asPercentage, asString);
		}
		else if (settingsSig == 2335)
		{
			return skewY_noise.putProperty(interpreter, owner, settingsSig, subSig, object, isGet, asPercentage, asString);
		}
	}
	else if (settingsSig >= 2340 && settingsSig < 2350)
	{
		if (settingsSig == 2340)
		{
			return shuffleA_node.putProperty(interpreter, owner, settingsSig, subSig, object, isGet, asPercentage, asString);
		}
		else if (settingsSig == 2341)
		{
			return shuffleA_controller.putProperty(interpreter, owner, settingsSig, subSig, object, isGet, asPercentage, asString);
		}
		else if (settingsSig == 2342)
		{
			return shuffleA_noise.putProperty(interpreter, owner, settingsSig, subSig, object, isGet, asPercentage, asString);
		}
		else if (settingsSig == 2343) {
			if (!isGet) { shuffleA_vert0 = std::get<bool>(object.get()->obj); }
			else {
				if (asString) { return std::make_shared<CObject>(makeSettingString(shuffleA_vert0, settingsSig, subSig, "shuffleA_vert0")); }
				return std::make_shared<CObject>(shuffleA_vert0);
			}
		}
		else if (settingsSig == 2344) {
			if (!isGet) { shuffleA_vert1 = std::get<bool>(object.get()->obj); }
			else {
				if (asString) { return std::make_shared<CObject>(makeSettingString(shuffleA_vert1, settingsSig, subSig, "shuffleA_vert1")); }
				return std::make_shared<CObject>(shuffleA_vert1);
			}
		}
		else if (settingsSig == 2345) {
			if (!isGet) { shuffleA_vert2 = std::get<bool>(object.get()->obj); }
			else {
				if (asString) { return std::make_shared<CObject>(makeSettingString(shuffleA_vert2, settingsSig, subSig, "shuffleA_vert2")); }
				return std::make_shared<CObject>(shuffleA_vert2);
			}
		}
		else if (settingsSig == 2346) {
			if (!isGet) { shuffleA_vert3 = std::get<bool>(object.get()->obj); }
			else {
				if (asString) { return std::make_shared<CObject>(makeSettingString(shuffleA_vert3, settingsSig, subSig, "shuffleA_vert3")); }
				return std::make_shared<CObject>(shuffleA_vert3);
			}
		}
	}
	else if (settingsSig >= 2350 && settingsSig < 2360)
	{
		if (settingsSig == 2350)
		{
			return shuffleB_node.putProperty(interpreter, owner, settingsSig, subSig, object, isGet, asPercentage, asString);
		}
		else if (settingsSig == 2351)
		{
			return shuffleB_controller.putProperty(interpreter, owner, settingsSig, subSig, object, isGet, asPercentage, asString);
		}
		else if (settingsSig == 2352)
		{
			return shuffleB_noise.putProperty(interpreter, owner, settingsSig, subSig, object, isGet, asPercentage, asString);
		}
		else if (settingsSig == 2353) {
			if (!isGet) { shuffleB_vert0 = std::get<bool>(object.get()->obj); }
			else {
				if (asString) { return std::make_shared<CObject>(makeSettingString(shuffleB_vert0, settingsSig, subSig, "shuffleB_vert0")); }
				return std::make_shared<CObject>(shuffleB_vert0);
			}
		}
		else if (settingsSig == 2354) {
			if (!isGet) { shuffleB_vert1 = std::get<bool>(object.get()->obj); }
			else {
				if (asString) { return std::make_shared<CObject>(makeSettingString(shuffleB_vert1, settingsSig, subSig, "shuffleB_vert1")); }
				return std::make_shared<CObject>(shuffleB_vert1);
			}
		}
		else if (settingsSig == 2355) {
			if (!isGet) { shuffleB_vert2 = std::get<bool>(object.get()->obj); }
			else {
				if (asString) { return std::make_shared<CObject>(makeSettingString(shuffleB_vert2, settingsSig, subSig, "shuffleB_vert2")); }
				return std::make_shared<CObject>(shuffleB_vert2);
			}
		}
		else if (settingsSig == 2356) {
			if (!isGet) { shuffleB_vert3 = std::get<bool>(object.get()->obj); }
			else {
				if (asString) { return std::make_shared<CObject>(makeSettingString(shuffleB_vert3, settingsSig, subSig, "shuffleB_vert3")); }
				return std::make_shared<CObject>(shuffleB_vert3);
			}
		}
	}
	else if (settingsSig >= 2360 && settingsSig < 2370)
	{
		if (settingsSig == 2360)
		{
			return spread_node.putProperty(interpreter, owner, settingsSig, subSig, object, isGet, asPercentage, asString);
		}
		else if (settingsSig == 2361)
		{
			return spread_controller.putProperty(interpreter, owner, settingsSig, subSig, object, isGet, asPercentage, asString);
		}
		else if (settingsSig == 2362)
		{
			return spread_noise.putProperty(interpreter, owner, settingsSig, subSig, object, isGet, asPercentage, asString);
		}
		else if (settingsSig == 2363) {
			if (!isGet) { spreadCorner = stringToTSetProp(std::get<std::string>(object.get()->obj)); }
			else {
				if (asString) { return std::make_shared<CObject>(makeSettingString(spreadCorner, settingsSig, subSig, "spreadCorner")); }
				return std::make_shared<CObject>(TSetPropToString(spreadCorner));
			}
		}
		else if (settingsSig == 2364) {
			if (!isGet) {
				if (asPercentage) {
					spreadSymmetry = percentRange_squared(
						(float)std::get<double>(object.get()->obj), 0.0f, 1.0f, false);
				}
				else { spreadSymmetry = clampf((float)std::get<double>(object.get()->obj), 0.0f, 1.0f); }
			}
			else {
				if (asString) { return std::make_shared<CObject>(makeSettingString(spreadSymmetry, settingsSig, subSig, "spreadSymmetry")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange_squared(spreadSymmetry, 0.0f, 1.0f, true)));
				}
				else { return std::make_shared<CObject>(double(spreadSymmetry)); }
			}
		}
	}
	else if (settingsSig >= 2380 && settingsSig < 2390)
	{
		if (settingsSig == 2380) {
			if (!isGet) { flipX_random = std::get<bool>(object.get()->obj); }
			else {
				if (asString) { return std::make_shared<CObject>(makeSettingString(flipX_random, settingsSig, subSig, "flipX_random")); }
				return std::make_shared<CObject>(flipX_random);
			}
		}
		else if (settingsSig == 2381) {
			if (!isGet) { flipY_random = std::get<bool>(object.get()->obj); }
			else {
				if (asString) { return std::make_shared<CObject>(makeSettingString(flipY_random, settingsSig, subSig, "flipY_random")); }
				return std::make_shared<CObject>(flipY_random);
			}
		}
	}

	if (asString) { return std::make_shared<CObject>(makeSettingString(isEnabled, settingsSig, subSig, "NULL")); }
	return std::make_shared<CObject>(nullptr);
}

TSetControl_Node* TSet_Character::getControlNode(int settingSig, int subSig)
{
	if (settingSig == 2300) { return &scaleNode; }
	else if (settingSig == 2303) { return &sizeX_node; }
	else if (settingSig == 2306) { return &sizeY_node; }
	else if (settingSig == 2320) { return &rotation_node; }
	else if (settingSig == 2330) { return &skewX_node; }
	else if (settingSig == 2333) { return &skewY_node; }
	else if (settingSig == 2340) { return &shuffleA_node; }
	else if (settingSig == 2350) { return &shuffleB_node; }
	else if (settingSig == 2360) { return &spread_node; }
	return nullptr;
}
TSetController* TSet_Character::getController(int settingSig, int subSig)
{
	if (settingSig == 2301) { return &scaleController; }
	else if (settingSig == 2304) { return &scaleController; }
	else if (settingSig == 2307) { return &scaleController; }
	else if (settingSig == 2321) { return &scaleController; }
	else if (settingSig == 2331) { return &scaleController; }
	else if (settingSig == 2334) { return &scaleController; }
	else if (settingSig == 2341) { return &scaleController; }
	else if (settingSig == 2351) { return &scaleController; }
	else if (settingSig == 2361) { return &scaleController; }
	return nullptr;
}
TSetGraph* TSet_Character::getGraph(int settingSig, int subSig)
{
	if (settingSig == 2301)
	{
		if (subSig == -1) { return &scaleController.pressureGraph; }
		else if (subSig == -2) { return &scaleController.directionGraph; }
		else if (subSig == -3) { return &scaleController.tiltGraph; }
		else if (subSig == -4) { return &scaleController.velocityGraph; }
		else if (subSig == -5) { return &scaleController.rotationGraph; }
	}
	else if (settingSig == 2304)
	{
		if (subSig == -1) { return &sizeX_controller.pressureGraph; }
		else if (subSig == -2) { return &sizeX_controller.directionGraph; }
		else if (subSig == -3) { return &sizeX_controller.tiltGraph; }
		else if (subSig == -4) { return &sizeX_controller.velocityGraph; }
		else if (subSig == -5) { return &sizeX_controller.rotationGraph; }
	}
	else if (settingSig == 2307)
	{
		if (subSig == -1) { return &sizeY_controller.pressureGraph; }
		else if (subSig == -2) { return &sizeY_controller.directionGraph; }
		else if (subSig == -3) { return &sizeY_controller.tiltGraph; }
		else if (subSig == -4) { return &sizeY_controller.velocityGraph; }
		else if (subSig == -5) { return &sizeY_controller.rotationGraph; }
	}
	else if (settingSig == 2321)
	{
		if (subSig == -1) { return &rotation_controller.pressureGraph; }
		else if (subSig == -2) { return &rotation_controller.directionGraph; }
		else if (subSig == -3) { return &rotation_controller.tiltGraph; }
		else if (subSig == -4) { return &rotation_controller.velocityGraph; }
		else if (subSig == -5) { return &rotation_controller.rotationGraph; }
	}
	else if (settingSig == 2331)
	{
		if (subSig == -1) { return &skewX_controller.pressureGraph; }
		else if (subSig == -2) { return &skewX_controller.directionGraph; }
		else if (subSig == -3) { return &skewX_controller.tiltGraph; }
		else if (subSig == -4) { return &skewX_controller.velocityGraph; }
		else if (subSig == -5) { return &skewX_controller.rotationGraph; }
	}
	else if (settingSig == 2334)
	{
		if (subSig == -1) { return &skewY_controller.pressureGraph; }
		else if (subSig == -2) { return &skewY_controller.directionGraph; }
		else if (subSig == -3) { return &skewY_controller.tiltGraph; }
		else if (subSig == -4) { return &skewY_controller.velocityGraph; }
		else if (subSig == -5) { return &skewY_controller
			.rotationGraph; }
	}
	else if (settingSig == 2341)
	{
		if (subSig == -1) { return &shuffleA_controller.pressureGraph; }
		else if (subSig == -2) { return &shuffleA_controller.directionGraph; }
		else if (subSig == -3) { return &shuffleA_controller.tiltGraph; }
		else if (subSig == -4) { return &shuffleA_controller.velocityGraph; }
		else if (subSig == -5) { return &shuffleA_controller.rotationGraph; }
	}
	else if (settingSig == 2351)
	{
		if (subSig == -1) { return &shuffleB_controller.pressureGraph; }
		else if (subSig == -2) { return &shuffleB_controller.directionGraph; }
		else if (subSig == -3) { return &shuffleB_controller.tiltGraph; }
		else if (subSig == -4) { return &shuffleB_controller.velocityGraph; }
		else if (subSig == -5) { return &shuffleB_controller.rotationGraph; }
	}
	else if (settingSig == 2361)
	{
		if (subSig == -1) { return &spread_controller.pressureGraph; }
		else if (subSig == -2) { return &spread_controller.directionGraph; }
		else if (subSig == -3) { return &spread_controller.tiltGraph; }
		else if (subSig == -4) { return &spread_controller.velocityGraph; }
		else if (subSig == -5) { return &spread_controller.rotationGraph; }
	}
	return nullptr;
}
TSetNoise* TSet_Character::getNoise(int settingSig, int subSig)
{
	if (settingSig == 2302) { return &scaleNoise; }
	else if (settingSig == 2305) { return &sizeX_noise; }
	else if (settingSig == 2308) { return &sizeY_noise; }
	else if (settingSig == 2322) { return &rotation_noise; }
	else if (settingSig == 2332) { return &skewX_noise; }
	else if (settingSig == 2335) { return &skewY_noise; }
	else if (settingSig == 2342) { return &shuffleA_noise; }
	else if (settingSig == 2352) { return &shuffleB_noise; }
	else if (settingSig == 2362) { return &spread_noise; }
	return nullptr;
}

void TSet_Character::initialize(float* trueSpacing, int* entityCount, glm::ivec2* canvasDimensions)
{
	if (!isEnabled) { return; }
	if (scaleNoise.isEnabled) { scaleNoise.initialize(trueSpacing, entityCount, canvasDimensions); }
	//
	if (sizeX_noise.isEnabled) { sizeX_noise.initialize(trueSpacing, entityCount, canvasDimensions); }
	if (sizeY_noise.isEnabled) { sizeY_noise.initialize(trueSpacing, entityCount, canvasDimensions); }
	//
	if (rotation_noise.isEnabled) { rotation_noise.initialize(trueSpacing, entityCount, canvasDimensions); }
	//
	if (skewX_noise.isEnabled) { skewX_noise.initialize(trueSpacing, entityCount, canvasDimensions); }
	if (skewY_noise.isEnabled) { skewY_noise.initialize(trueSpacing, entityCount, canvasDimensions); }
	//
	if (shuffleA_noise.isEnabled) { shuffleA_noise.initialize(trueSpacing, entityCount, canvasDimensions); }
	if (shuffleB_noise.isEnabled) { shuffleB_noise.initialize(trueSpacing, entityCount, canvasDimensions); }
	//
	if (spread_noise.isEnabled) { spread_noise.initialize(trueSpacing, entityCount, canvasDimensions); }
}

float TSet_Character::fetchPadding(float tipSize)
{
	float paddingX = 0.0f; float paddingY = 0.0f;
	if (sizeX_node.reportEnabled() || sizeX_noise.isEnabled) {
		paddingX += max(tipSize * 0.5f * sizeX_node.controlMin, tipSize * 0.5f * sizeX_node.controlMax);
	}
	if (sizeY_node.reportEnabled() || sizeY_noise.isEnabled) {
		paddingY += max(tipSize * sizeY_node.controlMin, tipSize * sizeY_node.controlMax);
	}
	if (shuffleA_node.reportEnabled() || shuffleA_node.isEnabled || shuffleB_node.reportEnabled() || shuffleB_node.isEnabled) {
		float shuffleMax = max(tipSize * shuffleA_node.controlMin * 0.5f, tipSize * shuffleA_node.controlMax * 0.5f);
		shuffleMax = max(tipSize * shuffleB_node.controlMin * 0.5f, tipSize * shuffleB_node.controlMax * 0.5f);
		paddingX += shuffleMax;
		paddingY += shuffleMax;
	}
	if (skewX_node.reportEnabled() || skewX_node.isEnabled) {
		paddingX += max(tipSize * skewX_node.controlMin, tipSize * skewX_node.controlMax);
	}
	if (skewY_node.reportEnabled() || skewY_node.isEnabled) {
		paddingY += max(tipSize * skewY_node.controlMin, tipSize * skewY_node.controlMax);
	}
	if (spread_node.reportEnabled() || spread_node.isEnabled) {
		paddingX += max(tipSize * spread_node.controlMin * 2.5f, tipSize * spread_node.controlMax * 2.5f);
		paddingY += max(tipSize * spread_node.controlMin * 2.5f, tipSize * spread_node.controlMax * 2.5f);
	}
	std::cout << max(paddingX, paddingY) << std::endl;
	return max(paddingX, paddingY);
}

void TSet_Character::modulateScale(glm::vec4& outScale, float* imageSize, bool calcAll,
	glm::vec3* pos, glm::vec3* origin, glm::vec3* dir, Input* input, int* shardCount, int* anchorCount, int* splineCount)
{
	float scaleControlValue = 1.0f;
	if (scaleNode.reportEnabled()) {
		scaleNode.calculateControl(
			scaleControlValue, pos, dir, input, &scaleController, anchorCount);
	}
	outScale.z = lerpf(scaleNode.controlMin, scaleNode.controlMax, scaleControlValue);
	if (scaleNoise.isEnabled) {
		scaleNoise.calculateNoise(outScale.z, pos, origin, dir, input, shardCount, anchorCount, splineCount, false);
	}
	outScale.z = clampf(outScale.z, scaleNode.rangeMin, scaleNode.rangeMax);
	outScale.w = outScale.z;
	if (flipX_random) {
		glm::vec2 rand = hash2(*pos * (float)scaleNoise.seed);
		outScale.z *= (fract(rand.x * rand.y) > 0.5f) ? -1.0f : 1.0f;
	}
	if (flipY_random) {
		glm::vec2 rand = hash2(*pos * (float)scaleNoise.seed * 1.2f);
		outScale.w *= (fract(rand.x * rand.y) > 0.5f) ? -1.0f : 1.0f;
	}

	if (calcAll) 
	{
		float sizeXControl = 1.0f; float sizeYControl = 1.0f;
		bool sizeXEnabled = false; bool sizeYEnabled = false;

		if (sizeX_node.reportEnabled()) {
			sizeXEnabled = true;
			sizeX_node.calculateControl(
				sizeXControl, pos, dir, input, &sizeX_controller, anchorCount);
		}
		if (sizeX_noise.isEnabled) {
			sizeX_noise.calculateNoise(sizeXControl, pos, origin, dir, input, shardCount, anchorCount, splineCount, false);
		}
		if (!sizeXEnabled && !sizeX_noise.isEnabled) { sizeXControl = 0.0f; }
		outScale.x = lerpf(sizeX_node.controlMin, sizeX_node.controlMax, sizeXControl);
		outScale.x *= (outScale.x < 0.0f) ? (*imageSize * 0.5f) - 1.0f : *imageSize;

		if (sizeY_node.reportEnabled()) {
			sizeYEnabled = true;
			sizeY_node.calculateControl(
				sizeYControl, pos, dir, input, &sizeY_controller, anchorCount);
		}
		if (sizeY_noise.isEnabled) {
			sizeY_noise.calculateNoise(sizeYControl, pos, origin, dir, input, shardCount, anchorCount, splineCount, false);
		}
		if (!sizeYEnabled && !sizeY_noise.isEnabled) { sizeYControl = 0.0f; }
		outScale.y = lerpf(sizeY_node.controlMin, sizeY_node.controlMax, sizeYControl);
		outScale.y *= (outScale.y < 0.0f) ? (*imageSize * 0.5f) - 1.0f : *imageSize;
	}
}

bool TSet_Character::modulateRotation(glm::vec3& outDir,
	glm::vec3* pos, glm::vec3* origin, glm::vec3* dir, Input* input, int* shardCount, int* anchorCount, int* splineCount)
{
	float rotationControl = 1.0f;
	bool rotationEnabled = false;
	if (rotation_node.reportEnabled()) { rotationEnabled = true; }
	if (!rotation_noise.isEnabled && !rotationEnabled) {
		outDir = DEFAULT_DIR; return false;
	}
	// Run Calculations
	if (rotationEnabled) {
		rotationEnabled = true;
		rotation_node.calculateControl(rotationControl, pos, dir, input, &rotation_controller, anchorCount);
	}
	if (rotation_noise.isEnabled) {
		rotation_noise.calculateNoise(rotationControl, pos, origin, dir, input, shardCount, anchorCount, splineCount, false);
	}
	if (!rotationEnabled && !rotation_noise.isEnabled) { rotationControl = 0.0f; }
	rotationControl = lerpf(rotation_node.controlMin, rotation_node.controlMax, rotationControl) * MATH_PI;
	outDir = glm::vec3(cos(rotationControl), sin(rotationControl), 0.0f);
	return true;
}

void TSet_Character::modulateCharacter(glm::vec2& outVert0, glm::vec2& outVert1, glm::vec2& outVert2, glm::vec2& outVert3, float* imageSize,
	glm::vec3* pos, glm::vec3* origin, glm::vec3* dir, Input* input,
	int* shardCount, int* anchorCount, int* splineCount)
{

	float skewXControl = 1.0f; float skewYControl = 1.0f;
	bool skewXEnabled = false; bool skewYEnabled = false;

	float shuffleAControl = 1.0f; float shuffleBControl = 1.0f;
	bool shuffleAEnabled = false; bool shuffleBEnabled = false;

	float spreadControl = 1.0f; 
	bool spreadEnabled = false;

	if (skewX_node.reportEnabled()) {
		skewXEnabled = true;
		skewX_node.calculateControl(
			skewXControl, pos, dir, input, &skewX_controller, anchorCount);
	}
	if (skewX_noise.isEnabled) {
		skewX_noise.calculateNoise(skewXControl, pos, origin, dir, input, shardCount, anchorCount, splineCount, false);
	}
	if (!skewXEnabled && !sizeX_noise.isEnabled) { skewXControl = 0.0f; }
	else { skewXEnabled = true; }

	if (skewY_node.reportEnabled()) {
		skewYEnabled = true;
		skewY_node.calculateControl(
			skewYControl, pos, dir, input, &skewY_controller, anchorCount);
	}
	if (skewY_noise.isEnabled) {
		skewY_noise.calculateNoise(skewYControl, pos, origin, dir, input, shardCount, anchorCount, splineCount, false);
	}
	if (!skewYEnabled && !skewY_noise.isEnabled) { skewYControl = 0.0f; }
	else { skewYEnabled = true; }

	if (shuffleA_vert0 || shuffleA_vert1 || shuffleA_vert2 || shuffleA_vert3)
	{
		if (shuffleA_node.reportEnabled()) {
			shuffleAEnabled = true;
			shuffleA_node.calculateControl(
				shuffleAControl, pos, dir, input, &shuffleA_controller, anchorCount);
		}
		if (shuffleA_noise.isEnabled) {
			shuffleA_noise.calculateNoise(shuffleAControl, pos, origin, dir, input, shardCount, anchorCount, splineCount, false);
		}
		if (!shuffleAEnabled && !shuffleA_noise.isEnabled) { shuffleAControl = 0.0f; }
		else { shuffleAEnabled = true; }
	}
	if (shuffleB_vert0 || shuffleB_vert1 || shuffleB_vert2 || shuffleB_vert3)
	{
		if (shuffleB_node.reportEnabled()) {
			shuffleBEnabled = true;
			shuffleB_node.calculateControl(
				shuffleBControl, pos, dir, input, &shuffleB_controller, anchorCount);
		}
		if (shuffleB_noise.isEnabled) {
			shuffleB_noise.calculateNoise(shuffleBControl, pos, origin, dir, input, shardCount, anchorCount, splineCount, false);
		}
		if (!shuffleBEnabled && !shuffleB_noise.isEnabled) { shuffleBControl = 0.0f; }
		else { shuffleBEnabled = true; }
	}

	if (spread_node.reportEnabled()) {
		spreadEnabled = true;
		spread_node.calculateControl(
			spreadControl, pos, dir, input, &spread_controller, anchorCount);
	}
	if (spread_noise.isEnabled) {
		spread_noise.calculateNoise(spreadControl, pos, origin, dir, input, shardCount, anchorCount, splineCount, false);
	}
	if (!spreadEnabled && !spread_noise.isEnabled) { spreadControl = 0.0f; }
	else { spreadEnabled = true; }

	// Note : For now, swap the intended X/Y channels because the coordinate system is incorrect for the brush tip despite being visually
	// correct when rendered.

	// Currently, Verex 0 is the Upper-Left, 1 is Upper-Right, 2 is Lower-Right, 3 is Lower-Left

	// Apply Skew
	if (skewXEnabled) {
		float skewX = *imageSize * lerpf(skewX_node.controlMin, skewX_node.controlMax, skewXControl);
		outVert0.y += skewX; outVert1.y += skewX;
		outVert2.y -= skewX; outVert3.y -= skewX;
	}
	if (skewYEnabled) {
		float skewY = *imageSize * lerpf(skewY_node.controlMin, skewY_node.controlMax, skewYControl);
		outVert1.x += skewY; outVert2.x += skewY;
		outVert0.x -= skewY; outVert3.x -= skewY;
	}

	// Apply Spread
	if (spreadEnabled) {
		float spread = *imageSize * 2.5f * lerpf(spread_node.controlMin, spread_node.controlMax, spreadControl);
		if (spreadCorner == TSetProp::one) {
			glm::vec2 spreadDir = glm::normalize(glm::vec2(*imageSize, *imageSize));
			outVert0.y += spreadDir.x * spread; outVert0.x += spreadDir.y * spread;
			outVert1.y += spreadDir.x * (spread * 0.3f * (1.0f - spreadSymmetry)); outVert1.x += spreadDir.y * (spread * 0.3f * (1.0f - spreadSymmetry));
			outVert3.y += spreadDir.x * (spread * 0.3f * (1.0f - spreadSymmetry)); outVert3.x += spreadDir.y * (spread * 0.3f * (1.0f - spreadSymmetry));
			outVert2.y -= spreadDir.x * (spread * spreadSymmetry); outVert2.x -= spreadDir.y * (spread * spreadSymmetry);
		}
		else if (spreadCorner == TSetProp::two) {
			glm::vec2 spreadDir = glm::normalize(glm::vec2(-*imageSize, *imageSize));
			outVert1.y += spreadDir.x * spread; outVert1.x += spreadDir.y * spread;
			outVert0.y += spreadDir.x * (spread * 0.3f * (1.0f - spreadSymmetry)); outVert0.x += spreadDir.y * (spread * 0.3f * (1.0f - spreadSymmetry));
			outVert2.y += spreadDir.x * (spread * 0.3f * (1.0f - spreadSymmetry)); outVert2.x += spreadDir.y * (spread * 0.3f * (1.0f - spreadSymmetry));
			outVert3.y -= spreadDir.x * (spread * spreadSymmetry); outVert3.x -= spreadDir.y * (spread * spreadSymmetry);
		}
		else if (spreadCorner == TSetProp::three) {
			glm::vec2 spreadDir = glm::normalize(glm::vec2(-*imageSize, -*imageSize));
			outVert2.y += spreadDir.x * spread; outVert2.x += spreadDir.y * spread;
			outVert1.y += spreadDir.x * (spread * 0.3f * (1.0f - spreadSymmetry)); outVert1.x += spreadDir.y * (spread * 0.3f * (1.0f - spreadSymmetry));
			outVert3.y += spreadDir.x * (spread * 0.3f * (1.0f - spreadSymmetry)); outVert3.x += spreadDir.y * (spread * 0.3f * (1.0f - spreadSymmetry));
			outVert0.y -= spreadDir.x * (spread * spreadSymmetry); outVert0.x -= spreadDir.y * (spread * spreadSymmetry);
		}
		else if (spreadCorner == TSetProp::four) {
			glm::vec2 spreadDir = glm::normalize(glm::vec2(*imageSize, -*imageSize));
			outVert3.y += spreadDir.x * spread; outVert3.x += spreadDir.y * spread;
			outVert0.y += spreadDir.x * (spread * 0.3f * (1.0f - spreadSymmetry)); outVert0.x += spreadDir.y * (spread * 0.3f * (1.0f - spreadSymmetry));
			outVert2.y += spreadDir.x * (spread * 0.3f * (1.0f - spreadSymmetry)); outVert2.x += spreadDir.y * (spread * 0.3f * (1.0f - spreadSymmetry));
			outVert1.y -= spreadDir.x * (spread * spreadSymmetry); outVert1.x -= spreadDir.y * (spread * spreadSymmetry);
		}
	}


	// Apply Shuffle
	if (shuffleAEnabled) {
		float shuffleA = *imageSize * 0.5f * lerpf(shuffleA_node.controlMin, shuffleA_node.controlMax, shuffleAControl);
		if (shuffleA_vert0) { outVert0.y += shuffleA; outVert0.x += shuffleA; }
		if (shuffleA_vert1) { outVert1.y -= shuffleA; outVert1.x += shuffleA; }
		if (shuffleA_vert2) { outVert2.y -= shuffleA; outVert2.x -= shuffleA; }
		if (shuffleA_vert3) { outVert3.y += shuffleA; outVert3.x -= shuffleA; }
	}
	if (shuffleBEnabled) {
		float shuffleB = *imageSize * 0.5f * lerpf(shuffleB_node.controlMin, shuffleB_node.controlMax, shuffleBControl);
		if (shuffleA_vert0) { outVert0.y += shuffleB; outVert0.x += shuffleB; }
		if (shuffleA_vert1) { outVert1.y -= shuffleB; outVert1.x += shuffleB; }
		if (shuffleA_vert2) { outVert2.y -= shuffleB; outVert2.x -= shuffleB; }
		if (shuffleA_vert3) { outVert3.y += shuffleB; outVert3.x -= shuffleB; }
	}
}