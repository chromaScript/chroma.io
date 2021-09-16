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
#include "../../include/toolSettings/controls/TSet_OnePointControl.h"

////////////////////////////////////////////////////////////////
// One Point Settings
////////////////////////////////////////////////////////////////

TSet_OnePointControl::TSet_OnePointControl()
{
	this->type = TSetType::onepoint;
	this->isEnabled = true;
	this->initialRotation = 0.0f;
	this->constrainAngleIncrement = 15.0f;
	this->constrainRatioKey = 340; // Left-Shift
	this->moveOriginModeKey = 32; // Space
	this->rotateModeKey = 342; // Left-Alt
	this->sizeAboutOriginKey = 341; // Left-Ctrl
	this->pressureMapping = true;
	this->pressureMapStrength = 0.9f;
	this->pressureMapDecayRate = 0.1f;
	this->pressureMapRelative = false;
	this->pressureMapThreshold = 120.0f;
}

std::shared_ptr<CObject> TSet_OnePointControl::putProperty(
	std::shared_ptr<CInterpreter> interpreter, std::shared_ptr<Tool> owner,
	int settingsSig, int subSig, std::shared_ptr<CObject> object, bool isGet, bool asPercentage, bool asString)
{
	if (asString) { return std::make_shared<CObject>(makeSettingString(isEnabled, settingsSig, subSig, "NULL")); }
	return std::make_shared<CObject>(nullptr);
}