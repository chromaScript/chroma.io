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
#include "../../include/toolSettings/effects/TSet_FX_Gradient.h"

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