#ifndef TSET_PAN_H
#define TSET_PAN_H

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

////////////////////////////////////////////////////////////////
// Pan Settings
////////////////////////////////////////////////////////////////

#define TSET_PAN_CONSTRAINBORDER_MIN 0.01f
#define TSET_PAN_CONSTRAINBORDER_MAX 500.0f
#define TSET_PAN_FRICTION_MIN 0.01f
#define TSET_PAN_FRICTION_MAX 100.0f
#define TSET_PAN_FORCE_MIN 0.01f
#define TSET_PAN_FORCE_MAX 1000.0f
#define TSET_PAN_SPEEDAMOUNT_MIN 0.0f
#define TSET_PAN_SPEEDAMOUNT_MAX 100.0f
#define TSET_PAN_SLOWFACTOR_MIN 0.01f
#define TSET_PAN_SLOWFACTOR_MAX 1.0f
#define TSET_PAN_CUSTOMANGLE_MIN -180.0f
#define TSET_PAN_CUSTOMANGLE_MAX 180.0f

class TSet_Pan : public ToolSettings // 100
{
public:
	bool constrainToCanvas; // 100
	bool constrainBorderRelative; // 101
	float constrainBorderAmount; // 102
	bool doFlickPanning; // 110
	float friction; // 111
	float force; // 112
	float speedAmount; // 113
	Keybind slowModKey; // 116
	float slowFactor; // 117
	bool clickTargetPanning; // 120
	Keybind clickTarModKey; // 122
	bool lockToX; // 130
	Keybind lockXModKey; // 131
	bool lockToY; // 132
	Keybind lockYModKey; // 133
	bool lockCustomAngle; // 134
	float customAngle; // 135
	Keybind lockCustomModKey; // 136
	bool cameraPosRecording; // 150
	Keybind cameraPosRecordKey; // 151
	virtual TSetType getType() { return type; }

	virtual TSetControl_Node* getControlNode(int settingSig, int subSig);
	virtual TSetController* getController(int settingSig, int subSig);
	virtual TSetGraph* getGraph(int settingSig, int subSig);
	virtual TSetNoise* getNoise(int settingSig, int subSig);

	virtual std::shared_ptr<CObject> putProperty(
		std::shared_ptr<CInterpreter> interpreter, std::shared_ptr<Tool> owner,
		int settingsSig, int subSig, std::shared_ptr<CObject> object, bool isGet, bool asPercentage, bool asString);
	TSet_Pan();
};

#endif