#ifndef TSET_ZOOM_H
#define TSET_ZOOM_H

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
// Zoom Settings
////////////////////////////////////////////////////////////////

#define TSET_ZOOM_SCRUBTHRESHOLD_MIN 4
#define TSET_ZOOM_SCRUBTHRESHOLD_MAX 64
#define TSET_ZOOM_ZOOMFACTOR_MIN 20.0f
#define TSET_ZOOM_ZOOMFACTOR_MAX 3000.0f
#define TSET_ZOOM_STEPAMOUNT_MIN 10.0f
#define TSET_ZOOM_STEPAMOUNT_MAX 1000.0f
#define TSET_ZOOM_SPEED_MIN 0.01f
#define TSET_ZOOM_SPEED_MAX 30.0f

class TSet_Zoom : public ToolSettings // 200
{
public:
	int scrubThreshold; // 200
	float minZoomFactor; // 201
	float maxZoomFactor; // 202
	float stepZoomAmount; // 210
	float zoomSpeed; // 220
	float zoomFactor_A; // 221
	virtual TSetType getType() { return type; }
	virtual std::shared_ptr<CObject> putProperty(
		std::shared_ptr<CInterpreter> interpreter, std::shared_ptr<Tool> owner,
		int settingsSig, int subSig, std::shared_ptr<CObject> object, bool isGet, bool asPercentage, bool asString);
	TSet_Zoom();
};

#endif