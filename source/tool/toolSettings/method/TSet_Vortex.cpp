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

#include "../../../include/tool/toolSettings/method/TSet_Vortex.h"

////////////////////////////////////////////////////////////////
// Vortex Settings
////////////////////////////////////////////////////////////////

TSet_Vortex::TSet_Vortex()
{
	this->type = TSetType::vortex;
	this->isEnabled = false;
	this->constantRingCount = true;
	this->rings = 3.0f;
	this->radius = 120.0f;
	this->ringDensity = 24;
	this->ringSpacingMode = TSetProp::automatic;
	this->ringSpacing_simple = 0.0f;
	this->ringRotationOffset = 0.0f;
	this->ringRotationRandom = 0.0f;
	this->scatteringMode = TSetProp::circular;
	this->angleVariance_simple = 0.0f;
	this->radiusVariance_simple = 0.0f;
	this->offsetVarianceX_simple = 0.0f;
	this->offsetVarianceY_simple = 0.0f;
	this->connectionOrdering = TSetProp::useDefault;
}

std::shared_ptr<CObject> TSet_Vortex::putProperty(
	std::shared_ptr<CInterpreter> interpreter, std::shared_ptr<Tool> owner,
	int settingsSig, int subSig, std::shared_ptr<CObject> object, bool isGet, bool asPercentage, bool asString)
{
	if (asString) { return std::make_shared<CObject>(makeSettingString(isEnabled, settingsSig, subSig, "NULL")); }
	return std::make_shared<CObject>(nullptr);
}

TSetControl_Node* TSet_Vortex::getControlNode(int settingSig, int subSig)
{
	return nullptr;
}
TSetController* TSet_Vortex::getController(int settingSig, int subSig)
{
	return nullptr;
}
TSetGraph* TSet_Vortex::getGraph(int settingSig, int subSig)
{
	return nullptr;
}
TSetNoise* TSet_Vortex::getNoise(int settingSig, int subSig)
{
	return nullptr;
}