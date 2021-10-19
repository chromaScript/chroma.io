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

#include "../../../include/tool/toolSettings/gen/TSet_Alpha.h"

////////////////////////////////////////////////////////////////
// Alpha Dynamics
////////////////////////////////////////////////////////////////

TSet_Alpha::TSet_Alpha()
{
	this->type = TSetType::alpha;
	this->isEnabled = false;
}
std::shared_ptr<CObject> TSet_Alpha::putProperty(
	std::shared_ptr<CInterpreter> interpreter, std::shared_ptr<Tool> owner,
	int settingsSig, int subSig, std::shared_ptr<CObject> object, bool isGet, bool asPercentage, bool asString)
{
	if (settingsSig == 2800)
	{
		return opacityNode.putProperty(interpreter, owner, settingsSig, subSig, object, isGet, asPercentage, asString);
	}
	else if (settingsSig == 2801)
	{
		return opacityController.putProperty(interpreter, owner, settingsSig, subSig, object, isGet, asPercentage, asString);
	}
	else if (settingsSig == 2802)
	{
		return opacityNoise.putProperty(interpreter, owner, settingsSig, subSig, object, isGet, asPercentage, asString);
	}
	else if (settingsSig == 2803)
	{
		return flowNode.putProperty(interpreter, owner, settingsSig, subSig, object, isGet, asPercentage, asString);
	}
	else if (settingsSig == 2804)
	{
		return flowController.putProperty(interpreter, owner, settingsSig, subSig, object, isGet, asPercentage, asString);
	}
	else if (settingsSig == 2805)
	{
		return flowNoise.putProperty(interpreter, owner, settingsSig, subSig, object, isGet, asPercentage, asString);
	}
	if (asString) { return std::make_shared<CObject>(makeSettingString(isEnabled, settingsSig, subSig, "NULL")); }
	return std::make_shared<CObject>(nullptr);
}

TSetControl_Node* TSet_Alpha::getControlNode(int settingSig, int subSig)
{
	if (settingSig == 2800)
	{
		return &opacityNode;
	}
	else if (settingSig == 2803)
	{
		return &flowNode;
	}
	return nullptr;
}
TSetController* TSet_Alpha::getController(int settingSig, int subSig)
{
	if (settingSig == 2801)
	{
		return &opacityController;
	}
	else if (settingSig == 2804)
	{
		return &flowController;
	}
	return nullptr;
}
TSetGraph* TSet_Alpha::getGraph(int settingSig, int subSig)
{
	if (settingSig == 2801)
	{
		if (subSig == -1) { return &opacityController.pressureGraph; }
		else if (subSig == -2) { return &opacityController.directionGraph; }
		else if (subSig == -3) { return &opacityController.tiltGraph; }
		else if (subSig == -4) { return &opacityController.velocityGraph; }
		else if (subSig == -5) { return &opacityController.rotationGraph; }
	}
	else if (settingSig == 2804)
	{
		if (subSig == -1) { return &flowController.pressureGraph; }
		else if (subSig == -2) { return &flowController.directionGraph; }
		else if (subSig == -3) { return &flowController.tiltGraph; }
		else if (subSig == -4) { return &flowController.velocityGraph; }
		else if (subSig == -5) { return &flowController.rotationGraph; }
	}
	return nullptr;
}
TSetNoise* TSet_Alpha::getNoise(int settingSig, int subSig)
{
	if (settingSig == 2802)
	{
		return &opacityNoise;
	}
	else if (settingSig == 2805)
	{
		return &flowNoise;
	}
	return nullptr;
}

void TSet_Alpha::initialize(float* trueSpacing, int* entityCount, glm::ivec2* canvasDimensions)
{
	if (!isEnabled) { return; }
	if (opacityNoise.isEnabled) { opacityNoise.initialize(trueSpacing, entityCount, canvasDimensions); }
	if (flowNoise.isEnabled) { flowNoise.initialize(trueSpacing, entityCount, canvasDimensions); }
}

void TSet_Alpha::modulateAlpha(
	float& outOpacity, float& outFlow, 
	glm::vec3* pos, glm::vec3* origin, glm::vec3* dir, Input* input, 
	int* shardCount, int* anchorCount, int* splineCount)
{
	float opacityControlValue = 1.0f;
	if (opacityNode.reportEnabled()) { opacityNode.calculateControl(
		opacityControlValue, pos, dir, input, &opacityController, anchorCount); }
	outOpacity = lerpf(opacityNode.controlMin, opacityNode.controlMax, opacityControlValue);
	if (opacityNoise.isEnabled) {
		opacityNoise.calculateNoise(outOpacity, pos, origin, dir, input, shardCount, anchorCount, splineCount, false);
	}
	outOpacity = clampf(outOpacity, opacityNode.rangeMin, opacityNode.rangeMax);
	

	float flowControlValue = 1.0f;
	if (flowNode.reportEnabled()) { flowNode.calculateControl(
		flowControlValue, pos, dir, input, &flowController, anchorCount); }
	outFlow = lerpf(flowNode.controlMin, flowNode.controlMax, flowControlValue);
	if (flowNoise.isEnabled) { 
		flowNoise.calculateNoise(outFlow, pos, origin, dir, input, shardCount, anchorCount, splineCount, false);
	}
	outFlow = clampf(outFlow, flowNode.rangeMin, flowNode.rangeMax);
}