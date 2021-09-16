#ifndef TSET_ONEPOINTCONTROL_H
#define TSET_ONEPOINTCONTROL_H

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
// One Point Settings
////////////////////////////////////////////////////////////////

class TSet_OnePointControl : public ToolSettings // 600
{
public:
	float initialRotation; // 600
	int constrainRatioKey; // 601
	float constrainAngleIncrement; // 602
	int moveOriginModeKey; // 603
	int rotateModeKey; // 604
	int sizeAboutOriginKey; // 605
	bool pressureMapping; // 606
	float pressureMapStrength; // 607
	float pressureMapDecayRate; // 608
	bool pressureMapRelative; // 609
	float pressureMapThreshold; // 610
	virtual TSetType getType() { return type; }
	virtual std::shared_ptr<CObject> putProperty(
		std::shared_ptr<CInterpreter> interpreter, std::shared_ptr<Tool> owner,
		int settingsSig, int subSig, std::shared_ptr<CObject> object, bool isGet, bool asPercentage, bool asString);
	TSet_OnePointControl();
};

#endif