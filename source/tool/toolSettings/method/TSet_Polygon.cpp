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

#include "../../../include/tool/toolSettings/method/TSet_Polygon.h"

////////////////////////////////////////////////////////////////
// Polygon Settings
////////////////////////////////////////////////////////////////

TSet_Polygon::TSet_Polygon()
{
	this->type = TSetType::polygon;
	this->isEnabled = false;
	this->shapeType = TSetProp::rectangle;
	this->sides = 4;
	this->size = 160.0f;
}

std::shared_ptr<CObject> TSet_Polygon::putProperty(
	std::shared_ptr<CInterpreter> interpreter, std::shared_ptr<Tool> owner,
	int settingsSig, int subSig, std::shared_ptr<CObject> object, bool isGet, bool asPercentage, bool asString)
{
	if (asString) { return std::make_shared<CObject>(makeSettingString(isEnabled, settingsSig, subSig, "NULL")); }
	return std::make_shared<CObject>(nullptr);
}

TSetControl_Node* TSet_Polygon::getControlNode(int settingSig, int subSig)
{
	return nullptr;
}
TSetController* TSet_Polygon::getController(int settingSig, int subSig)
{
	return nullptr;
}
TSetGraph* TSet_Polygon::getGraph(int settingSig, int subSig)
{
	return nullptr;
}
TSetNoise* TSet_Polygon::getNoise(int settingSig, int subSig)
{
	return nullptr;
}