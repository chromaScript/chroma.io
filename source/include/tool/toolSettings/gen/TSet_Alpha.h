#ifndef TSET_ALPHA_H
#define TSET_ALPHA_H

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
// Alpha Settings
////////////////////////////////////////////////////////////////

class TSet_Alpha : public ToolSettings // 2800
{
public:
	TSetControl_Node opacityNode = TSetControl_Node(0.0f, 1.0f, false, "OPACITY"); // 2800
	TSetController opacityController; // 2801
	TSetNoise opacityNoise = TSetNoise(0.0f, 1.0f, false, "OPACITY"); // 2802

	TSetControl_Node flowNode = TSetControl_Node(0.0f, 1.0f, false, "FLOW");; // 2803
	TSetController flowController; // 2804
	TSetNoise flowNoise = TSetNoise(0.0f, 1.0f, false, "FLOW"); // 2805

	virtual TSetType getType() { return type; }

	virtual TSetControl_Node* getControlNode(int settingSig, int subSig);
	virtual TSetController* getController(int settingSig, int subSig);
	virtual TSetGraph* getGraph(int settingSig, int subSig);
	virtual TSetNoise* getNoise(int settingSig, int subSig);

	virtual std::shared_ptr<CObject> putProperty(
		std::shared_ptr<CInterpreter> interpreter, std::shared_ptr<Tool> owner,
		int settingsSig, int subSig, std::shared_ptr<CObject> object, bool isGet, bool asPercentage, bool asString);
	TSet_Alpha();
	void initialize(float* trueSpacing, int* entityCount, glm::ivec2* canvasDimensions);
	void modulateAlpha(
		float& outOpacity, float& outFlow, 
		glm::vec3* pos, glm::vec3* origin, glm::vec3* dir, Input* input, 
		int* shardCount, int* anchorCount, int* splineCount);
};

#endif