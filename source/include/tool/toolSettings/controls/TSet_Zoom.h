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

////////////////////////////////////////////////////////////////
// Zoom Settings
////////////////////////////////////////////////////////////////

class TSet_Zoom : public ToolSettings // 200
{
public:
	int scrubThreshold; // 200
	float accelerationIn; // 201
	float accelerationOut; // 202
	float zoomSpeed; // 203
	bool clickTarZoom; // 204
	bool animatedZoom; // 205
	float animSpeed; // 206
	std::string zoomStepField = ""; // 210
	std::vector<float> zoomSteps;

	virtual TSetType getType() { return type; }

	virtual TSetControl_Node* getControlNode(int settingSig, int subSig);
	virtual TSetController* getController(int settingSig, int subSig);
	virtual TSetGraph* getGraph(int settingSig, int subSig);

	virtual TSetNoise* getNoise(int settingSig, int subSig);
	virtual std::shared_ptr<CObject> putProperty(
		std::shared_ptr<CInterpreter> interpreter, std::shared_ptr<Tool> owner,
		int settingsSig, int subSig, std::shared_ptr<CObject> object, bool isGet, bool asPercentage, bool asString);
	TSet_Zoom();
};

#endif