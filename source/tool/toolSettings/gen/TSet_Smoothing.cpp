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

#include "../../../include/tool/toolSettings/gen/TSet_Smoothing.h"

////////////////////////////////////////////////////////////////
// Smoothing Settings
////////////////////////////////////////////////////////////////

TSet_Smoothing::TSet_Smoothing()
{
	this->type = TSetType::smoothing;
	this->isEnabled = false;
	this->smoothDirection = false;
	this->directionThreshold = 35.0f;
	this->directionFactor = 0.7f;
	this->smoothDirectionCurve = false;
	this->curveDirectionThreshold = 20.0f;
	this->curveDirectionFactor = 0.4f;
}

std::shared_ptr<CObject> TSet_Smoothing::putProperty(
	std::shared_ptr<CInterpreter> interpreter, std::shared_ptr<Tool> owner,
	int settingsSig, int subSig, std::shared_ptr<CObject> object, bool isGet, bool asPercentage, bool asString)
{
	if (asString) { return std::make_shared<CObject>(makeSettingString(isEnabled, settingsSig, subSig, "NULL")); }
	return std::make_shared<CObject>(nullptr);
}

TSetControl_Node* TSet_Smoothing::getControlNode(int settingSig, int subSig)
{
	return nullptr;
}
TSetController* TSet_Smoothing::getController(int settingSig, int subSig)
{
	return nullptr;
}
TSetGraph* TSet_Smoothing::getGraph(int settingSig, int subSig)
{
	return nullptr;
}
TSetNoise* TSet_Smoothing::getNoise(int settingSig, int subSig)
{
	return nullptr;
}