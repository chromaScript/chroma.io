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

#include "../../../include/tool/toolSettings/gen/TSet_Mixing.h"

TSet_Mixing::TSet_Mixing()
{
	this->type = TSetType::mixing;
	this->isEnabled = false;
}
std::shared_ptr<CObject> TSet_Mixing::putProperty(
	std::shared_ptr<CInterpreter> interpreter, std::shared_ptr<Tool> owner,
	int settingsSig, int subSig, std::shared_ptr<CObject> object, bool isGet, bool asPercentage, bool asString)
{
	if (asString) { return std::make_shared<CObject>(makeSettingString(isEnabled, settingsSig, subSig, "NULL")); }
	return std::make_shared<CObject>(nullptr);
}

TSetControl_Node* TSet_Mixing::getControlNode(int settingSig, int subSig)
{
	return nullptr;
}
TSetController* TSet_Mixing::getController(int settingSig, int subSig)
{
	return nullptr;
}
TSetGraph* TSet_Mixing::getGraph(int settingSig, int subSig)
{
	return nullptr;
}
TSetNoise* TSet_Mixing::getNoise(int settingSig, int subSig)
{
	return nullptr;
}