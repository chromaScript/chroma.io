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

#include "../../../include/tool/toolSettings/method/TSet_Fan.h"

////////////////////////////////////////////////////////////////
// Fan Settings
////////////////////////////////////////////////////////////////

TSet_Fan::TSet_Fan()
{
	this->type = TSetType::fan;
	this->isEnabled = false;
	this->subdivisionCount = 0;
	this->scatteringMode = TSetProp::circular;
	this->angleVariance_simple = 0.0f;
	this->radiusVariance_simple = 0.0f;
	this->offsetVarianceX_simple = 0.0f;
	this->offsetVarianceY_simple = 0.0f;
	this->connectionOrdering = TSetProp::useDefault;
	this->airbrushMode = true;
}

std::shared_ptr<CObject> TSet_Fan::putProperty(
	std::shared_ptr<CInterpreter> interpreter, std::shared_ptr<Tool> owner,
	int settingsSig, int subSig, std::shared_ptr<CObject> object, bool isGet, bool asPercentage, bool asString)
{
	if (asString) { return std::make_shared<CObject>(makeSettingString(isEnabled, settingsSig, subSig, "NULL")); }
	return std::make_shared<CObject>(nullptr);
}

TSetControl_Node* TSet_Fan::getControlNode(int settingSig, int subSig)
{
	return nullptr;
}
TSetController* TSet_Fan::getController(int settingSig, int subSig)
{
	return nullptr;
}
TSetGraph* TSet_Fan::getGraph(int settingSig, int subSig)
{
	return nullptr;
}
TSetNoise* TSet_Fan::getNoise(int settingSig, int subSig)
{
	return nullptr;
}