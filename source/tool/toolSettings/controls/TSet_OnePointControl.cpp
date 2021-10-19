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

#include "../../../include/tool/toolSettings/controls/TSet_OnePointControl.h"

////////////////////////////////////////////////////////////////
// One Point Settings
////////////////////////////////////////////////////////////////

TSet_OnePointControl::TSet_OnePointControl()
{
	this->type = TSetType::onepoint;
	this->isEnabled = true;
	this->initialRotation = 0.0f;
	this->constrainAngleIncrement = 15.0f;
	this->constrainRatioKey = Keybind(InputKey::unknown, InputModKey::shift); // Left-Shift
	this->moveOriginModeKey = Keybind(InputKey::space, InputModKey::none); // Space
	this->rotateModeKey = Keybind(InputKey::unknown, InputModKey::alt); // Left-Alt
	this->sizeAboutOriginKey = Keybind(InputKey::unknown, InputModKey::ctrl); // Left-Ctrl
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

TSetControl_Node* TSet_OnePointControl::getControlNode(int settingSig, int subSig)
{
	return nullptr;
}
TSetController* TSet_OnePointControl::getController(int settingSig, int subSig)
{
	return nullptr;
}
TSetGraph* TSet_OnePointControl::getGraph(int settingSig, int subSig)
{
	return nullptr;
}
TSetNoise* TSet_OnePointControl::getNoise(int settingSig, int subSig)
{
	return nullptr;
}