#ifndef TSET_FX_GRADIENT_H
#define TSET_FX_GRADIENT_H

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

class TSet_FX_Gradient : public ToolSettings // 8200
{
public:
	Gradient gradient;

	glm::vec3 direction;

	// Sampling, masking, and channel blending parameters
	float mixAmount;

	// Unit Tracking for Gradient
	bool rebuildBounds = true;
	glm::vec2 fillX = glm::vec2(0.0f);
	glm::vec2 fillY = glm::vec2(0.0f);
	glm::vec3 center = glm::vec3(0.0f);
	bool isPortrait = false;

	virtual TSetType getType() { return type; }

	virtual TSetControl_Node* getControlNode(int settingSig, int subSig);
	virtual TSetController* getController(int settingSig, int subSig);
	virtual TSetGraph* getGraph(int settingSig, int subSig);
	virtual TSetNoise* getNoise(int settingSig, int subSig);

	virtual std::shared_ptr<CObject> putProperty(
		std::shared_ptr<CInterpreter> interpreter, std::shared_ptr<Tool> owner,
		int settingsSig, int subSig, std::shared_ptr<CObject> object, bool isGet, bool asPercentage, bool asString);
	TSet_FX_Gradient();
	void updateXY(glm::vec3 pos);
	void initializeData(CColor FGColor, CColor BGColor, int tipSize);
};

#endif