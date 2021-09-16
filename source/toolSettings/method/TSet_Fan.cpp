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
#include "../../include/toolSettings/method/TSet_Fan.h"

////////////////////////////////////////////////////////////////
// Fan Settings
////////////////////////////////////////////////////////////////

TSet_Fan::TSet_Fan()
{
	this->type = TSetType::fan;
	this->isEnabled = false;
	this->waitMode = TSetProp::distance;
	this->waitAmount = 0.0f;
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