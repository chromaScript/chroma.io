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

#include "../../../include/tool/toolSettings/method/TSet_Mesh.h"

////////////////////////////////////////////////////////////////
// Mesh Settings
////////////////////////////////////////////////////////////////

TSet_Mesh::TSet_Mesh()
{
	this->type = TSetType::mesh;
	this->isEnabled = false;
}

std::shared_ptr<CObject> TSet_Mesh::putProperty(
	std::shared_ptr<CInterpreter> interpreter, std::shared_ptr<Tool> owner,
	int settingsSig, int subSig, std::shared_ptr<CObject> object, bool isGet, bool asPercentage, bool asString)
{
	if (asString) { return std::make_shared<CObject>(makeSettingString(isEnabled, settingsSig, subSig, "NULL")); }
	return std::make_shared<CObject>(nullptr);
}

TSetControl_Node* TSet_Mesh::getControlNode(int settingSig, int subSig)
{
	return nullptr;
}
TSetController* TSet_Mesh::getController(int settingSig, int subSig)
{
	return nullptr;
}
TSetGraph* TSet_Mesh::getGraph(int settingSig, int subSig)
{
	return nullptr;
}
TSetNoise* TSet_Mesh::getNoise(int settingSig, int subSig)
{
	return nullptr;
}