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

#include "../../../include/tool/toolSettings/controls/TSet_DragControl.h"

////////////////////////////////////////////////////////////////
// Drag Control Settings
////////////////////////////////////////////////////////////////

TSet_DragControl::TSet_DragControl()
{
	this->type = TSetType::drag;
	this->isEnabled = true;
	this->snapModeKey = Keybind(InputKey::unknown, InputModKey::shift);
	this->snapAngleIncrement = 15.0f;
	this->moveOriginModeKey = Keybind(InputKey::space, InputModKey::none);
	this->sizeAboutOriginKey = Keybind(InputKey::unknown, InputModKey::ctrl);
	this->pressureMapping = true;
	this->pressureMapStrength = 0.9f;
	this->pressureMapDecayRate = 0.1f;
	this->pressureMapRelative = false;
	this->pressureMapThreshold = 120.0f;
	this->connectInputs = true;
	this->connectInputKey = Keybind(InputKey::unknown, InputModKey::none);
	this->terminateInputKey = Keybind(InputKey::unknown, InputModKey::none);
	// Extrusion Settings, only visible for tools with it enabled
	this->extrudeModeKey = Keybind(InputKey::unknown, InputModKey::alt);
	this->initialExtrusion = 0.0f;
	this->extrudeSnappingMode = TSetProp::distance;
	this->extrudeSnapIncrement = 10.0f;
}

std::shared_ptr<CObject> TSet_DragControl::putProperty(
	std::shared_ptr<CInterpreter> interpreter, std::shared_ptr<Tool> owner,
	int settingsSig, int subSig, std::shared_ptr<CObject> object, bool isGet, bool asPercentage, bool asString)
{
	if (asString) { return std::make_shared<CObject>(makeSettingString(isEnabled, settingsSig, subSig, "NULL")); }
	return std::make_shared<CObject>(nullptr);
}

TSetControl_Node* TSet_DragControl::getControlNode(int settingSig, int subSig)
{
	return nullptr;
}
TSetController* TSet_DragControl::getController(int settingSig, int subSig)
{
	return nullptr;
}
TSetGraph* TSet_DragControl::getGraph(int settingSig, int subSig)
{
	return nullptr;
}
TSetNoise* TSet_DragControl::getNoise(int settingSig, int subSig)
{
	return nullptr;
}