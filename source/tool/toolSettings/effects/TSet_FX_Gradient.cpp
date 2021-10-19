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

#include "../../../include/tool/toolSettings/effects/TSet_FX_Gradient.h"

////////////////////////////////////////////////////////////////
// FX GRADIENT Settings
////////////////////////////////////////////////////////////////

TSet_FX_Gradient::TSet_FX_Gradient()
{
	this->type = TSetType::fx_gradient;
	this->isEnabled = false;
	this->direction = glm::vec3(0.0f, 1.0f, 0.0f);
	this->mixAmount = 0.25f;
}
std::shared_ptr<CObject> TSet_FX_Gradient::putProperty(
	std::shared_ptr<CInterpreter> interpreter, std::shared_ptr<Tool> owner,
	int settingsSig, int subSig, std::shared_ptr<CObject> object, bool isGet, bool asPercentage, bool asString)
{
	if (asString) { return std::make_shared<CObject>(makeSettingString(isEnabled, settingsSig, subSig, "NULL")); }
	return std::make_shared<CObject>(nullptr);
}

TSetControl_Node* TSet_FX_Gradient::getControlNode(int settingSig, int subSig)
{
	return nullptr;
}
TSetController* TSet_FX_Gradient::getController(int settingSig, int subSig)
{
	return nullptr;
}
TSetGraph* TSet_FX_Gradient::getGraph(int settingSig, int subSig)
{
	return nullptr;
}
TSetNoise* TSet_FX_Gradient::getNoise(int settingSig, int subSig)
{
	return nullptr;
}


void TSet_FX_Gradient::updateXY(glm::vec3 pos)
{
	if (rebuildBounds) { fillX.x = fillX.y = pos.x; fillY.x = fillY.y = pos.y; rebuildBounds = false; }
	else
	{
		if (pos.x > fillX.y) { fillX.y = pos.x; }
		else if (pos.x < fillX.x) { fillX.x = pos.x; }
		if (pos.y > fillY.y) { fillY.y = pos.y; }
		else if (pos.y < fillY.x) { fillY.x = pos.y; }
		if ((fillX.y - fillX.x) < (abs(fillY.y - fillY.x))) { isPortrait = true; }
		else { isPortrait = false; }
	}
	center = glm::vec3(lerpf(fillX.x, fillX.y, 0.5f), lerpf(fillY.x, fillY.y, 0.5f), 0.0f);
}
void TSet_FX_Gradient::initializeData(CColor FGColor, CColor BGColor, int tipSize)
{
}