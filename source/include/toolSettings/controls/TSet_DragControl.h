#ifndef TSET_DRAGCONTROL_H
#define TSET_DRAGCONTROL_H

class CFunction;
class CObject;
class CInterpreter;
class Tool;

#include <glm.hpp>
#include <string>
#include <vector>
#include <memory>
#include <map>

class ToolSettings;

#include "../../ToolSettings.h"
#include "../../structs.h"

////////////////////////////////////////////////////////////////
// Drag Control Settings
////////////////////////////////////////////////////////////////

class TSet_DragControl : public ToolSettings // 500
{
public:
	int snapModeKey; // 500
	float snapAngleIncrement; // 501
	int moveOriginModeKey; // Disabled when connectInputs is true // 502
	int sizeAboutOriginKey; // 503
	bool pressureMapping; // 504
	float pressureMapStrength; // 505
	float pressureMapDecayRate; // 506
	bool pressureMapRelative; // 507
	float pressureMapThreshold; // 508
	bool connectInputs; // 509
	int connectInputKey; // 510
	int terminateInputKey; // 511
	bool enableExtrusion = false; // Hidden Setting // 512
	// Extrusion Settings, only visible for tools with it enabled
	int extrudeModeKey; // 513
	float initialExtrusion; // 514
	TSetProp extrudeSnappingMode; // 515
	float extrudeSnapIncrement; // 516
	virtual TSetType getType() { return type; }
	virtual std::shared_ptr<CObject> putProperty(
		std::shared_ptr<CInterpreter> interpreter, std::shared_ptr<Tool> owner,
		int settingsSig, int subSig, std::shared_ptr<CObject> object, bool isGet, bool asPercentage, bool asString);
	TSet_DragControl();
};

#endif