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
#include "../../include/toolSettings/controls/TSet_DragControl.h"

////////////////////////////////////////////////////////////////
// Drag Control Settings
////////////////////////////////////////////////////////////////

TSet_DragControl::TSet_DragControl()
{
	this->type = TSetType::drag;
	this->isEnabled = true;
	this->snapModeKey = 340; // Left-Shift
	this->snapAngleIncrement = 15.0f;
	this->moveOriginModeKey = 32; // Space
	this->sizeAboutOriginKey = 341; // Left-Ctrl
	this->pressureMapping = true;
	this->pressureMapStrength = 0.9f;
	this->pressureMapDecayRate = 0.1f;
	this->pressureMapRelative = false;
	this->pressureMapThreshold = 120.0f;
	this->connectInputs = true;
	this->connectInputKey = 32; // Space
	this->terminateInputKey = 32; // Space
	// Extrusion Settings, only visible for tools with it enabled
	this->extrudeModeKey = 342; // Left-Alt
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