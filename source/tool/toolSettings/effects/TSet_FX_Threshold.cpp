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

#include "../../../include/tool/toolSettings/effects/TSet_FX_Threshold.h"

////////////////////////////////////////////////////////////////
// FX THRESHOLD Settings
////////////////////////////////////////////////////////////////

TSet_FX_Threshold::TSet_FX_Threshold()
{
	this->type = TSetType::fx_threshold;
	this->isEnabled = false;
	this->level = 0.3f;
	this->lowColor = red;
	this->upperColor = blue;
	this->mixAmount = 0.8f;
	this->mixLower = 0.8f;
	this->mixUpper = 1.0f;
}
std::shared_ptr<CObject> TSet_FX_Threshold::putProperty(
	std::shared_ptr<CInterpreter> interpreter, std::shared_ptr<Tool> owner,
	int settingsSig, int subSig, std::shared_ptr<CObject> object, bool isGet, bool asPercentage, bool asString)
{
	if (asString) { return std::make_shared<CObject>(makeSettingString(isEnabled, settingsSig, subSig, "NULL")); }
	return std::make_shared<CObject>(nullptr);
}

TSetControl_Node* TSet_FX_Threshold::getControlNode(int settingSig, int subSig)
{
	return nullptr;
}
TSetController* TSet_FX_Threshold::getController(int settingSig, int subSig)
{
	return nullptr;
}
TSetGraph* TSet_FX_Threshold::getGraph(int settingSig, int subSig)
{
	return nullptr;
}
TSetNoise* TSet_FX_Threshold::getNoise(int settingSig, int subSig)
{
	return nullptr;
}

void TSet_FX_Threshold::initializeData(CColor FGColor, CColor BGColor)
{
}