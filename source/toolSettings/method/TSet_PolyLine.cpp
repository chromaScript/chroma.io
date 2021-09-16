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
#include "../../include/toolSettings/method/TSet_PolyLine.h"

////////////////////////////////////////////////////////////////
// Polyline Settings
////////////////////////////////////////////////////////////////

TSet_PolyLine::TSet_PolyLine()
{
	this->type = TSetType::polyline;
	this->isEnabled = false;
	this->useMiterJoints = true;
	this->maxMiterLength = 40.0f;
	this->bevelType = TSetProp::polygon;
	this->bevelWidth = 60.0f;
	this->segmentCount = 6;
	this->constantSegmentCount = false;
	this->startCapType = TSetProp::polyCircle;
	this->startCapScale = 1.0f;
	this->endCapType = TSetProp::polyCircle;
	this->endCapScale = 1.0f;
}

std::shared_ptr<CObject> TSet_PolyLine::putProperty(
	std::shared_ptr<CInterpreter> interpreter, std::shared_ptr<Tool> owner,
	int settingsSig, int subSig, std::shared_ptr<CObject> object, bool isGet, bool asPercentage, bool asString)
{
	if (asString) { return std::make_shared<CObject>(makeSettingString(isEnabled, settingsSig, subSig, "NULL")); }
	return std::make_shared<CObject>(nullptr);
}
